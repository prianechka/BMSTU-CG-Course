#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <stdint.h>
#include <string>
#include <functional>
#include <memory>
#include "vertex.h"
#include "mat.h"
#include "shaders.h"
#include "primitive.h"
#include <QImage>

using data_intersect = std::pair<float, Vec3f>;
const float max_angle = 360, rot_step_x = 15, rot_step_y = 15, rot_step_z = 15;

struct InterSectionData;

struct Face
{
    Vertex a, b, c;
    Vec3f normal;
};

class Model{

public:

    Model() = default;

    Model(const std::string& fileName, uint32_t uid_,  int n_ = 20, const Vec3f& scale = {1.f, 1.f, 1.f},
          const Vec3f& position = {0.f, 0.f, 0.f});

    virtual void rotateX(float angle){
        auto step = wrap_angle(angle_x, angle, rot_step_x);
        angle_x = angle;
        rotation_matrix = rotation_matrix * Mat4x4f::RotationX(step);
    }

    virtual void rotateY(float angle){
        auto step = wrap_angle(angle_y, angle, rot_step_y);
        angle_y = angle;
        rotation_matrix = rotation_matrix * Mat4x4f::RotationY(step);
    }

    virtual void rotateZ(float angle){
        auto step = wrap_angle(angle_z, angle, rot_step_z);
        angle_z = angle;
        rotation_matrix = rotation_matrix * Mat4x4f::RotationZ(step);
    }

    virtual void shiftX(float dist){
        shift_x = dist;
    }

    virtual void shiftY(float dist){
        shift_y = dist;
    }

    virtual void shiftZ(float dist){
        shift_z = dist;
    }

    void scaleX(float factor){
        scale_x = factor;
    }

    void scaleY(float factor){
        scale_y = factor;
    }

    void scaleZ(float factor){
        scale_z = factor;
    }

    Mat4x4f objToWorld() const
    {
        return Mat4x4f::Scaling(scale_x, scale_y, scale_z) *
               rotation_matrix *
               Mat4x4f::Translation(shift_x, shift_y, shift_z);
    }

    void setColor(const Vec3f& color){
        for (auto& f: faces){
            f.a.color = color;
            f.b.color = color;
            f.c.color = color;
        }
        this->color = color;
    }


    uint32_t getUid() const{
        return uid;
    }

    virtual bool isObject() {return true;}

    std::pair<data_intersect, data_intersect> interSect(const Vec3f& o, const Vec3f& d);
    bool intersect(const Ray& ray, InterSectionData& data);

    void genBox();

//    virtual ~Model(){}


private:
    float wrap_angle(float curr_angle, float next_angle, float step){
        if (next_angle < curr_angle)
            step *= -1;
        return step;
    }


    bool triangleIntersect(const Face& face, const Ray& ray,
                           const Mat4x4f& objToWorld, const Mat4x4f& rotMatrix,
                           InterSectionData& data);


public:
    std::vector<uint32_t> index_buffer;
    std::vector<Vertex> vertex_buffer;
    std::vector<Face> faces;
    Mat4x4f rotation_matrix = Mat4x4f::Identity();
    Mat4x4f scale_matrix;
    QImage texture;
    bool has_texture = false;

    float specular = 0.f;

    float reflective = 0.f;
    float refractive = 0.f;

    float transparency = 0.f;

    int n = 20;

    Vec3f color;
    BoundingBox box;

private:
    float angle_x = 0.f, angle_y = 0.f, angle_z = 0.f;
    float shift_x, shift_y, shift_z;
    float scale_x = 1.f, scale_y = 1.f, scale_z = 1.f;
    uint32_t uid;
};

struct InterSectionData
{
    Model model;
    float t;
    float coef_n;
    Vec3f point;
    Vec3f normal;
    Vec3f color;
};
#endif // MODEL_H
