#include "general.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int main() {

    // World
    hittable_list world;
    
    material mat;
    
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5, mat));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, mat));

    // Camera
    camera cam;
    
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.background = color(0.70, 0.80, 1.00);
    
    cam.render(world);

}
