//
// Created by rick on 19-7-11.
//

#include "HttpClientWindow.h"
#include <QtNetwork/QtNetwork>
#include <QNetworkAccessManager>

struct HttpClientWindowPrivate {
  Q_DECLARE_PUBLIC(HttpClientWindow)
  HttpClientWindowPrivate(HttpClientWindow* ptr):q_ptr{ptr} {}
  HttpClientWindow *q_ptr;


  void initNetwork(){
    networkAccess = new QNetworkAccessManager(q_ptr);
  }
  void buildUI(){
    auto layout = new QVBoxLayout;
    auto addressLab = new QLabel("地址:");
    addressText = new QLineEdit;

    auto responseLab = new QLabel("回复:");
    responseText = new QPlainTextEdit;

    layout->setSpacing(5);
    layout->addWidget(addressLab);
    layout->addWidget(addressText);
    layout->addWidget(responseLab);
    layout->addWidget(responseText);

    auto bottomLayout = new QHBoxLayout;
    startLoad = new QPushButton("开始");
    QObject::connect(startLoad,&QPushButton::clicked,[this](){
      auto outer = this;
      QNetworkRequest request;
      request.setUrl(QUrl(addressText->text()));

      auto reply = networkAccess->get(request);
      QObject::connect(reply,&QNetworkReply::finished,[=](){
        auto contentType = reply->header(QNetworkRequest::KnownHeaders::ContentTypeHeader);
        qDebug() << contentType.toString();

        QByteArray content = reply->readAll();
        QString response = QString::fromUtf8(content);
        QJsonDocument json = QJsonDocument::fromJson(content);
        qDebug() << json.isObject() << json.isArray() << json.isEmpty() << json.isNull();
        responseText->setPlainText(response);

        reply->deleteLater();
      });

      QObject::connect(reply,&QNetworkReply::downloadProgress,[](auto bytesReceived,
                                                                 auto bytesTotal){

          qDebug() << QString("Downloading %1/%2").arg(bytesReceived).arg(bytesTotal);
      });
    });

    bottomLayout->addWidget(startLoad,0,Qt::AlignRight);
    layout->addLayout(bottomLayout);

    auto center = new QWidget();
    center->setLayout(layout);
    q_ptr->setCentralWidget(center);
    q_ptr->setWindowTitle("http client");

  }

  QNetworkAccessManager *networkAccess;
  QPushButton *startLoad;
  QLineEdit *addressText;
  QPlainTextEdit* responseText;
};

HttpClientWindow::HttpClientWindow():d_ptr{std::make_unique<HttpClientWindowPrivate>(this)} {
  Q_D(HttpClientWindow);
  d->initNetwork();
  d->buildUI();
}
HttpClientWindow::~HttpClientWindow() {}
