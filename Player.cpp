#include "Player.h"

Player::Player() {
    mVertices.push_back(Vertex(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)));
    mVertices.push_back(Vertex(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)));
    mVertices.push_back(Vertex(glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1)));
    std::cout << "Standard player created" << std::endl;
    createIndices();
    createNormals();
}
Player::~Player() {

}

void Player::setTransform(float x, float y, float angle) {
    //set the transformation info, then create the transformationMatrix
    mX = x;
    mY = y;
    mRotAngle = angle;
    mTransformation = glm::translate(glm::mat4(1), glm::vec3(mX, mY, 0)) * glm::rotate(glm::mat4(1), mRotAngle, glm::vec3(0, 0, 1));

}

void Player::setShape(std::vector<Vertex> vertices) {
    //pass in a vector with vertex info (pos and color) do this with origin at the center of the object
    mVertices = vertices;
    //create a new vector with all the normal information
    createNormals();

}

void Player::createIndices() {
    for (unsigned int i = 0; i < mVertices.size(); i++) {
        mIndices.push_back(i);
    }
}

void Player::createNormals() {
    //Create normals here
    //take segments of 3 vertices per iteration, add the calculated normal to the normallist
    for (unsigned int i = 0; i < mVertices.size(); i+=3) {
        //on next loop if i was 0, i will start from 3. [3] [4] [5] ..
        Vertex tempVertex1 = mVertices[i];
        Vertex tempVertex2 = mVertices[i+1];
        Vertex tempVertex3 = mVertices[i+2];
        glm::vec3 a = tempVertex2.Pos - tempVertex1.Pos;
        glm::vec3 b = tempVertex3.Pos - tempVertex1.Pos;
        glm::vec3 normal = glm::normalize(glm::cross(a, b));
        mNormals.push_back(normal);
        mNormals.push_back(normal);
        mNormals.push_back(normal);
    }
    std::cout << "normals Created for player" << std::endl;
}

glm::vec3 Player::getPosition() {
    return glm::vec3(mX, mY, 0);
}

void Player::render() {

    glBindVertexArray(mVaoPlayer);
    glDrawElements(GL_TRIANGLE_STRIP, mIndices.size(), GL_UNSIGNED_SHORT, 0);
}

void Player::init() {
    glGenVertexArrays(1, &mVaoPlayer);
    glBindVertexArray(mVaoPlayer);

    //create bind and upload
    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*mVertices.size(), &mVertices[0], GL_DYNAMIC_DRAW);


    glGenBuffers(1, &mNormID);
    glBindBuffer(GL_ARRAY_BUFFER, mNormID);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

    //location
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) sizeof(glm::vec3));
    ////normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    //indices 
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*mIndices.size(), &mIndices[0], GL_STATIC_DRAW);

    std::cout << "initialised  player gl" << std::endl;

}