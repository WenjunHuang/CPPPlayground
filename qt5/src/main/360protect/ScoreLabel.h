#pragma once


#include <QLabel>

class ScoreLabel : public QLabel {
Q_OBJECT
  Q_PROPERTY(QPixmap pixmap
               READ
                 pixmap
               WRITE
                 setPixmap
               NOTIFY
               pixmapChanged)
public:
  ScoreLabel(QWidget *parent = nullptr);

  QPixmap pixmap() const {
    return _pixmap;
  }

  void setPixmap(const QPixmap &pixmap) {
    _pixmap = pixmap;
    emit pixmapChanged();
  }

signals:
  void pixmapChanged();

protected:
  void paintEvent(QPaintEvent *) override;

private:
  QPixmap _pixmap;

};



