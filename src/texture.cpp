#include "texture.h"
#include "bary.h"
#include <QtDebug>

TextureShader::TextureShader(const QImage& img){
    texture = img;
}

Vec3f TextureShader::shade(const Vertex &a, const Vertex &b, const Vertex &c, const Vec3f &bary){
    float pixel_z = interPolateCord(a.invW, b.invW, c.invW, bary);
    float invZ = 1 / pixel_z;

    auto face_color = baryCentricInterpolation(a.color, b.color, c.color, bary);

    float pixel_u = interPolateCord(a.u , b.u, c.u, bary) * invZ;
    float pixel_v = interPolateCord(a.v, b.v, c.v, bary) * invZ;

    int x = std::floor(pixel_u * (texture.width()) - 1);
    int y = std::floor(pixel_v * (texture.height() - 1));

    if (x < 0) x = 0;
    if (y < 0) y = 0;


    auto color = texture.pixelColor(x, y);
    auto red = (float)color.red();
    auto green = (float)color.green();
    auto blue = (float)color.blue();
    Vec3f pixel_color = Vec3f{red / 255.f,
            green/ 255.f ,
            blue /255.f
};

    auto t = pixel_color.hadamard(face_color).saturate();

    return t;
}
