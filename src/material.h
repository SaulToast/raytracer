#pragma once

#include "color.h"
#include "vec3.h"

#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

struct material
{
    double kd, ks, ka;
    vec3 od, os;
    double shininess;
};

struct light
{
    vec3 light_direction;
    color light_color;
    color ambient_light;
};

inline material parse_material(const json& j)
{
    material m;
    
    m.ka = j["ka"];
    m.ks = j["ks"];
    m.kd = j["kd"];
    m.od = color(j["od"][0], j["od"][1], j["od"][2]);
    m.os = color(j["os"][0], j["os"][1], j["os"][2]);
    m.shininess = j["kgls"];
    
    return m;
}

inline light parse_light(const json& j)
{
    light l;
    
    
    l.light_direction = vec3(j["direction_to_light"][0],
                             j["direction_to_light"][1],
                             j["direction_to_light"][2]);
    
    l.light_color = color(j["light_color"][0],
                          j["light_color"][1],
                          j["light_color"][2]);
    
    l.ambient_light = color(j["ambient_light"][0],
                            j["ambient_light"][1],
                            j["ambient_light"][2]);
    
    return l;
}