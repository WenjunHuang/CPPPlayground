//
// Created by HUANG WEN JUN on 2020/1/20.
//

#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <future>

class HttpWorker: public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(HttpWorker)
  public:
    explicit HttpWorker(QObject *parent = nullptr);

    std::future<QNetworkReply*> get(QUrl const& url);

    bool event(QEvent* event) override;
  private:
    void request(QUrl const& url,std::promise<QNetworkReply*>&& promise);
    QNetworkAccessManager* _networkAccessManager;
};
