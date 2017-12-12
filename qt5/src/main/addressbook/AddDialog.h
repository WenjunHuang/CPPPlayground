#pragma once


#include <QDialog>
class QLineEdit;
class QTextEdit;
class AddDialog : public QDialog{
  Q_OBJECT
public:
  AddDialog(QWidget *parent = nullptr);

  QLineEdit *nameText;
  QTextEdit *addressText;
};



