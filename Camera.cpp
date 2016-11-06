#include "Camera.h"
#include <iostream>

Camera::Camera() {
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,int shaderProgramID) {
    mUp = up;
    mTarget = target;
    mPosition = position;
    mShaderProgramID = shaderProgramID;
    glm::vec4 LookDirection = glm::vec4(mTarget - mPosition, 0.0f);
    mLookDir = normalize(LookDirection);
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
    glUniform3f(glGetUniformLocation(mShaderProgramID, "eyePos"), mPosition.x, mPosition.y, mPosition.z);
}
void Camera::update() {
    mCamera = glm::lookAt(mPosition, mTarget, mUp);
}

void Camera::rotate(float angle) {
    glm::vec4 LookDirection = glm::vec4(mTarget - mPosition,0.0f);
    mLookDir = normalize(LookDirection);
    //rotate around y axis
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));
    glm::vec3 rotated = glm::vec3(rotationMat*LookDirection); //discard the w comp;
    //mPosition stays the same
    mTarget = mPosition + rotated;

}
void Camera::moveWithDir(float spd) {
    mPosition += spd*glm::vec3(mLookDir.x,0, mLookDir.z);
    mTarget += spd*glm::vec3(mLookDir.x,0, mLookDir.z);
}
void Camera::movePerpendicularOnDir(float spd) {
    //standard is move right
    mPosition += spd*glm::vec3(-mLookDir.z,0, mLookDir.x);
    mTarget += spd*glm::vec3(-mLookDir.z,0, mLookDir.x);
}