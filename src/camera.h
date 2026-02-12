#pragma once

#include "general.h"
#include "hittable.h"
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
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
            
            pixel_samples_scale = 1.0 / samples_per_pixel;
            
            auto focal_len = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height);
            
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);
        
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;
        
            auto viewport_top_left = center - vec3(0, 0, focal_len) - viewport_u/2 - viewport_v/2;
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
            
            
            light light1 = world.lights.at(0);
            auto ambient = rec.material->ka * rec.material->od * light1.ambient_light;
            auto diffuse = rec.material->kd * rec.material->od * light1.light_color * fmax(dot(rec.normal, light1.light_direction), 0);
            auto reflected = 2 * dot(rec.normal, light1.light_direction) * rec.normal - light1.light_direction;
            auto direction_to_cam = normalize(-r.direction());
            auto specular = rec.material->ks * rec.material->os * pow(fmax(dot(reflected, direction_to_cam), 0), rec.material->shininess);
            return ambient + diffuse + specular;
            
            
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