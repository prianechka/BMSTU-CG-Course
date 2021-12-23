#ifndef LIGHT_H
#define LIGHT_H
#include "model.h"

const float ambInt = 0.3f;
const int light_n = 1;

class Light: public Model{
public:

    enum light_type
    {
        ambient = 0,
        point,
        directional,
        pointLightSource,
    };

    Light(light_type t_ = ambient, Vec3f color_intensity_ = {ambInt, ambInt, ambInt},
          Vec3f position_ = {0, 0, 0}, float lightning_power_ = 1,
          Vec3f direction_ = {0, 0, 1},
          const std::string& fileName = "", uint32_t uid_ = 0,
          const Vec3f& scale = {1.f, 1.f, 1.f}):
        t(t_), direction(direction_), color_intensity(color_intensity_),
        position(position_), lightning_power(lightning_power_),
        Model(fileName, uid_, light_n, scale, position_){}

    void setType(const light_type& light_t)
    {
        t = light_t;
    }

    void setLightColor(const Vec3f color_intensity_)
    {
        this->color_intensity = color_intensity_;
    }

    virtual void shiftX(float dist){
        position.x = dist;
        Model::shiftX(dist);
    }

    virtual void shiftY(float dist){
        position.y = dist;
        Model::shiftY(dist);
    }

    virtual void shiftZ(float dist){
        position.z = dist;
        Model::shiftZ(dist);
    }

    virtual void rotateX(float angle) override{
        Model::rotateX(-angle);
    }

    virtual void rotateY(float angle) override{
        Model::rotateY(-angle);
    }

    virtual void rotateZ(float angle) override{
        Model::rotateZ(-angle);
    }

    Vec3f getDirection()
    {
        Vec4f temp(direction);
        temp = temp * rotation_matrix;
        return Vec3f{temp.x, temp.y, temp.z}.normalize();
    }

    virtual bool isObject() override{return false;}

    virtual ~Light(){}

public:
    light_type t;
    Vec3f color_intensity;
    Vec3f position;
    float lightning_power;

private:
    Vec3f direction;
};
#endif // LIGHT_H
