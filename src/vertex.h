#ifndef VERTEX_H
#define VERTEX_H

#include "vec3.h"
#include "vec4.h"
class Vertex{
public:
    Vertex(Vec3f pos_, Vec3f normal_, float u_, float v_, Vec3f color_ = {0.5, 0.5, 0.5}):
        pos{pos_}, normal{normal_}, color{color_}, u{u_}, v{v_}{}
public:
    Vec3f pos, normal, color;
    float u, v;
    float invW = 1.f;
};
#endif // VERTEX_H
