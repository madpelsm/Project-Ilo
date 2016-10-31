#include "Player.h"

Player::Player() {

}
Player::~Player() {

}

void Player::setTransform(float x, float y, float angle) {

}
glm::vec3 Player::getPosition() {
    return glm::vec3(mX, mY, 0);
}

void Player::render() {
    glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLE_STRIP, mIndices.size(), GL_UNSIGNED_SHORT, 0);
}

void Player::init() {
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    //create bind and upload
    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertices.size(), &mVertices[0], GL_DYNAMIC_DRAW);

    //location
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) sizeof(glm::vec3));
    ////normal
    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(2* sizeof(glm::vec3)));
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*mIndices.size(), &mIndices[0], GL_STATIC_DRAW);


}