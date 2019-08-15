#pragma once
#include <QWidget>
#include <QLabel>
#include <QTextEdit>

class ShowWidget : public QWidget {
  Q_OBJECT
public:
  explicit ShowWidget(QWidget *parent = nullptr);
  QImage image;
  QLabel *imageLabel;
  QTextEdit *text;
};



