#pragma once

#include "general.h"
#include "hittable.h"
#include "material.h"
#include "vec3.h"
#include "world.h"
#include <algorithm>
#include <cmath>

class camera
{
    public:
        double aspect_ratio = 1;
        int image_width = 100;
        int samples_per_pixel = 10;
        int max_depth = 10;
        color background;
        point3 look_from;
        point3 look_at;
        vec3 look_up;
        double fov;

        
        void render(const world& world)
        {
            initialize();
            
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        
            for (int j = 0; j < image_height; j++)
            {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++)
                {
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++)
                    {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
        
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
        
            std::clog << "\rDone.                      \n";
        }
        
    private:
        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        
        void initialize()
        {
            
            center = look_from;
            auto w = normalize(look_from - look_at);
            auto u = normalize(cross(look_up, w));
            auto v = cross(w, u);
            
            auto theta = degrees_to_rads(fov);
            auto h = tan(theta/2);
            
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
            
            pixel_samples_scale = 1.0 / samples_per_pixel;
            
            auto viewport_height = 2.0 * h;
            auto viewport_width = viewport_height * (double(image_width)/image_height);
            
            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v;
        
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;
        
            auto viewport_top_left = center - w - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_top_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }
        
        color ray_color(const ray& r, int depth, const world& world) const
        {
            if (depth <= 0)
            {
                return color(0,0,0);
            }
            
            hit_record rec;
            
            if (!world.objects.hit(r, interval(0, infinity), rec))
            {
                return background;            
            }
            
            auto a = world.lights.at(0).ambient_light;
            color final_color = rec.material->ka * rec.material->od * a;
            
            auto hit_point = r.at(rec.t);
            auto N = rec.normal;
            
            for (const auto& light : world.lights)
            {
                const double offset = 1e-4;
                point3 shadow_origin = hit_point + offset * N;
                auto L = normalize(light.light_direction);
                ray shadow_ray(shadow_origin, L);
                hit_record shadow_rec;
                
                if (!world.objects.hit(shadow_ray, interval(0, infinity), shadow_rec))
                {
                    auto diffuse = rec.material->kd * rec.material->od * light.light_color * fmax(dot(N, L), 0);
                    auto reflected = normalize(2 * dot(N, L) * N - L);
                    auto direction_to_cam = normalize(-r.direction());
                    auto specular = rec.material->ks * rec.material->os * pow(fmax(dot(reflected, direction_to_cam), 0), rec.material->shininess);
                    
                    final_color += diffuse + specular;
                }
                
                if (rec.material->refl > 0)
                {
                    point3 reflection_ray_origin = hit_point + offset * N;
                    vec3 reflection_direction = normalize(r.direction() - 2 * dot(r.direction(), N) * N);
                    ray reflected_ray(reflection_ray_origin, reflection_direction);
                    final_color = (1-rec.material->refl) * final_color + rec.material->refl * ray_color(reflected_ray, depth - 1, world);
                }
                

            }
            return final_color;
            
        }
        
        ray get_ray(int i, int j)
        {
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc 
                        + ((i + offset.x()) * pixel_delta_u) 
                        + ((j + offset.y()) * pixel_delta_v);
            
            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;
            
            return ray(ray_origin, ray_direction);
        }
        
        vec3 sample_square() const
        {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }
};

inline camera parse_camera(const json& j)
{
    camera cam;
    cam.look_at = point3(j["look_at"][0], j["look_at"][1], j["look_at"][2]);
    
    cam.background = color(j["background_color"][0],
                           j["background_color"][1], 
                           j["background_color"][2]);
    
    cam.look_from = point3(j["look_from"][0], j["look_from"][1], j["look_from"][2]);
    cam.look_up = point3(j["look_up"][0], j["look_up"][1], j["look_up"][2]);
    cam.fov = j["field_of_view"];
    
    return cam;
    
}