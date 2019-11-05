//
// Created by HUANG WEN JUN on 2019/11/5.
//

#pragma once
#include <QQuickItem>
#include <QSGGeometryNode>

class ClockCircle : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor borderNonActiveColor READ borderNonActiveColor WRITE setBorderNonActiveColor NOTIFY borderNonActiveColorChanged)
  public:
    explicit ClockCircle(QQuickItem* parent = nullptr);

  protected:
    virtual QSGNode*
    updatePaintNode(QSGNode* oldNode,
                    UpdatePaintNodeData* updatePaintNodeData) override;

  private:
    QSGGeometryNode* _borderActiveNode;
};
