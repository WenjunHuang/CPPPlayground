//
// Created by rick on 2020/4/25.
//

#pragma once
#include <QtCore/QtCore>

class FinishedHandler : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(FinishedHandler)

  public:
    explicit FinishedHandler(QObject* parent = nullptr);

  public slots:
    void handleFinished();
};
