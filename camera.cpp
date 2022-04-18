#include "camera.h"

Camera::Camera()
{
    zNear = 0.1f;
    zFar = 100.0f;
    fov = 45.0f;
    position = {0.0f,0.0f,3.0f};
    target = {0.0f,0.0f,0.0f};
}

void Camera::RotateAroundTarget(QVector2D motion)
{
    QVector3D formTarget = position-target;
    float radius = formTarget.length();
    float theta = (float)atan2(formTarget.x(), formTarget.z());
    float phi = (float)acos(formTarget.y() / radius);
    float factor = acos(-1) * 2;
    QVector3D offset;

    theta -= motion.x() * factor;
    phi -= motion.y() * factor;
    if(phi < 0.1f) phi = 0.1f;
    if(phi > (float)acos(-1)-0.1f) phi = (float)acos(-1)-0.1f;

    offset.setX(radius * (float)sin(phi) * (float)sin(theta));
    offset.setY(radius * (float)cos(phi));
    offset.setZ(radius * (float)sin(phi) * (float)cos(theta));

    position = target + offset;
}

void Camera::MoveTarget(QVector2D motion)
{
    QVector3D fromPosition = target-position;
    QVector3D forward = fromPosition.normalized();
    QVector3D left = QVector3D::crossProduct({0.0f,1.0f,0.0f},forward);
    left.normalize();
    QVector3D up = QVector3D::crossProduct(forward,left);
    up.normalize();

    float distance = fromPosition.length();
    float factor = distance * (float)tan(qDegreesToRadians(fov/2)) * 2;
    QVector3D delta_x = factor * aspect * motion.x() * left;
    QVector3D delta_y = factor * motion.y() * up;
    target += (delta_x+delta_y);
    position += (delta_x+delta_y);
}

void Camera::CloseToTarget(float ratio)
{
    QVector3D formTarget = position-target;
    float radius = formTarget.length();
    float theta = (float)atan2(formTarget.x(), formTarget.z());
    float phi = (float)acos(formTarget.y() / radius);
    QVector3D offset;
    radius *= (float)pow(0.95, ratio);
    offset.setX(radius * (float)sin(phi) * (float)sin(theta));
    offset.setY(radius * (float)cos(phi));
    offset.setZ(radius * (float)sin(phi) * (float)cos(theta));
    position = target + offset;
}

QMatrix4x4 Camera::GetViewMatrix()
{
    QMatrix4x4 res;
    res.lookAt(position,target,{0.0f,1.0f,0.0f});
    return res;
}

QMatrix4x4 Camera::GetProjectionMatrix()
{
    QMatrix4x4 res;
    res.perspective(fov, aspect, zNear, zFar);
    return res;
}
