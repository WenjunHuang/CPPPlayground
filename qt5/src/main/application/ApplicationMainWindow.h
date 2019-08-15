#pragma once


#include <QMainWindow>
#include <QSessionManager>
#include <QPlainTextEdit>

class ApplicationMainWindow : public QMainWindow {
Q_OBJECT
public:
  ApplicationMainWindow();

  void loadFile(const QString &fileName);

protected:
  void closeEvent(QCloseEvent *event) override;

private slots:

  void newFile();

  void open();

  bool save();

  bool saveAs();

  void about();

  void documentWasModified();

  void commitData(QSessionManager &);

private:
  QPlainTextEdit *textEdit;

  void createActions();

  void createStatusBar();

  void readSettings();

  void setCurrentFile(QString qString);

  bool maybeSave();

  void writeSettings();
};



