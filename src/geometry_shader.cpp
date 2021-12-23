#include "geometry_shader.h"

Vertex GeometryShader::shade(const Vertex &a, const Mat4x4f &projection, const Mat4x4f& camView)
{
    Vec4f res(a.pos);
    res = res * camView;
    res = res * projection;
    Vertex output = a;
    output.pos = {res.x, res.y, res.z};

    if (fabs(res.w) < eps)
        res.w = 1;
    output.invW = 1 / res.w;
    output.u *= output.invW;
    output.v *= output.invW;

    return output;
}
