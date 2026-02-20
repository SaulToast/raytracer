#pragma once

#include "hittable.h"
#include "material.h"
#include "vec3.h"

class sphere : public hittable
{
    public:
        sphere(const point3& center, double radius, material mat) : center(center), radius(std::fmax(0,radius)), mat(mat) {}
        
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override
        {
            vec3 oc = center - r.origin();
            auto a = r.direction().length_squared(); // equal to r.direction dot itself
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;
            auto discriminant = h*h - a*c;
            
            if (discriminant < 0)
                return false;
            
            auto sqrtd = std::sqrt(discriminant);
            
            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root))
            {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }
            
            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.material = &mat;
            
            return true;
            
        }
        
    private:
        point3 center;
        double radius;
        material mat;
};

inline sphere parse_sphere(const json& j)
{
    auto center = point3(j["center"][0], j["center"][1], j["center"][2]);
    auto radius = j["radius"];
    auto mat = parse_material(j["material"]);
    std::clog << "Done parsing sphere\n";
    
    return sphere(center, radius, mat);
}