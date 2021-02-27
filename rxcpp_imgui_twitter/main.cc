//
// Created by rick on 2020/11/14.
//
#include "json.hpp"
#include "model.h"
#include "rxcurl.h"
#include "rximgui.h"
#include "tweets.h"
#include "util.h"
#include <GLFW/glfw3.h>
#include <chrono>
#include <curl/curl.h>
#include <deque>
#include <fstream>
#include <gl/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <iostream>
#include <map>
#include <random>
#include <range/v3/all.hpp>
#include <regex>
#include <rxcpp/rx.hpp>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>

using json = nlohmann::json;
using namespace std;
using namespace std::chrono;
using namespace rxcurl;
using namespace rximgui;
using namespace ::util;
using namespace model;
using namespace tweets;

const ImVec4 clearColor = ImColor(114, 114, 154);
const auto length       = milliseconds(60000);

const auto every = milliseconds(5000);
auto keep        = minutes(10);

template <class F>
inline void updateGroups(Model& model, milliseconds timestamp,
                         milliseconds window, F&& f) {
    auto& md = model.data;
    auto& m  = *md;

    auto searchBegin =
        duration_cast<minutes>(duration_cast<minutes>(timestamp) - window);
    auto searchEnd = timestamp;
    for (auto offset = milliseconds(0); searchBegin + offset < searchEnd;
         offset += duration_cast<milliseconds>(every)) {
        auto key =
            TimeRange(searchBegin + offset, searchBegin + offset + length);
        auto it = m.groupedTweets.find(key);

        if (it == m.groupedTweets.end()) {
            m.groups.push_back(key);
            m.groups |= ranges::actions::sort(std::less<TimeRange>{});
            it = m.groupedTweets
                     .insert(make_pair(key, make_shared<TweetGroup>()))
                     .first;
        }
    }

    while (!m.groups.empty() &&
           m.groups.front().begin + keep < m.groups.back().begin) {
        m.groupedTweets.erase(m.groups.front());
        m.groups.pop_front();
    }
}

string settingsFile;
json settings;

static void glfwErrorCallback(int error, const char* description) {
    std::cerr << "Glfw Error " << error << ":" << description << std::endl;
}

