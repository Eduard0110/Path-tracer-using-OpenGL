#include "object.h"

void Object::set_col(float r, float g, float b) {
    colour[0] = r;
    colour[1] = g;
    colour[2] = b;
}

void Object::set_pos(float x, float y, float z) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void Object::set_rot(float x, float y, float z) {
    rot[0] = x;
    rot[1] = y;
    rot[2] = z;
}

void Object::set_cubeSize(float x, float y, float z) {
    cubeSize[0] = x;
    cubeSize[1] = y;
    cubeSize[2] = z;
}

void Object::set_default_settings(int objType) {
    if (objType == 0) {
        isLight = 0.0;
        refractionIndex = -1.0;
    }
    else if (objType == 1) {
        isLight = 1.0;
        refractionIndex = -1.0;
    }
    else if (objType == 2) {
        isLight = 0.0;
        if (refractionIndex == -1.0) refractionIndex = 1.0;
    }
}

void Object::set_default_settingsALL() {
    name.clear();
    set_pos(0.0f, 0.0f, 0.0f);
    set_rot(0.0f, 0.0f, 0.0f);
    set_col(1.0f, 1.0f, 1.0f);
    type = 0.0;
    radius = 1.0;
    set_cubeSize(1.0f, 1.0f, 1.0f);
    isLight = 0.0;
    powerOfLight = 1.0;
    reflectivity = 0.0;
    refractionIndex = -1.0;
    percentSpecular = 1.0;
    roughness = 0.0;
}