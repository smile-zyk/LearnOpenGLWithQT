#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QtMath>

//Orbit Camera
class Camera
{
public:
    Camera();
    float aspect{1.0f};
    QVector3D position;
    void RotateAroundTarget(QVector2D motion);
    void MoveTarget(QVector2D motion);
    void CloseToTarget(float ratio);
    QMatrix4x4 GetViewMatrix();
    QMatrix4x4 GetProjectionMatrix();
private:
    float zNear;
    float zFar;
    float fov;
    QVector3D target;
};

#endif // CAMERA_H
