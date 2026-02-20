#pragma once

#include "hittable.h"
#include "interval.h"
#include "sphere.h"
#include "tri.h"

#include <memory>
#include <stdexcept>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
    public:
        std::vector<shared_ptr<hittable>> objects;
        
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) {add(object); }
        
        void clear() { objects.clear(); }
        
        void add(shared_ptr<hittable> object)
        {
            objects.push_back(object);
        }
        
        bool hit(const ray &r, interval ray_t, hit_record &rec) const override
        {
            hit_record temp_rec;
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;
            
            for (const auto& object : objects)
            {
                if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
                {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            
            return hit_anything;
        }
};

inline std::shared_ptr<hittable> parse_object(const json& obj)
{
    const std::string type = obj.at("type");
    std::clog << "parsing object type: " << type << '\n';
    if (type == "sphere")
        return make_shared<sphere>(parse_sphere(obj));
    if (type == "triangle")
        return make_shared<tri>(parse_tri(obj));
    
    throw std::runtime_error("unknown object type: " + type);
}

inline hittable_list parse_objects(const json& j)
{
    hittable_list objects;
    
    for (const json& obj : j)
        objects.add(parse_object(obj));
    
    return objects;
}