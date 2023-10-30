#include "object.h"

void Object::set_default_settings(int objType) {
    if (objType == 0) { // Rought material
        isLight = 0.0;
        refractionIndex = -1.0;
    }
    else if (objType == 1) { // Light source
        isLight = 1.0;
        refractionIndex = -1.0;
    }
    else if (objType == 2) { // Lens
        isLight = 0.0;
        if (refractionIndex == -1.0) refractionIndex = 1.0;
    }
}

void Object::set_default_settingsALL() {
    name.clear();
    // pos
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
    // rotation
    rot[0] = 0.0f;
    rot[1] = 0.0f;
    rot[2] = 0.0f;
    // colour
    colour[0] = 1.0f;
    colour[1] = 1.0f;
    colour[2] = 1.0f;
    // specular colour
    specularColour[0] = 1.0f;
    specularColour[1] = 1.0f;
    specularColour[2] = 1.0f;
    // cube size
    cubeSize[0] = 1.0f;
    cubeSize[1] = 1.0f;
    cubeSize[2] = 1.0f;
    // other
    type = 0.0;
    radius = 1.0;
    isLight = 0.0;
    powerOfLight = 1.0;
    reflectivity = 0.0;
    refractionIndex = -1.0;
    percentSpecular = 1.0;
    roughness = 0.0;
}