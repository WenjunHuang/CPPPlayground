#pragma once


#include <QPushButton>

class ThreeStateButton : public QPushButton {
Q_OBJECT
  Q_PROPERTY(qint8 index READ index WRITE setIndex NOTIFY indexChanged)
  Q_PROPERTY(QString pixmapUrl READ pixmapUrl WRITE setPixmapUrl NOTIFY pixmapUrlChanged)
public:
  explicit ThreeStateButton(QWidget *parent = nullptr);

  qint8 index() const;
  void setIndex(qint8 index);

  QString pixmapUrl() const;
  void setPixmapUrl(const QString& url);

signals:
  void indexChanged();
  void pixmapUrlChanged();


protected:
  void paintEvent(QPaintEvent *) override;

  void enterEvent(QEvent *) override;

  void leaveEvent(QEvent *) override;

  void mousePressEvent(QMouseEvent *) override;

  void mouseReleaseEvent(QMouseEvent *) override;

private:
  QVector<QPixmap> _pixmaps;
  QString _pixmapUrl;
  qint8 _index;
public:
};



