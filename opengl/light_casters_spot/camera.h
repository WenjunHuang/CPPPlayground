//
// Created by rick on 2021/4/20.
//

#pragma once
#include <QMatrix4x4>
#include <QVector3D>
#include <algorithm>
#include <numbers>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f / 100;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

inline double toRadian(double degree)
{
    auto n = std::numbers::pi_v<double>;
    return (degree * n) / 180;
}

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(QVector3D position = QVector3D { 0.0f, 0.0f, 0.0f },
        QVector3D up = QVector3D { 0.0f, 1.0f, 0.0f },
        float yaw = YAW, float pitch = PITCH)
        : Front { 0.0f, 0.0f, -1.0f }
        , MovementSpeed { SPEED }
        , MouseSensitivity { SENSITIVITY }
        , Zoom { ZOOM }
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    QMatrix4x4 getViewMatrix()
    {
        QMatrix4x4 matrix;
        matrix.lookAt(Position, Position + Front, Up);
        return matrix;
    }

    void processKeyboard(CameraMovement direction)
    {
        auto velocity = MovementSpeed;
        switch (direction) {
        case CameraMovement::FORWARD:
            Position += Front * velocity;
            break;
        case CameraMovement::BACKWARD:
            Position -= Front * velocity;
            break;
        case CameraMovement::LEFT:
            Position -= Right * velocity;
            break;
        case CameraMovement::RIGHT:
            Position += Right * velocity;
            break;
        }
    }

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;
        if (constrainPitch) {
            Pitch = std::clamp(Pitch, -89.0f, 89.0f);
        }
        updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
        Zoom -= yoffset;
        Zoom = std::clamp(Zoom, 1.0f, 45.0f);
    }

private:
    void updateCameraVectors()
    {
        QVector3D front;
        front.setX(cos(toRadian(Yaw)) * cos(toRadian(Pitch)));
        front.setY(sin(toRadian(Pitch)));
        front.setZ(sin(toRadian(Yaw)) * cos(toRadian(Pitch)));

        Front = front.normalized();
        Right = QVector3D::crossProduct(Front, WorldUp).normalized();
        Up = QVector3D::crossProduct(Right, Front).normalized();
    }
};