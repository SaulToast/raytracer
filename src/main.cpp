#include "material.h"
#include "world.h"
#include "camera.h"
#include "hittable_list.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "json.hpp"

using json = nlohmann::json;


int main(int argc, char* argv[]) {
    
    if (argc < 3)
    {
        std::cerr << "Usage: raytracer <scenes/scene.json> <output_filename>\n";
        return 1;
    }
    
    std::string scene_filename = argv[1];
    std::clog << "Loading scene: " << scene_filename << '\n';
    
    std::ifstream scene_file(scene_filename);
    json data = json::parse(scene_file);

    // World
    hittable_list objects;
    std::vector<light> lights;
    
    world world1;
    std::clog << "parsing objects\n";
    world1.objects = parse_objects(data["objects"]);
    
    std::clog << "parsing lights\n";
    world1.lights = parse_lights(data["lights"]);

    // Camera
    camera cam = parse_camera(data["camera"]);
    
    cam.aspect_ratio = 1.0;
    cam.image_width = 500;
    cam.samples_per_pixel = 100;
    cam.max_depth = 3; //increase for refelction
    cam.filename = argv[2];
    
    cam.render(world1);

}
