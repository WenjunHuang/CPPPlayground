//
// Created by rick on 2020/11/14.
//

#pragma once
#include <chrono>
#include <curl/curl.h>
#include <iostream>
#include <map>
#include <memory>
#include <rxcpp/rx.hpp>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;
namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
    using namespace rxcpp::subjects;
} // namespace Rx
using namespace Rx;

namespace rxcurl {
    struct RxCurlState {
        RxCurlState()
            : thread{observe_on_new_thread()}, worker{},
              curlm{curl_multi_init()} {
            worker = observable<void, void>::create<CURLMsg*>(
                         [this](subscriber<CURLMsg*> out) {
                             while (out.is_subscribed()) {
                                 int running = 0;
                                 curl_multi_perform(curlm, &running);
                                 for (;;) {
                                     CURLMsg* message = nullptr;
                                     int remaining    = 0;
                                     message = curl_multi_info_read(curlm,
                                                                    &remaining);
                                     out.on_next(message);
                                     if (!!message && remaining > 0) {
                                         continue;
                                     }
                                     break;
                                 }

                                 int handlecount = 0;
                                 curl_multi_wait(curlm, nullptr, 0, 500,
                                                 &handlecount);
                                 if (handlecount == 0) {
                                     this_thread::sleep_for(milliseconds(100));
                                 }
                             }
                             out.on_completed();
                         }) |
                     subscribe_on(thread) |
                     finally([]() { cerr << "rxcurl worker exit" << endl; }) |
                     publish() | connect_forever();
        }

        RxCurlState(const RxCurlState&) = delete;
        RxCurlState(RxCurlState&&)      = delete;
        RxCurlState& operator=(const RxCurlState&) = delete;
        RxCurlState& operator=(RxCurlState&&) = delete;

        observe_on_one_worker thread;
        observable<CURLMsg*> worker;
        CURLM* curlm;
    };

    struct HttpRequest {
        string url;
        string method;
        std::map<string, string> headers;
        string body;
    };

    struct HttpState {
        shared_ptr<RxCurlState> rxcurl;
        HttpRequest request;
        string error;
        CURLcode code;
        int httpStatus;
        subject<string> chunkbus;
        unique_ptr<subscriber<string>> chunkout;
        CURL* curl;
        curl_slist* headers;
        vector<string> strings;

        HttpState(shared_ptr<RxCurlState> m, HttpRequest r)
            : rxcurl{m}, request{r}, code{CURLE_OK},
              httpStatus{0}, curl{nullptr}, headers{nullptr} {
            error.resize(CURL_ERROR_SIZE);
        }
        ~HttpState() {
            if (!!curl) {
                // remove on worker thread
                auto localcurl    = curl;
                auto localHeaders = headers;
                auto localRxCurl  = rxcurl;
                auto localRequest = request;
                chunkbus.get_subscription().unsubscribe();
                auto localChunkout = chunkout.release();

                rxcurl->worker.take(1)
                    .tap([=](CURLMsg*) {
                        curl_multi_remove_handle(localRxCurl->curlm, localcurl);
                        curl_easy_cleanup(localcurl);
                        curl_slist_free_all(localHeaders);
                        delete localChunkout;
                    })
                    .subscribe();

                curl    = nullptr;
                headers = nullptr;
            }
        };
    }; // namespace RxCurl

    struct HttpException : runtime_error {
        shared_ptr<HttpState> state;

        explicit HttpException(const shared_ptr<HttpState>& s)
            : runtime_error{s->error}, state{s} {}

        CURLcode code() const { return state->code; }

        int httpStatus() const { return state->httpStatus; }
    };

    struct HttpBody {
        observable<string> chunks;
        observable<string> complete;
    };

    struct HttpResponse {
        const HttpRequest request;
        HttpBody body;
        shared_ptr<HttpState> state;

        CURLcode code() const { return state->code; }

        int httpStatus() const { return state->httpStatus; }
    };

