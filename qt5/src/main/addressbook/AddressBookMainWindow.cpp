//
// Created by xxzyjy on 11/12/2017.
//

#include <QMenuBar>
#include <QFileDialog>
#include "AddressBookMainWindow.h"

AddressBookMainWindow::AddressBookMainWindow() {
  _addressWidget = new AddressWidget;
  setCentralWidget(_addressWidget);
  createMenus();
  setWindowTitle(tr("AddressBook"));
}

void AddressBookMainWindow::updateActions(const QItemSelection &selection) {
  auto indexes = selection.indexes();
  if (!indexes.isEmpty()){
    _removeAct->setEnabled(true);
    _editAct->setEnabled(true);
  }else {
    _removeAct->setEnabled(false);
    _editAct->setEnabled(false);
  }

}

void AddressBookMainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty())
    _addressWidget->readFromFile(fileName);
}

void AddressBookMainWindow::saveFile() {
  auto fileName = QFileDialog::getSaveFileName(this);
  if (!fileName.isEmpty())
    _addressWidget->writeToFile(fileName);
}

void AddressBookMainWindow::createMenus() {
  _fileMenu = menuBar()->addMenu(tr("&File"));

  _openAct = new QAction(tr("&Open.."));
  _fileMenu->addAction(_openAct);
  connect(_openAct,&QAction::triggered,this,&AddressBookMainWindow::openFile);

  _saveAct = new QAction(tr("&Save As..."));
  _fileMenu->addAction(_saveAct);
  connect(_saveAct,&QAction::triggered,this,&AddressBookMainWindow::saveFile);

  _fileMenu->addSeparator();

  _exitAct = new QAction(tr("E&xit"),this);
  _fileMenu->addAction(_exitAct);
  connect(_exitAct,&QAction::triggered,this,&QWidget::close);

  _toolMenu = menuBar()->addMenu(tr("&Tools"));
  _addAct = new QAction(tr("&Add Entry...."));
  _toolMenu->addAction(_addAct);
  connect(_addAct,&QAction::triggered,_addressWidget,&AddressWidget::showAddEntryDialog);

  _editAct = new QAction(tr("&Edit Entry"));
  _editAct->setEnabled(false);
  _toolMenu->addAction(_editAct);
  connect(_editAct,&QAction::triggered,_addressWidget,&AddressWidget::editEntry);

  _toolMenu->addSeparator();

  _removeAct = new QAction(tr("&Remove Entry"),this);
  _removeAct->setEnabled(false);
  _toolMenu->addAction(_removeAct);
  connect(_removeAct,&QAction::triggered,_addressWidget,&AddressWidget::removeEntry);

  connect(_addressWidget,&AddressWidget::selectionChanged,this,&AddressBookMainWindow::updateActions);



}
