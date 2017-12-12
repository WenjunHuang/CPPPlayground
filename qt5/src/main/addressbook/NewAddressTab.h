#pragma once


#include <QWidget>

class NewAddressTab : public QWidget {
Q_OBJECT
public:
  NewAddressTab(QWidget *parent = nullptr);

public slots:

  void addEntry();

signals:
  void sendDetails(QString name, QString address);

};



