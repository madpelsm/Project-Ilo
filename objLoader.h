#pragma once
#include "Vertex.h"
#include <iostream>
#include <glad\glad.h>
#include <vector>
#include "tiny_obj_loader.h"
class objectLoader {
    std::string filePath;
    std::vector<GLushort> mIndices;
    std::vector<Vertex2> mVertices;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    bool ret;
    std::string err;
public:
    objectLoader(std::string filePath);
    ~objectLoader();
    std::vector<Vertex2> getVertices();
    std::vector<GLushort> getIndices();
};