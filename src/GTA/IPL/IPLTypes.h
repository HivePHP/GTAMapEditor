#pragma once
#include <string>

struct IPLPlacement {
    int id;
    std::string modelName;
    int interior;
    float posX, posY, posZ;
    float rotX, rotY, rotZ, rotW;
    int lod; // Level of Detail (обычно -1, если нет)
};