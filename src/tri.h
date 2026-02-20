#pragma once

#include "hittable.h"
#include "material.h"
#include "vec3.h"
#include <cmath>
#include <memory>
#include <vector>

class tri : public hittable
{
    public:
    
        tri(const point3& v0, const point3& v1, const point3& v2, material mat) : v0(v0), v1(v1), v2(v2), mat(mat) 
        {
            vec3 u = v1 - v0;
            vec3 v = v2 - v0;
            normal = normalize(cross(u, v));
            D = dot(normal, v0);
        }
    
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            // find intersection with plane
            auto denom = dot(normal, r.direction());
            
            if (std::fabs(denom) < 1e-8)
                return false;
            
            auto t = (D - dot(normal, r.origin())) / denom;
            
            if (!ray_t.contains(t))
                return false;
            
            auto intersection = r.at(t);
            // test point against each edge
            point3 v[3] = {v0, v1, v2};
            
            vec3 n = normal;
            if (dot(r.direction(), normal) > 0)
                n = -normal;
            
            for (int i = 0; i < 3; ++i)
            {
                vec3 edge = v[(i + 1) % 3] - v[i];
                vec3 c = intersection - v[i];
                
                if (dot(n, cross(edge, c)) < 0)
                {
                    return false;
                }
            }
            
            rec.t = t;
            rec.p = r.at(t);
            rec.set_face_normal(r, normal);
            rec.material = &mat;
            
            return true;
        }
    
    private:
        point3 v0, v1, v2;
        material mat;
        vec3 normal;
        double D;
};

inline tri parse_tri(const json& j)
{
    std::cerr << "Parsing tri\n";
    auto v0 = point3(j["v0"][0], j["v0"][1], j["v0"][2]);
    auto v1 = point3(j["v1"][0], j["v1"][1], j["v1"][2]);
    auto v2 = point3(j["v2"][0], j["v2"][1], j["v2"][2]);
    auto mat = parse_material(j["material"]);
    std::cerr << "done parsing tri\n";
    
    return tri(v0, v1, v2, mat);
}