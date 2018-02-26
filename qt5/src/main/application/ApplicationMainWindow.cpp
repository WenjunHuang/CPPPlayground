//
// Created by xxzyjy on 16/12/2017.
//

#include "ApplicationMainWindow.h"
#include <QTextDocument>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QGuiApplication>
#include <QMessageBox>

ApplicationMainWindow::ApplicationMainWindow() : textEdit(new QPlainTextEdit) {
  setCentralWidget(textEdit);

  createActions();
  createStatusBar();

  readSettings();

  connect(textEdit->document(), &QTextDocument::contentsChanged,
          this, &ApplicationMainWindow::documentWasModified);

  QGuiApplication::setFallbackSessionManagementEnabled(false);
  connect(qApp, &QGuiApplication::commitDataRequest,
          this,
          &ApplicationMainWindow::commitData);

  setCurrentFile(QString());
  setUnifiedTitleAndToolBarOnMac(true);

}

void ApplicationMainWindow::closeEvent(QCloseEvent *event) {
  if (maybeSave()) {
    writeSettings();
    event->accept();
  } else {
    event->ignore();
  }
}

void ApplicationMainWindow::newFile() {

}

void ApplicationMainWindow::createActions() {
  auto fileMenu = menuBar()->addMenu(tr("&File"));
  auto fileToolBar = addToolBar(tr("File"));
  auto newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));

  auto newAct = new QAction(newIcon, tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, &QAction::triggered, this, &ApplicationMainWindow::newFile);
  fileMenu->addAction(newAct);
  fileToolBar->addAction(newAct);

  auto openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
  auto openAct = new QAction(openIcon, tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, &QAction::triggered, this, &ApplicationMainWindow::open);

  fileMenu->addAction(openAct);
  fileToolBar->addAction(openAct);

  auto saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
  auto saveAct = new QAction(saveIcon, tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, &QAction::triggered, this, &ApplicationMainWindow::save);
  fileMenu->addAction(saveAct);
  fileToolBar->addAction(saveAct);

  auto saveAsIcon = QIcon::fromTheme("document-save-as");
  auto saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &ApplicationMainWindow::saveAs);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));

  fileMenu->addSeparator();

  auto exitIcon = QIcon::fromTheme("application-exit");
  auto exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
  exitAct->setShortcuts(QKeySequence::Quit);
  exitAct->setStatusTip(tr("Exit the application"));

  auto editMenu = menuBar()->addMenu(tr("&Edit"));
  auto editToolBar = addToolBar(tr("Edit"));

  auto cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
  auto cutAct = new QAction(cutIcon, tr("Cu&t"), this);
  cutAct->setShortcuts(QKeySequence::Cut);
  cutAct->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
  connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
  editMenu->addAction(cutAct);
  editToolBar->addAction(cutAct);

  auto copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
  auto copyAct = new QAction(copyIcon, tr("&Copy"), this);
  copyAct->setShortcuts(QKeySequence::Copy);
  copyAct->setStatusTip(tr("Copy the current selection's content to the clipboard"));
  connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
  editMenu->addAction(copyAct);
  editToolBar->addAction(copyAct);

  auto pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
  auto pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
  pasteAct->setShortcuts(QKeySequence::Paste);
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
  connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
  editMenu->addAction(pasteAct);
  editToolBar->addAction(pasteAct);

  menuBar()->addSeparator();
}

void ApplicationMainWindow::createStatusBar() {
  statusBar()->showMessage(tr("Ready"));
}

void ApplicationMainWindow::readSettings() {

}

void ApplicationMainWindow::setCurrentFile(QString file) {

}

bool ApplicationMainWindow::maybeSave() {
  if (!textEdit->document()->isModified())
    return true;

  auto ret = QMessageBox::warning(this, tr("Application"),
                                  tr("The document has been modified"),
                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  switch(ret) {
    case QMessageBox::Save:
      return save();
    case QMessageBox::Cancel:
      return false;
    default:
      break;
  }
  return true;
}

void ApplicationMainWindow::writeSettings() {
}

void ApplicationMainWindow::commitData(QSessionManager &) {
}

void ApplicationMainWindow::documentWasModified() {
}

bool ApplicationMainWindow::saveAs() {
  return false;
}

void ApplicationMainWindow::about() {
}

bool ApplicationMainWindow::save() {
  return false;
}

void ApplicationMainWindow::open() {
}
