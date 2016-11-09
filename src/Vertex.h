#pragma once
#include <glm/glm.hpp>
struct Vertex {
    glm::vec3 Pos;
    glm::vec3 Col;
    Vertex(glm::vec3 Pos, glm::vec3 Col) {
        this->Pos = Pos;
        this->Col = Col;
    }
};

struct Vertex2 {
    glm::vec3 Pos;
    glm::vec3 Col;
    glm::vec3 Norm;
    glm::vec3 Material; //shininess, specularStrength, ambient
    Vertex2(Vertex v, glm::vec3 Norm, glm::vec3 Material) {
        this->Pos = v.Pos;
        this->Col = v.Col;
        this->Norm = Norm;
        this->Material = Material;
    }
};