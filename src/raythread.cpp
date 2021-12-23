#include "raythread.h"

Vec3f RayThread::toWorld(int x, int y){
    float ndc_x = (float)(x << 1) / (float)width - 1.f;
    float ndc_y = 1.f - (float)(y << 1) / (float)height;
    Vec4f res(ndc_x, ndc_y, 1);
    return res * inverse;
}

Vec3f RayThread::toWorld(const Vec3f &u, const Vec3f &v, const Vec3f &w, int x, int y)
{
    return u * float(x) - v * float(y) + w;
}

void RayThread::run()
{
    auto u = Vec3f::cross(cam->up, cam->direction).normalize();
    auto v = cam->up.normalize();
    auto w = -cam->direction.normalize();

    auto w_ = u * float(-(width >> 1)) + v * float(height >> 1) - w * (float((height >> 1)) / tan(cam->fov / 2 * M_PI / 180));

    for (int x = bound.xs; x <= bound.xe; x++)
    {
        for (int y = bound.ys; y <= bound.ye; y++)
        {
            auto d = toWorld(u, v, w_, x, y).normalize();
            auto color = cast_ray(Ray(cam->position, d)) * 255.f;
            img.setPixelColor(x, y, QColor(color.x, color.y, color.z));
        }
    }

    emit finished();
}
