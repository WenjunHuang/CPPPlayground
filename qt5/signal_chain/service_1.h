//
// Created by rick on 2020/4/5.
//

#pragma once
#include <QObject>

class Service1;

class Service1Handler : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Service1Handler)
  public:
    Service1Handler(QObject* parent = nullptr);
    ~Service1Handler();

  signals:
    void finished();


  private:
    friend class Service1;
    void finish();
};

class Service1 : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Service1)

  private:
    Service1();

    static Service1* instance_;

    QThread* thread_;

  public:
    static void initialize();
    static Service1* instance();

  public:
    bool event(QEvent *event) override;
    void setThread(QThread* thread){
        thread_ = thread;
        moveToThread(thread_);
    }
    Service1Handler * doSomething();
};
