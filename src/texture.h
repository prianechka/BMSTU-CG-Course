#ifndef TEXTURE_H
#define TEXTURE_H

#include "shaders.h"
#include <QImage>
class TextureShader: public PixelShaderInterface
{
public:
    TextureShader(const QImage& img);
    virtual Vec3f shade(const Vertex &a, const Vertex &b, const Vertex &c, const Vec3f &bary) override;
    virtual ~TextureShader(){}
private:
    QImage texture;
};

#endif // TEXTURE_H
