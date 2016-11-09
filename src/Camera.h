#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera {
  public:
    glm::mat4 mCamera;
    glm::vec3 mUp;
    glm::vec3 mTarget;
    glm::vec3 mPosition, mLookDir, mNormalizedXZLookDir;
    Camera();
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up);
    ~Camera();

    void move(glm::vec3 position, glm::vec3 target);
    void move(glm::vec3 target);
    void uploadCameraInfo(int shaderProgramID);
    void update();
    void setViewPos(int shaderProgramID);
    void rotateJaw(float angle);
    void rotatePitch(float angle);
    void moveWithDir(float spd);
    void movePerpendicularOnDir(float spd);
};