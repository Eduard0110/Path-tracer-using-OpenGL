#pragma once
#include <iostream>

class Object {
public:
    std::string name; // for the gui
    float pos[3] = { 0.0f, 0.0f, 0.0f };
    float rot[3] = { 0.0f, 0.0f, 0.0f };

    // object type
    float type = 0.0f; // 0 - nothing, 1 - sphere, 2 - cube, 3 - plane
    float radius = 1.0f;
    float cubeSize[3] = { 1.0f, 1.0f, 1.0f };

    // material properties
    float colour[3] = { 1.0f, 1.0f, 1.0f };
    float specularColour[3] = { 1.0f, 1.0f, 1.0f };
    float isLight = 0.0f;
    float powerOfLight = 1.0f;
    float reflectivity = 0.0f;
    float refractionIndex = -1.0f;
    float percentSpecular = 0.0f;
    float roughness = 0.0f;

    void set_default_settings(int);

    void set_default_settingsALL();
};