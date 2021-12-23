#include "raythread.h"
#include "scene_manager.h"
#include <algorithm>

const float eps_float = 1e-5;
float power_ref = 1.f;

bool checkIntersection(const float& t, const float& t_min, const float& t_max, const float& closest_t){
    return t > t_min && t < t_max && t < closest_t;
}


Vec3f reflect(const Vec3f &L, const Vec3f &N) {
    return (L - (N * 2.f * Vec3f::dot(N, L))).normalize();
}

Vec3f refract(const Vec3f &I, const Vec3f &N, float eta_t, float eta_i=1.f) { // Snell's law
    float cosi = - std::max(-1.f, std::min(1.f, Vec3f::dot(I, N)));
    if (cosi < 0)
        return refract(I, -N, eta_i, eta_t); // if the ray comes from the inside the object, swap the air and the media
    float eta = eta_i / eta_t;
    float k = 1 - eta*eta*(1 - cosi*cosi);
    return k<0 ? Vec3f(0,0,0) : I*eta + N*(eta*cosi - sqrtf(k));
}

bool RayThread::sceneIntersect(const Ray &ray, InterSectionData &data, float t_max){
    float closeset_t = std::numeric_limits<float>::max();
    bool intersected = false;
    InterSectionData d;
    for (auto& model: models){
        if (!model->isObject()) continue;
        if (model->intersect(ray, d) && d.t < closeset_t){
            closeset_t = d.t;
            intersected = true;
            data = d;
            data.model = *model;
        }
    }

    return intersected;
}


Vec3f RayThread::cast_ray(const Ray &ray, int depth)
{

    InterSectionData data;
    if (depth > 4 || !sceneIntersect(ray, data))
        return Vec3f{0.f, 0, 0};

    float di = 1 - data.model.specular;

    float distance = 0.f;

    float occlusion = 1e-4f;

    Vec3f ambient, diffuse = {0.f, 0.f, 0.f}, spec = {0.f, 0.f, 0.f}, lightDir = {0.f, 0.f, 0.f},
            reflect_color = {0.f, 0.f, 0.f}, refract_color = {0.f, 0.f, 0.f};

    if (fabs(data.model.refractive) > 1e-5)
    {
        Vec3f refract_dir = refract(ray.direction, data.normal, power_ref, data.model.refractive).normalize();
        Vec3f refract_orig = Vec3f::dot(refract_dir, data.normal) < 0 ? data.point - data.normal * 1e-3f : data.point + data.normal * 1e3f;
        refract_color = cast_ray(Ray(refract_orig, refract_dir), depth + 1);
    }

    if (fabs(data.model.reflective) > 1e-5)
    {
        Vec3f reflect_dir = reflect(ray.direction, data.normal).normalize();
        Vec3f reflect_orig = Vec3f::dot(reflect_dir, data.normal) < 0 ? data.point - data.normal * 1e-3f : data.point + data.normal * 1e-3f;
        reflect_color = cast_ray(Ray(reflect_orig, reflect_dir), depth + 1);
    }

    for (auto &model: models)
    {
        if (model->isObject()) continue;
        Light* light = dynamic_cast<Light*>(model);
        if (light->t == Light::light_type::ambient)
            ambient = light->color_intensity;
        else
        {
            if (light->t == Light::light_type::point)
            {
                lightDir = (light->position - data.point);
                distance = lightDir.len();
                lightDir = lightDir.normalize();
            } else{
                lightDir = light->getDirection();
                distance = std::numeric_limits<float>::infinity();
            }

            auto tDot = Vec3f::dot(lightDir, data.normal);

            Vec3f shadow_orig = tDot < 0 ? data.point - data.normal*occlusion : data.point + data.normal*occlusion; // checking if the point lies in the shadow of the lights[i]
            InterSectionData tmpData;
            if (sceneIntersect(Ray(shadow_orig, lightDir), tmpData))
                if ((tmpData.point - shadow_orig).len() < distance)
                    continue;

            diffuse += (light->color_intensity * std::max(0.f, Vec3f::dot(data.normal, lightDir)) * di);
            if (fabs(data.model.specular) < 1e-5) continue;
            auto r = reflect(lightDir, data.normal);
            auto r_dot = Vec3f::dot(r, ray.direction);
            auto power = powf(std::max(0.f, r_dot), data.model.n);
            spec += light->color_intensity * power * data.model.specular;
        }

    }

    return data.color.hadamard(ambient +
                               diffuse +
                               spec +
                               reflect_color * data.model.reflective +
                               refract_color * data.model.refractive).saturate();
}

Vec4f toWorld(int x, int y, const Mat4x4f& inverse, int width, int height){
    float ndc_x = (float)(x << 1) / (float)width - 1.f;
    float ndc_y = 1.f - (float)(y << 1) / (float)height;
    Vec4f res(ndc_x, ndc_y, 1);
    return res * inverse;
}

std::vector<RayBound> subBlock(const RayBound& bound, int depth){
    if (depth == 0)
    {
        return {RayBound{.xs = bound.xs, .xe = bound.xe, .ys = bound.ys, .ye = bound.ye}};
    }
    std::vector<RayBound> output;
    int x_q = (bound.xe - bound.xs) % 2 == 0;
    int y_q = (bound.ye - bound.ys) % 2 == 0;
    int x_step = bound.xe / 2;
    int y_step = bound.ye / 2;
    for (int curr = x_step - x_q, prev = bound.xs; curr <= bound.xe;)
    {
        for (int curr_y = y_step - y_q, prev_y = bound.ys; curr_y <= bound.ye;)
        {
            auto v = subBlock(RayBound{.xs = prev, .xe = curr, .ys = prev_y, .ye = curr_y}, depth - 1);
            output.insert(output.end(), v.begin(), v.end());
            prev_y = curr_y + 1;
            curr_y += y_step + !y_q;
        }
        prev = curr + 1;
        curr += x_step + !x_q;
    }
    return output;
}

std::vector<RayBound> split(int width, int height)
{
    int cnt = 8;
    int step = height / cnt;
    int start = 0;
    std::vector<RayBound> output;
    for (int i = 0; i < cnt; ++i) {
        output.push_back(RayBound{.xs = 0, .xe = width - 1, .ys = start, .ye = (start + step - 1) % height});
        start += step;
    }
    return output;
}

ThreadVector* SceneManager::trace()
{
    img.fill(Qt::black);
    power_ref = this->k_n;
    auto v = split(width, height);
    auto cam = camers[curr_camera];
    auto origin = cam.position;
    auto mat = cam.viewMatrix() * cam.projectionMatrix;
    auto inverse = Mat4x4f::Inverse(mat);

    for (auto& model: models)
        if (model->isObject())
            model->genBox();

    for (auto& bound: v)
    {
        auto th = new RayThread(&camers[curr_camera], img,  models, inverse, bound, width, height);
        threads.push_back(th);
    }

    return &threads;

}
