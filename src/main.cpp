#include "general.h"

#include "world.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include <vector>

int main() {

    // World
    hittable_list objects;
    std::vector<light> lights;
    
    
    material mat;
    mat.ka = 0.1;
    mat.kd = 0.7;
    mat.ks = 0.2;
    mat.od = color(1.0, 0.0, 1.0);
    mat.os = color(1.0, 1.0, 1.0);
    mat.shininess = 16.0;
    
    world world1;
    world1.objects = objects;
    world1.lights.push_back(light{vec3(0,1.0,0), color(1.0,1.0,1.0), color(0,0,0)});
    
    world1.objects.add(make_shared<sphere>(point3(0,0,-1), 0.5, mat));
    world1.objects.add(make_shared<sphere>(point3(0, -100.5, -1), 100, mat));

    // Camera
    camera cam;
    
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.background = color(0.70, 0.80, 1.00);
    
    cam.render(world1);

}
