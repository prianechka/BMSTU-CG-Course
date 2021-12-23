#ifndef CAMERA_H
#define CAMERA_H
#include "vec3.h"
#include "mat.h"
#include <QDebug>

const float eps_cam = 1e-5;

class Camera
{
    public:
    Camera(float width, float height, Vec3f pos = {0, 0, -7}, Vec3f up = {0, 1, 0}, Vec3f direction = {0, 0, 1},
           float fov = 45, float zn_ = 0.1f, float zf_ = 1000.f): position{pos}, up{up}, direction{direction},
                                                                  fov{fov}, zn{zn_}, zf{zf_}
    {
        aspect_ratio = width / height;
        projectionMatrix = Mat4x4f::ProjectionFOV(fov, aspect_ratio, zn, zf);
    }

    Mat4x4f viewMatrix() const
    {
        return Mat4x4f::LookAtLH(position, position + direction, up);
    }

    void shiftX(float dist)
    {
        auto normal = Vec3f::cross(direction, up).normalize();
        normal *= dist;
        position += normal;
    }

    void shiftZ(float dist)
    {
        position += direction.normalize() * dist;
    }

    void shiftY(float dist)
    {
        position.y += dist;
    }


    void rotateX(float angle)
    {
        auto normal = Vec3f::cross(direction, up);
        direction = rotateQautr(normal, direction, angle);
        up = rotateQautr(normal, up, angle);
        x_rot_angle += angle;
    }

    void rotateY(float angle)
    {
        // поворот относительно вектора up
        direction = direction * Mat3x3f::RotationY(angle);
        up = up * Mat3x3f::RotationY(angle);
    }


private:

    Vec3f rotateQautr(const Vec3f& axis, const Vec3f& v, const float& angle){
        auto u = Vec4f(v, 0) ;
        auto teta = angle * M_PI / 180.f;
        auto q = Vec4f(axis.normalize(), cos(teta * 0.5));
        auto sin_teta = sin(teta * 0.5);
        q.x *= sin_teta;
        q.y *= sin_teta;
        q.z *= sin_teta;

        auto v1 = Vec3f(q.x, q.y, q.z);
        auto v2 = Vec3f(u.x, u.y, u.z);
        auto qu = v2 * q.w + Vec3f::cross(v1, v2);

        v1 = Vec3f{qu.x, qu.y, qu.z};
        v2 = Vec3f{-q.x, -q.y, -q.z};

        return v1 * q.w + Vec3f::cross(v1, v2);
    }

public:
    Vec3f position;
    Vec3f up;
    Vec3f direction;
    float fov;
    float zn;
    float zf;
    float aspect_ratio;
    Mat4x4f projectionMatrix;
private:
    float x_rot_angle = 0;
};

#endif // CAMERA_H
