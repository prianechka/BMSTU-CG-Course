#ifndef COLOR_SHADER_H
#define COLOR_SHADER_H
#include "shaders.h"
#include <stdint.h>

class ColorShader: public PixelShaderInterface{
public:
    Vec3f shade(const Vertex &a, const Vertex &b, const Vertex &c, const Vec3f& bary) override;

    ~ColorShader() override{}
};
#endif // COLOR_SHADER_H
