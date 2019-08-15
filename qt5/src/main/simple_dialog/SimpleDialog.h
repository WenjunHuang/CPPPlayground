#pragma once

#include <QDialog>

namespace Ui {
  class SimpleDialog;
}
class SimpleDialog : public QDialog {
Q_OBJECT
public:
  explicit SimpleDialog(QWidget *parent = 0);

  ~SimpleDialog();

private slots:
  void onLineEditTextChanged();

private:
  Ui::SimpleDialog *ui;
};



