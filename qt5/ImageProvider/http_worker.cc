//
// Created by HUANG WEN JUN on 2020/1/20.
//

#include "http_worker.h"
#include <QCoreApplication>
#include <QEvent>
#include <QNetworkReply>

const int kHttpRequestEventType = QEvent::registerEventType();

class HttpRequestEvent : public QEvent {
  public:
    HttpRequestEvent(QUrl url, std::promise<QNetworkReply*>&& promise)
        : QEvent(static_cast<QEvent::Type>(kHttpRequestEventType)),
          url{std::move(url)}, promise{std::move(promise)} {}

    QUrl url;
    std::promise<QNetworkReply*> promise;
};

HttpWorker::HttpWorker(QObject* parent) : QObject(parent) {
    _networkAccessManager = new QNetworkAccessManager(this);
}

bool HttpWorker::event(QEvent* event) {
    if (event->type() == kHttpRequestEventType) {
        auto requestEvent = dynamic_cast<HttpRequestEvent*>(event);

        QNetworkRequest request(requestEvent->url);
        auto reply = _networkAccessManager->get(request);
        requestEvent->promise.set_value(reply);
    }
}

void HttpWorker::request(QUrl const& url,
                         std::promise<QNetworkReply*>&& promise) {
    QCoreApplication::postEvent(this,
                                new HttpRequestEvent(url, std::move(promise)));
}

std::future<QNetworkReply*> HttpWorker::get(QUrl const& url) {
    std::promise<QNetworkReply*> promise;
    auto future = promise.get_future();
    request(url, std::move(promise));
    return future;
}
