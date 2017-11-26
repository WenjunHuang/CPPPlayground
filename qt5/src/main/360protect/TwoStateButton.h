#pragma once


#include <QPushButton>
#include <QToolButton>

class TwoStateButton : public QPushButton {
Q_OBJECT
  Q_PROPERTY(QPixmap pixmap
               READ
                 pixmap
               WRITE
                 setPixmap
               NOTIFY
               pixmapChanged)
public:
  TwoStateButton(QWidget *parent = nullptr);

  QPixmap pixmap() const { return _pixmap; }

  void setPixmap(const QPixmap &pixmap) {
    _pixmap = pixmap;
    emit pixmapChanged();
  }

signals:
  void pixmapChanged();

protected:
  void paintEvent(QPaintEvent *) override;

  void enterEvent(QEvent *) override;

  void leaveEvent(QEvent *) override;

private:
  QPixmap _pixmap;
  bool _enter;
  QPixmap _pixmapBorder;
};



