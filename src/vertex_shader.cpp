#include "vertex_shader.h"
#include <iostream>

Vertex VertexShader::shade(const Vertex &a, const Mat4x4f& rotMatrix, const Mat4x4f& objToWorld, const Camera& cam)
{
    Vec4f res(a.pos);
    res = res * objToWorld;

    Vertex output = a;
    output.pos = Vec3f(res.x, res.y, res.z);

    auto normal = Vec4f(a.normal) * rotMatrix;
    output.normal = Vec3f(normal.x, normal.y, normal.z).normalize();

    auto diffuse = light_color * std::max(0.f, Vec3f::dot(output.normal.normalize(), -cam.direction.normalize())) * intensity;
    auto c = (diffuse + ambient).saturate();
    output.color = output.color.hadamard(c).saturate();
    return output;
}
