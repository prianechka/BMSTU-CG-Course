#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "vec3.h"

class Ray
{
    public:

      Ray(const Vec3f& origin_, const Vec3f& direction_)
        : origin(origin_)
        , direction(direction_.normalize())
      {
          invdirection = {1 / direction.x, 1 / direction.y, 1 /direction.z};
          sign[0] = (invdirection.x < 0);
          sign[1] = (invdirection.y < 0);
          sign[2] = (invdirection.z < 0);
      }
      Ray(const Ray& other)
        : origin(other.origin)
        , direction(other.direction.normalize())
      {}

    public:
      Vec3f origin;
      Vec3f direction, invdirection;
      int sign[3];
};


class Primitive {
public:
  virtual ~Primitive();

  virtual bool intersect(const Ray& ray) const
  {
    return false;
  }
};


class BoundingBox: public Primitive{
public:
    BoundingBox() = default;
    BoundingBox(const Vec3f& min_, const Vec3f& max_){
        bounds[0] = min_;
        bounds[1] = max_;
    }
    virtual ~BoundingBox() override;
    virtual bool intersect(const Ray &ray) const override;
private:
    Vec3f min;
    Vec3f max;
    Vec3f bounds[2];
};

#endif // PRIMITIVE_H
