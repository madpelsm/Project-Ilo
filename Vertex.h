#pragma once
#include <glm.hpp>
struct Vertex {
    glm::vec3 Pos;
    glm::vec3 Col;
    Vertex(glm::vec3 Pos, glm::vec3 Col) {
        this->Pos = Pos;
        this->Col = Col;
    }
};
