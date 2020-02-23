//
// Created by rick on 2020/2/23.
//

#pragma once
#include <QtCore/QtCore>
#include <QtGui/QtGui>

enum class CameraMovement { Forward, Backward, Left, Right };
const float kYaw = -90.0f;
const float kPitch = 0.0f;
const float kSpeed = 2.5f;
const float kSensitivity = 0.1f;
const float kZoom = 45.0f;

class Camera {
  public:
    Camera(QVector3D position = QVector3D{0.0f,0.0f,0.0f},
        QVector3D up = QVector3D{0.0f,1.0f,0.0f},
        float yaw = kYaw,
        float pitch = kPitch,
        ):front_{0.0f,0.0f,-1.0f},movementSpeed_{kSpeed},mouseSensitivity_{kSensitivity},zoom_{kZoom} {
        position_ = position;
    }

  private:
    QVector3D position_;
    QVector3D front_;
    QVector3D up_;
    QVector3D right_;
    QVector3D worldUp_;

    // Euler Angles
    float yaw_;
    float pitch_;

    // camera options
    float movementSpeed_;
    float mouseSensitivity_;
    float zoom_;
};