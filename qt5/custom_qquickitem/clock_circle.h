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
    Q_PROPERTY(QColor borderActiveColor READ borderActiveColor WRITE
                   setBorderActiveColor NOTIFY borderActiveColorChanged)
    Q_PROPERTY(QColor borderNonActiveColor READ borderNonActiveColor WRITE
                   setBorderNonActiveColor NOTIFY borderNonActiveColorChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(QTime circleTime READ circleTime WRITE setCircleTime NOTIFY
                   circleTimeChanged)
  public:
    explicit ClockCircle(QQuickItem* parent = nullptr);

    Q_INVOKABLE void clear() {
        setCircleTime(QTime(0,0,0,0));
        setAngle(0);
        update();
        emit cleared();
    }

    Q_INVOKABLE void start(){
        _internalTimer->start(50);
    }
    Q_INVOKABLE void stop() {
        _internalTimer->stop();
    }
    Q_INVOKABLE bool isRunning(){
        return _internalTimer->isActive();
    }

    QString name() const {
        return _name;
    }
    QColor backgroundColor() const {
        return _backgroundColor;
    }
    QColor borderNonActiveColor() const {
        return _borderNonActiveColor;
    }
    QColor borderActiveColor() const {
        return _borderActiveColor;
    }
    qreal angle() const {
        return _angle;
    }
    QTime circleTime() const {
        return _circleTime;
    }

  public slots:
    void setName(const QString& name) {
        if (_name == name)
            return;

        _name = name;
        emit nameChanged(name);
    };
    void setBackgroundColor(const QColor backgroundColor) {
        if (_backgroundColor == backgroundColor)
            return;

        _backgroundColor = backgroundColor;
        emit backgroundColorChanged(backgroundColor);
    }
    void setBorderActiveColor(const QColor borderActiveColor){
        if (_borderActiveColor == borderActiveColor)
            return;

        _borderActiveColor = borderActiveColor;
        emit borderActiveColorChanged(borderActiveColor);
    }
    void setBorderNonActiveColor(const QColor borderNonActiveColor){
        if (_borderNonActiveColor == borderNonActiveColor)
            return;

        _borderNonActiveColor = borderNonActiveColor;
        emit borderNonActiveColorChanged(borderNonActiveColor);
    }
    void setAngle(const qreal angle) {
        if (_angle == angle)
            return;

        _angle = angle;
        if (_angle >= 360) _angle = 0;
        emit angleChanged(_angle);
    }
    void setCircleTime(const QTime circleTime){
        if (_circleTime == circleTime)
            return;
        _circleTime = circleTime;
        emit circleTimeChanged(circleTime);
    }

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
