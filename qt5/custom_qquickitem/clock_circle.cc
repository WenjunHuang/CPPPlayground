//
// Created by HUANG WEN JUN on 2019/11/5.
//

#include "clock_circle.h"
#include <QSGFlatColorMaterial>
#include <cmath>

constexpr double Pi      = 3.14159265358979323846264338327950288419717;
constexpr double Deg2Rad = Pi / 180;
ClockCircle::ClockCircle(QQuickItem* parent)
    : QQuickItem(parent), _backgroundColor(Qt::white),
      _borderActiveColor(Qt::blue), _borderNonActiveColor(Qt::gray), _angle(0),
      _circleTime(QTime(0, 0, 0, 0)) {
    setFlag(QQuickItem::ItemHasContents);
    setAntialiasing(true);

    _internalTimer = new QTimer(this);
    connect(_internalTimer, &QTimer::timeout, [=]() {
        setAngle(angle() + 0.3);
        setCircleTime(circleTime().addMSecs(50));
        update();
    });
}
QSGNode* ClockCircle::updatePaintNode(
    QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* updatePaintNodeData) {
    Q_UNUSED(updatePaintNodeData)

    if (!oldNode) {
        auto drawCircle = [this](double radius, QSGGeometry* geometry) {
            for (int i = 0; i < 360; ++i) {
                double rad = (i - 90) * Deg2Rad;
                geometry->vertexDataAsPoint2D()[i].set(std::cos(rad) * radius +
                                                       width() / 2,
                                                       std::sin(rad)* radius + height() /2);
            }
        };

        auto borderNonActiveGeometry =
            new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 360);
        borderNonActiveGeometry->setDrawingMode(GL_POLYGON);
        drawCircle(width() / 2, borderNonActiveGeometry);

        auto borderNonActiveMaterial = new QSGFlatColorMaterial();
        borderNonActiveMaterial->setColor(_borderNonActiveColor);

        auto borderNonActiveNode = new QSGGeometryNode();
        borderNonActiveNode->setGeometry(borderNonActiveGeometry);
        borderNonActiveNode->setMaterial(borderNonActiveMaterial);
        borderNonActiveNode->setFlags(QSGNode::OwnsGeometry |
                                      QSGNode::OwnsMaterial);

        auto borderActiveGeometry =
            new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        borderActiveGeometry->setDrawingMode(GL_POLYGON);

        auto borderActiveMaterial = new QSGFlatColorMaterial();
        borderActiveMaterial->setColor(_borderActiveColor);

        _borderActiveNode = new QSGGeometryNode();
        _borderActiveNode->setGeometry(borderActiveGeometry);
        _borderActiveNode->setMaterial(borderActiveMaterial);

        _borderActiveNode->setFlags(QSGNode::OwnsGeometry |
                                    QSGNode::OwnsMaterial);
        borderNonActiveNode->appendChildNode(_borderActiveNode);

        auto backgroundGeometry =
            new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 360);
        backgroundGeometry->setDrawingMode(GL_POLYGON);
        drawCircle(width() / 2 - 10, backgroundGeometry);

        auto backgroundMaterial = new QSGFlatColorMaterial();
        backgroundMaterial->setColor(_backgroundColor);

        auto backgroundNode = new QSGGeometryNode();
        backgroundNode->setGeometry(backgroundGeometry);
        backgroundNode->setMaterial(backgroundMaterial);

        return borderNonActiveNode;
    } else {
        static const double radius = width() / 2;
        auto countPoints           = static_cast<int>(angle());

        auto geometry = _borderActiveNode->geometry();
        geometry->allocate(countPoints + 1, 0);
        _borderActiveNode->markDirty(QSGNode::DirtyGeometry);
        geometry->vertexDataAsPoint2D()[0].set(radius, radius);

        for (int i = 1; i < countPoints + 1; ++i) {
            double rad = (i - 90) * Deg2Rad;
            geometry->vertexDataAsPoint2D()[i].set(
                std::cos(rad) * radius + width() / 2,
                std::sin(rad) * radius + height() / 2);
        }
    }
    return oldNode;
}
