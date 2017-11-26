#pragma once

#include <memory>
#include <QDialog>
#include <QLabel>

class InputDlgPimp;

class InputDlg : public QDialog {
Q_OBJECT
public:
  InputDlg(QWidget *parent = nullptr);
  ~InputDlg();

private slots:

  void ChangeName();

  void ChangeSex();

  void ChangeAge();

  void ChangeScore();

private:
  std::unique_ptr<InputDlgPimp> pimp;
};



