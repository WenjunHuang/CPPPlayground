//
// Created by rick on 19-7-11.
//

#pragma once
#include <memory>
#include <QtWidgets>

class HttpClientWindowPrivate;

class HttpClientWindow: public QMainWindow {
  Q_OBJECT
public:
  explicit HttpClientWindow();
  ~HttpClientWindow();
private:
  Q_DECLARE_PRIVATE(HttpClientWindow)

  std::unique_ptr<HttpClientWindowPrivate> d_ptr;

};



