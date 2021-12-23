#include "color_shader.h"
#include "bary.h"

Vec3f ColorShader::shade(const Vertex &a, const Vertex &b, const Vertex &c, const Vec3f& bary)
{
    auto pixel_color = baryCentricInterpolation(a.color, b.color, c.color, bary);
    return pixel_color;
}
