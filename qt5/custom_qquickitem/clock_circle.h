//
// Created by HUANG WEN JUN on 2019/11/5.
//

#pragma once
#include <QQuickItem>
#include <QSGGeometryNode>
#include <QTime>
#include <QTimer>

class ClockCircle : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE
                   setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor borderNonActiveColor READ borderNonActiveColor WRITE
                   setBorderNonActiveColor NOTIFY borderNonActiveColorChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(QTime circleTime READ circleTime WRITE setCircleTime NOTIFY
                   circleTimeChanged)
  public:
    explicit ClockCircle(QQuickItem* parent = nullptr);

    Q_INVOKABLE void clear();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE bool isRunning();

    QString name() const;
    QColor backgroundColor() const;
    QColor borderNonActiveColor() const;
    qreal angle() const;
    QTime circleTime() const;

  public slots:
    void setName(const QString& name);
    void setBackgroundColor(const QColor backgroundColor);
    void setBorderActiveColor(const QColor borderActiveColor);
    void setAngle(const qreal angle);
    void setCircleTime(const QTime circleTime);

  signals:
    void cleared();
    void nameChanged(const QString& name);
    void backgroundColorChanged(QColor backgroundColor);
    void borderActiveColorChanged(const QColor borderActiveColor);
    void borderNonActiveColorChanged(const QColor borderNonActiveColor);
    void angleChanged(const qreal angle);
    void circleTimeChanged(const QTime circleTime);

  protected:
    virtual QSGNode*
    updatePaintNode(QSGNode* oldNode,
                    UpdatePaintNodeData* updatePaintNodeData) override;

  private:
    QString _name;
    QColor _backgroundColor;
    QColor _borderActiveColor;
    QColor _borderNonActiveColor;
    qreal _angle;
    QTime _circleTime;
    QTimer *_internalTimer;
    QSGGeometryNode* _borderActiveNode;
};