    size_t rxcurlHttpCallback(char* ptr, size_t size, size_t nmemb,
                              subscriber<string>* out) {
        int realSize = size * nmemb;
        string chunk;
        chunk.assign(ptr, realSize);
        out->on_next(chunk);
        return realSize;
    }

    struct RxCurl {
        shared_ptr<RxCurlState> state;

        observable<HttpResponse> create(HttpRequest request) const {
            return observable<>::create<HttpResponse>(
                [=](subscriber<HttpResponse>& out) {
                    auto requestState = make_shared<HttpState>(state, request);
                    HttpResponse r{request, HttpBody{}, requestState};
                    r.body.chunks = r.state->chunkbus.get_observable().tap(
                        [requestState](const string&) {}); // keep connection alive
                    r.body.complete = r.state->chunkbus.get_observable()
                                          .tap([requestState](const string&) {}) // keep connection alive
                                          .start_with(string{})
                                          .sum()
                                          .replay(1)
                                          .ref_count();

                    out.on_next(r);
                    out.on_completed();

                    auto localState = state;
                    // start on worker thread
                    state->worker.take(1)
                        .tap([r, localState](CURLMsg*) {
                            auto curl     = curl_easy_init();
                            auto& request = r.state->request;

                            curl_easy_setopt(curl, CURLOPT_URL,
                                             request.url.c_str());

                            if (request.method == "POST") {
                                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                                curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                                                 request.body.c_str());
                            }

                            auto& strings = r.state->strings;
                            auto& headers = r.state->headers;
                            for (auto& h : request.headers) {
                                strings.push_back(h.first + ": " + h.second);
                                headers = curl_slist_append(
                                    headers, strings.back().c_str());
                            }
                            if (!!headers) {
                                curl_easy_setopt(curl, CURLOPT_HTTPHEADER,
                                                 headers);
                            }

                            // - User agent name
                            curl_easy_setopt(curl, CURLOPT_USERAGENT,
                                             "rxcpp curl client 1.1");
                            // - HTTP STATUS >= 400 --> ERROR
                            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

                            // - Callback function
                            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                                             rxcurlHttpCallback);
                            // - Write data
                            r.state->chunkout.reset(new subscriber<string>(
                                r.state->chunkbus.get_subscriber()));
                            curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                                             (void*)r.state->chunkout.get());

                            // - keep error messages
                            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER,
                                             &r.state->error[0]);

                            r.state->curl = curl;
                            curl_multi_add_handle(localState->curlm, curl);
                        })
                        .subscribe();

                    weak_ptr<HttpState> wrs = requestState;

                    // extract completion and result
                    state->worker
                        .filter([wrs](CURLMsg* message) {
                            auto rs = wrs.lock();
                            return !!rs && !!message &&
                                   message->easy_handle == rs->curl &&
                                   message->msg == CURLMSG_DONE;
                        })
                        .take(1)
                        .tap([wrs](CURLMsg* message) {
                            auto rs = wrs.lock();
                            if (!rs)
                                return;

                            rs->error.resize(strlen(&rs->error[0]));
                            auto chunkout   = rs->chunkbus.get_subscriber();
                            long httpStatus = 0;

                            curl_easy_getinfo(rs->curl, CURLINFO_RESPONSE_CODE,
                                              &httpStatus);
                            rs->httpStatus = httpStatus;

                            if (message->data.result != CURLE_OK) {
                                rs->code = message->data.result;
                                if (rs->error.empty()) {
                                    rs->error = curl_easy_strerror(
                                        message->data.result);
                                }
                                observable<>::error<string>(HttpException{rs})
                                    .subscribe(chunkout);
                                return;
                            } else if (httpStatus > 499) {
                                observable<>::error<string>(HttpException{rs})
                                    .subscribe(chunkout);
                                return;
                            }
                            chunkout.on_completed();
                        })
                    .subscribe();
                });
        }
    };

    RxCurl createRxCurl() {
        RxCurl r{make_shared<RxCurlState>()};
        return r;
    }
} // namespace RxCurl