int main(int argc, char* argv[]) {
    auto command = string{};
    for (auto cursor = argv, end = argv + argc; cursor != end; ++cursor) {
        command += string{*cursor};
    }

    cerr << "command = " << command.c_str() << endl;
    auto exeFile = string{argv[0]};
    cerr << "exe = " << exeFile.c_str() << endl;

    auto exeDir = exeFile.substr(0, exeFile.find_last_of('/'));
    cerr << "dir = " << exeDir.c_str() << endl;

    auto exeParent = exeDir.substr(0, exeDir.find_last_of('/'));
    cerr << "parent = " << exeParent.c_str() << endl;

    auto selector = string{tolower(argc > 1 ? argv[1] : "")};

    const bool playback = argc == 3 && selector == "playback";
    const bool gui      = argc == 6;

    bool dumpjson = argc == 7 && selector == "dumpjson";
    bool dumptext = argc == 7 && selector == "dumptext";

    string iniFile = exeParent + "/Resources/imgui.ini";

    cerr << "dir = " << exeDir.c_str() << endl;

    bool setting = false;
    settingsFile = exeDir + "/settings.json";

    if (argc == 2 && ifstream(argv[1]).good()) {
        setting      = true;
        settingsFile = argv[1];
    } else if (!playback && argc == 3 && selector == "setting") {
        setting      = true;
        settingsFile = argv[2];
    } else if (argc == 1 || argc == 2) {
        setting = true;
    }

    if (setting) {
        cerr << "settings = " << settingsFile.c_str() << endl;
        ifstream i{settingsFile};
        if (i.good()) {
            i >> settings;
        }
    }
    if (!playback && !dumptext && !dumpjson && !gui && !setting) {
        printf("twitter <settings file path>\n");
        printf("twitter SETTING <settings file path>\n");
        printf("twitter PLAYBACK <json file path>\n");
        printf("twitter DUMPJSON <CONS_KEY> <CONS_SECRET> <ATOK_KEY> "
               "<ATOK_SECRET> [sample.json | filter.json?track=<topic>]\n");
        printf("twitter DUMPTEXT <CONS_KEY> <CONS_SECRET> <ATOK_KEY> "
               "<ATOK_SECRET> [sample.json | filter.json?track=<topic>]\n");
        printf("twitter          <CONS_KEY> <CONS_SECRET> <ATOK_KEY> "
               "<ATOK_SECRET> [sample.json | filter.json?track=<topic>]\n");
        return -1;
    }

    int argOffset = 1;
    if (gui) {
        argOffset = 0;
    }

    if (settings.count("Keep") == 0) {
        settings["Keep"] = keep.count();
    } else {
        keep = minutes(settings["Keep"].get<int>());
    }

    if (settings.count("Query") == 0) {
        settings["Query"] = json::parse(R"({"Action":"sample"})");
    }

    if (settings.count("WordFilter") == 0) {
        settings["WordFilter"] = "-http,-expletive";
    }

    if (settings.count("TweetFilter") == 0) {
        settings["TweetFilter"] = "";
    }

    if (settings.count("Language") == 0) {
        settings["Language"] = "en";
    }

    // twitter api creds
    if (settings.count("ConsumerKey") == 0) {
        settings["ConsumerKey"] = string{};
    }
    if (settings.count("ConsumerSecret") == 0) {
        settings["ConsumerSecret"] = string{};
    }
    if (settings.count("AccessTokenKey") == 0) {
        settings["AccessTokenKey"] = string{};
    }
    if (settings.count("AccessTokenSecret") == 0) {
        settings["AccessTokenSecret"] = string{};
    }

    // azure ml api creds
    if (settings.count("SentimentUrl") == 0) {
        settings["SentimentUrl"] = string{};
    }
    if (settings.count("SentimentKey") == 0) {
        settings["SentimentKey"] = string{};
    }
    if (settings.count("SentimentRequests") == 0) {
        settings["SentimentRequests"] = "Off";
    }

    // google api creds
    if (settings.count("PerspectiveUrl") == 0)
        settings["PerspectiveUrl"] = string{};

    if (settings.count("PerspectiveKey") == 0) {
        settings["PerspectiveKey"] = string{};
    }
    if (settings.count("PerspectiveRequests") == 0) {
        settings["PerspectiveRequests"] = "Off";
    }

    // ==== Constants - paths
    const string URL = "https://stream.twitter.com/1.1/statuses/";
    string url       = URL;
    string filePath;
    if (!playback) {
        if (!setting) {
            // read from args
            url += argv[5 + argOffset];

            // == Twitter keys
            const char* CONS_KEY = argv[1 + argOffset];
            const char* CONS_SEC = argv[2 + argOffset];
            const char* ATOK_KEY = argv[3 + argOffset];
            const char* ATOK_SEC = argv[4 + argOffset];

            settings["ConsumerKey"]       = string(CONS_KEY);
            settings["ConsumerSecret"]    = string(CONS_SEC);
            settings["AccessTokenKey"]    = string(ATOK_KEY);
            settings["AccessTokenSecret"] = string(ATOK_SEC);
        }
    } else {
        filePath = argv[1 + argOffset];
        cerr << "file = " << filePath.c_str() << endl;
    }

    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        return 1;
#ifdef __APPLE__
    // GL 3.2 + GLSL 150
    const char* glslVersion = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glslVersion = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    auto window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example",
                                   nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    bool err = glewInit() != 0;

    if (err) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    ImGuiIO& io    = ImGui::GetIO();
    io.IniFilename = iniFile.c_str();

    // Setup Fonts
    int fontsAdded = 0;

    static const ImWchar noto[] = {0x0020, 0x0513, 0x1e00, 0x1f4d,
                                   0x2000, 0x25ca, 0xfb01, 0xfb04,
                                   0xfeff, 0xfffd, 0};
    if (ifstream(exeDir + "/NotoMono-Regular.ttf").good()) {
        ++fontsAdded;
        io.Fonts->AddFontFromFileTTF((exeDir + "/NotoMono-Regular.ttf").c_str(),
                                     13.0f, nullptr, noto);
    } else if (ifstream(exeParent + "/Resources/NotoMono-Regular.ttf").good()) {
        ++fontsAdded;
        io.Fonts->AddFontFromFileTTF(
            (exeParent + "/Resources/NotoMono-Regular.ttf").c_str(), 13.0f,
            nullptr, noto);
    }

    static ImFontConfig config;
    config.MergeMode               = true;
    static const ImWchar symbols[] = {0x20a0, 0x2e3b, 0x3008, 0x3009, 0x4dc0,
                                      0x4dff, 0xa700, 0xa71f, 0};
    if (ifstream(exeDir + "/NotoSansSymbols-Regular.ttf").good()) {
        ++fontsAdded;
        io.Fonts->AddFontFromFileTTF(
            (exeDir + "/NotoSansSymbols-Regular.ttf").c_str(), 13.0f, &config,
            symbols);
    } else if (ifstream(exeParent + "/Resources/NotoSansSymbols-Regular.ttf")
                   .good()) {
        ++fontsAdded;
        io.Fonts->AddFontFromFileTTF(
            (exeParent + "/Resources/NotoSansSymbols-Regular.ttf").c_str(),
            13.0f, &config, symbols);
    }

    if (fontsAdded) {
        io.Fonts->Build();
    }

    RXCPP_UNWIND_AUTO([&]() {
        // cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    });

    auto mainThreadId = this_thread::get_id();
    auto mainThread   = observe_on_run_loop(rl);

    auto tweetThread = observe_on_new_thread();
    auto poolThread  = observe_on_event_loop();

    auto factory = createRxCurl();

    composite_subscription lifetime;

    rxsub::replay<json, decltype(mainThread)> settingUpdate{1, mainThread,
                                                            lifetime};
    auto settingUpdates = settingUpdate.get_observable();
    auto sendSettings   = settingUpdate.get_subscriber();
    auto sf             = settingsFile;
    auto updateSettings = [sendSettings, sf](json s) {
        ofstream o{sf};
        o << setw(4) << s;
        if (sendSettings.is_subscribed()) {
            sendSettings.on_next(s);
        }
    };

    // initial update
    updateSettings(settings);

    auto useSentimentApi =
        settingUpdates | rxo::map([=](const json& settings) {
            string use =
                tolower(settings["SentimentRequests"].get<std::string>());
            bool haveUrl = settings.count("SentimentUrl") > 0 &&
                           !settings["SentimentUrl"].get<std::string>().empty();
            bool haveKey = settings.count("SentimentKey") > 0 &&
                           !settings["SentimentKey"].get<std::string>().empty();
            return use == "on" && haveUrl && haveKey;
        }) |
        debounce(milliseconds(500), mainThread) | distinct_until_changed() |
        rxo::replay(1) | ref_count() | as_dynamic();

    auto usePerspectiveApi =
        settingUpdates | rxo::map([=](const json& settings) {
            string use =
                tolower(settings["PerspectiveRequests"].get<std::string>());
            bool haveUrl =
                settings.count("PerspectiveUrl") > 0 &&
                !settings["PerspectiveUrl"].get<std::string>().empty();
            bool haveKey =
                settings.count("PerspectiveKey") > 0 &&
                !settings["PerspectiveKey"].get<std::string>().empty();
            return use == "on" && haveUrl && haveKey;
        }) |
        debounce(milliseconds(500), mainThread) | distinct_until_changed() |
        rxo::replay(1) | ref_count() | as_dynamic();

    auto urlChanges =
        settingUpdates | rxo::map([=](const json& settings) {
            string url =
                URL + settings["Query"]["Action"].get<std::string>() + ".json?";
            if (settings.count("Language") > 0) {
                url +=
                    "language=" + settings["Language"].get<std::string>() + "&";
            }
            if (settings["Query"].count("Keywords") > 0 &&
                settings["Query"]["Keywords"].is_array()) {
                url += "track=";
                for (auto& kw : settings["Query"]["Keywords"]) {
                    url += kw.get<std::string>() + ",";
                }
            }
            return url;
        }) |
        debounce(milliseconds(1000), mainThread) | distinct_until_changed() |
        tap([](string url) { std::cerr << "url = " << url << std::endl; }) |
        rxo::replay(1) | ref_count() | as_dynamic();

    // === Tweets
    observable<string> chunks;
    if (playback) {
        chunks = fileChunks(tweetThread, filePath);
    } else {
        // switch to new connection whenever the url changes
        chunks = urlChanges | rxo::map([&](const string& url) {
                     // ==== Constants - flags
                     const bool isFilter =
                         url.find("/statuses/filter") != string::npos;
                     string method = isFilter ? "POST" : "GET";

                     return twitterRequest(tweetThread, factory, url, method,
                                           settings["ConsumerKey"],
                                           settings["ConsumerSecret"],
                                           settings["AccessTokenKey"],
                                           settings["AccessTokenSecret"]) |
                            on_error_resume_next([](std::exception_ptr ep) {
                                std::cerr << rxu::what(ep) << std::endl;
                                return rxs::never<string>();
                            });
                 }) |
                 switch_on_next();
    }

    // parse tweets
    auto tweets = chunks | parseTweets(poolThread, tweetThread) |
                  rxo::map([](ParsedTweets p) {
                      p.errors | tap([](ParseError e) {
                          std::cerr << rxu::what(e.ep) << std::endl;
                      }) | subscribe<ParseError>();
                      return p.tweets;
                  }) |
                  merge(tweetThread);

    auto ts = tweets | retry() | publish() | ref_count() | as_dynamic();

    // === Model
    vector<observable<Reducer>> reducers;

    auto newJsonFile = [exeDir]() -> unique_ptr<ofstream> {
        return unique_ptr<ofstream>{new ofstream(
            exeDir + "/" +
            to_string(time_point_cast<milliseconds>(system_clock::now())
                          .time_since_epoch()
                          .count()) +
            ".json")};
    };

    auto jsonFile = newJsonFile();

    auto dumpJsonChanged = interval(every, tweetThread) |
                           rxo::map([&](long) { return dumpjson; }) |
                           distinct_until_changed() | publish() | ref_count();

    auto delayedTweets = ts | buffer_with_time(every, tweetThread) |
                         delay(length, tweetThread) | publish(lifetime) |
                         connect_forever();

    reducers.push_back(
        dumpJsonChanged | filter([](bool dj) { return dj; }) |
        rxo::map([&](bool) -> observable<Reducer> {
            return delayedTweets |
                   take_until(dumpJsonChanged |
                              filter([](bool dj) { return !dj; }) |
                              delay(length, tweetThread)) |
                   rxo::map([&](const vector<Tweet>& tws) {
                       return Reducer([&, tws](Model& m) {
                           for (auto& tw : tws) {
                               auto& tweet = tw.data->tweet;
                               auto json   = tweet.dump();
                               cout << json << "\r\n";
                               *jsonFile << json << "\r\n";
                           }
                           *jsonFile << flush;
                           return std::move(m);
                       });
                   });
        }) |
        switch_on_next(tweetThread) | noopOnError() | start_with(noop));

    reducers.push_back(
        urlChanges|
        rxo::map([=](string url){
            return Reducer([=](Model& m){
                m.data->url = url;
                return std::move(m);
            });
        })|
        noopOnError() |
        start_with(noop));

    reducers.push_back(ts|
                       onlyTweets()|
                       filter([&](const Tweet&){
                           return dumptext;
                       })|
                       tap([=](const Tweet& tw){
                           auto& tweet = tw.data->tweet;
                           if (tweet["user"]["name"].is_string() && tweet["user"]["screen_name"].is_string()) {
                               cout << "--------------------" << endl;
                               cout << tweet["user"]["name"].get<string>() << " (" << tweet["user"]["screen_name"].get<string>() << ")" << endl;
                               cout << tweetText(tweet) << endl;
                           }
                       }) |
                       noopAndIgnore() |
                       start_with(noop));
    if (!playback) {
        reducers.push_back()
    }

}