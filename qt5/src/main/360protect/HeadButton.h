#pragma once


#include <QPushButton>

class HeadButton : public QPushButton {
Q_OBJECT
  Q_PROPERTY(qint8 index
               READ
               index
               WRITE
               setIndex
               NOTIFY
               indexChanged)
public:
  explicit HeadButton(QWidget *parent = nullptr);

  qint8 index() const;
  void setIndex(qint8 index);

protected:
  void paintEvent(QPaintEvent *) override;

  void enterEvent(QEvent *) override;

  void leaveEvent(QEvent *) override;

signals:
  void indexChanged();

private:
  QVector<QPixmap> _pixmaps;
  qint8 _index;
};



