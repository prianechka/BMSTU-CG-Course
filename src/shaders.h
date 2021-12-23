#ifndef SHADERS_H
#define SHADERS_H

#include "vertex.h"
#include "camera.h"

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high)
{
    if (val < low)
        return low;
    if (val > high)
        return high;
    return val;
}

inline float GammaCorrect(float value)
{
    if (value <= 0.0031308f)
    {
        return 12.92f * value;
    }
    return 1.055f * std::pow(value, (1.f / 2.4f)) - 0.055f;
}


class PixelShaderInterface{
public:
    virtual Vec3f shade(const Vertex& a, const Vertex& b, const Vertex&c, const Vec3f& bary) = 0;
    virtual ~PixelShaderInterface(){}
};

class VertexShaderInterface{
public:
    virtual Vertex shade(const Vertex &a,
                         const Mat4x4f& rotation,
                         const Mat4x4f& objToWorld,
                         const Camera& cam) = 0;
    virtual ~VertexShaderInterface(){}

};

class GeometryShaderInterface{
public:
    virtual Vertex shade(const Vertex &a,
                         const Mat4x4f& projectMatrix, const Mat4x4f& camView) = 0;
    virtual ~GeometryShaderInterface(){}
};
#endif // SHADERS_H
