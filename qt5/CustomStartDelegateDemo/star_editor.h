//
// Created by rick on 2022/3/2.
//

#pragma once

#include <QWidget>

class StarEditor : public QWidget {
Q_OBJECT
public:
    explicit StarEditor(QWidget *parent = nullptr);

signals:

    void editingFinished();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    int getStartRating() const;

    void setStartRating(int startRating);

public:
    QSize sizeHint() const override;

private:
    int _startRating;

private:
    QPolygon _poly;
};


