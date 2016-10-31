#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,int shaderProgramID) {
    mUp = up;
    mTarget = target;
    mPosition = position;
    mShaderProgramID = shaderProgramID;
    update();
}

Camera::~Camera() {

}

void Camera::move(glm::vec3 position, glm::vec3 target) {
    mTarget = target;
    mPosition = position;
    update();
    
}

void Camera::move(glm::vec3 target) {
    mTarget = target;
    mPosition = glm::vec3(target.x, target.y, mPosition.z);
    update();
}

void Camera::uploadCameraInfo() {
    int camLocation = glGetUniformLocation(mShaderProgramID, "cam");
    glProgramUniformMatrix4fv(mShaderProgramID, camLocation, 1, GL_FALSE, glm::value_ptr(mCamera));
}
void Camera::update() {
    mCamera = glm::lookAt(mPosition, mTarget, mUp);
}