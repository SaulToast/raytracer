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
    
    if (argc < 2)
    {
        std::cerr << "Usage: raytracer <scenes/scene.json>\n";
        return 1;
    }
    
    std::string scene_filename = argv[1];
    std::clog << "Loading scene: " << scene_filename << '\n';
    
    std::ifstream scene_file(scene_filename);
    json data = json::parse(scene_file);

    // World
    hittable_list objects;
    std::vector<light> lights;
    light l = parse_light(data["light"]);
    
    world world1;
    world1.objects = parse_objects(data["objects"]);
    world1.lights.push_back(l);

    // Camera
    camera cam = parse_camera(data["camera"]);
    
    cam.aspect_ratio = 1.0;
    cam.image_width = 500;
    cam.samples_per_pixel = 100;
    
    cam.render(world1);

}
