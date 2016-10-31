#pragma once

#include <vector>
#include "Vertex.h"
#include <glad\glad.h>
#include <glm.hpp>
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
class GameObject {
public:
    virtual void render()=0;
};