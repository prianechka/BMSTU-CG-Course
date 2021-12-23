#ifndef RAYTHREAD_H
#define RAYTHREAD_H
#include <QThread>
#include <QImage>
#include "light.h"

struct RayBound{
    int xs, xe;
    int ys, ye;
};

class RayThread: public QThread
{
    Q_OBJECT
public:
    RayThread(Camera* cam_, QImage& img_, std::vector<Model*>& models_,
              Mat4x4f& inverse_, RayBound& bound_, int width_, int height_):
        cam{cam_}, img{img_}, models{models_}, inverse{inverse_},
        bound{bound_}, width{width_}, height{height_}{}
protected:
    void run() override;

signals:
    void finished();

private:
    Vec3f toWorld(int x, int y);
    Vec3f toWorld(const Vec3f& u, const Vec3f& v, const Vec3f& w, int x, int y);
    Vec3f traceRay(const Vec3f& o, const Vec3f& d, float t_min, float t_max, int depth);
    Vec3f cast_ray(const Ray& ray, int depth = 0);
    Vec3f computeLightning(const Vec3f& p, const Vec3f& n, const Vec3f& direction, float specular,
                           int depth = 0);
    bool sceneIntersect(const Ray& ray, InterSectionData& data, float t_max = 0.f);

private:
    QImage &img;
    std::vector<Model*>& models;
    Mat4x4f inverse;
    RayBound bound;
    int width, height;
    Camera* cam;
};

#endif // RAYTHREAD_H
