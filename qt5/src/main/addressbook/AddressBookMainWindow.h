#pragma once


#include <QMainWindow>
#include <QItemSelection>
#include "AddressWidget.h"

class AddressBookMainWindow : public QMainWindow{
  Q_OBJECT
public:
  AddressBookMainWindow();

  private slots:
  void updateActions(const QItemSelection& selection);
  void openFile();
  void saveFile();

private:
  void createMenus();
  AddressWidget* _addressWidget;
  QMenu *_fileMenu;


};



