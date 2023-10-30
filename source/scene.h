#pragma once

#include "object.h"
#include <fstream>
#include <filesystem>
#include <format>

const int MAX_OBJECTS = 30;
const int ELMENTS_IN_1OBJ = 23;

class Scene {
public:
    Object objects[MAX_OBJECTS];
    float sceneData1D[MAX_OBJECTS * ELMENTS_IN_1OBJ];

    Scene() {
        convertTo1D();
    }

    void convertTo1D();
    void updateObjectsFrom1D();

    void rearrangeObjects();

    void saveInFile(const char* fileName);

    void readFromAfile(const char* fileName);
};