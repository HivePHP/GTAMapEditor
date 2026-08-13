#pragma once
#include <string>

struct IDEObject {
    int id;
    std::string modelName;
    std::string txdName;

    // В будущем сюда можно добавить дистанцию прорисовки (draw distance)
    // и флаги объектов, но пока нам нужны только связи с моделями.
};