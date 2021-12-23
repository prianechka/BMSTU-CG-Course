#ifndef GEOMETRY_SHADER_H
#define GEOMETRY_SHADER_H
#include "shaders.h"

const float eps =  1e-7f;
class GeometryShader: public GeometryShaderInterface{
public:
    Vertex shade(const Vertex &a,
                 const Mat4x4f& projection, const Mat4x4f& camView) override;
    ~GeometryShader() override{}
};
#endif // GEOMETRY_SHADER_H
