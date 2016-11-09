#include "Camera.h"
#include <iostream>

Camera::Camera() {
}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    mUp = up;
    mTarget = target;
    mPosition = position;
    glm::vec3 LookDirection = mTarget - mPosition;
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

void Camera::uploadCameraInfo(int shaderProgramID) {
    int camLocation = glGetUniformLocation(shaderProgramID, "cam");
    glProgramUniformMatrix4fv(shaderProgramID, camLocation, 1, GL_FALSE, glm::value_ptr(mCamera));
}

void Camera::setViewPos(int shaderProgramID) {
    glUniform3f(glGetUniformLocation(shaderProgramID, "eyePos"), mPosition.x, mPosition.y, mPosition.z);
}
void Camera::update() {
    mCamera = glm::lookAt(mPosition, mTarget, mUp);
}

void Camera::rotateJaw(float angle) {
    glm::vec4 LookDirection = glm::vec4(mTarget - mPosition, 0.0f);
    mLookDir = normalize(glm::vec3(LookDirection));
    //rotate around y axis
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0));
    glm::vec3 rotated = glm::vec3(rotationMat*LookDirection); //discard the w comp;
    //mPosition stays the same
    mTarget = mPosition + rotated;

}

void Camera::rotatePitch(float angle) {
    glm::vec4 LookDirection = glm::vec4(mTarget - mPosition, 0.0f);
    mLookDir = normalize(glm::vec3(LookDirection));
    //rotate around y axis
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1), angle, glm::vec3(-mLookDir.z, 0, mLookDir.x));
    glm::vec3 rotated = glm::vec3(rotationMat*LookDirection); //discard the w comp;
                                                              //mPosition stays the same
    mTarget = mPosition + rotated;

}
void Camera::moveWithDir(float spd) {
    mNormalizedXZLookDir = glm::normalize(glm::vec3(mLookDir.x, 0, mLookDir.z));

    mPosition += spd*mNormalizedXZLookDir;
    mTarget += spd*mNormalizedXZLookDir;
}
void Camera::movePerpendicularOnDir(float spd) {
    mNormalizedXZLookDir = glm::normalize(glm::vec3(mLookDir.x, 0, mLookDir.z));

    //standard is move right
    mPosition += spd*glm::vec3(-mNormalizedXZLookDir.z, 0, mNormalizedXZLookDir.x);
    mTarget += spd*glm::vec3(-mNormalizedXZLookDir.z, 0, mNormalizedXZLookDir.x);
}
