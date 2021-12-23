#ifndef BARY_H
#define BARY_H
#include "vec3.h"

float interPolateCord(float val1, float val2, float val3, const Vec3f& bary);

Vec3f baryCentricInterpolation(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& bary);

float calcBar(Vec3f a, Vec3f b, Vec3f p);

Vec3f toBarycentric(Vec3f a, Vec3f b, Vec3f c, Vec3f p);

#endif // BARY_H
