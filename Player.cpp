#include "Player.h"

Player::Player() {/*
    mVertices.push_back(Vertex(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)));
    mVertices.push_back(Vertex(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)));
    mVertices.push_back(Vertex(glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));

    mVertices.push_back(Vertex(glm::vec3(1.5f, 0, 0), glm::vec3(1, 0, 0)));
    mVertices.push_back(Vertex(glm::vec3(2.5f, 0, 0), glm::vec3(0, 1, 0)));
    mVertices.push_back(Vertex(glm::vec3(2.0f, 1, 0), glm::vec3(0, 0, 1)));*/
    objectLoader objLoader("shapes/cornell_box.obj");
    mVertices2 = objLoader.getVertices();
    std::cout << "Standard player created" << std::endl;
    mIndices = objLoader.getIndices();

    //createIndices();
    //createNormals();
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
    for (unsigned int i = 0; i < mVertices2.size(); i++) {
        mIndices.push_back((GLushort)i);
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
        glm::vec3 a = tempVertex2.Pos - tempVertex1.Pos;//from vert 1 to vert 2
        glm::vec3 b = tempVertex3.Pos - tempVertex1.Pos;//from vert1 to vert 3
        glm::vec3 normal = glm::normalize(glm::cross(a, b));
        mVertices2.push_back(Vertex2(tempVertex1, normal));
        mVertices2.push_back(Vertex2(tempVertex2, normal));
        mVertices2.push_back(Vertex2(tempVertex3, normal));


    }
    std::cout << "normals Created for player" << std::endl;
}

glm::vec3 Player::getPosition() {
    return glm::vec3(mX, mY, 0);
}

void Player::render() {

    glBindVertexArray(mVaoPlayer);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_SHORT, 0);
}

void Player::init() {
    //vertexBuffer
    glGenBuffers(1, &mVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2)*mVertices2.size(), &mVertices2[0], GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &mVaoPlayer);
    glBindVertexArray(mVaoPlayer);
    //create bind and upload

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    //location
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), 0);
    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid *) sizeof(glm::vec3));
    //Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid *) (2*sizeof(glm::vec3)));

    //indices 
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
    std::cout << "drawing " << 3*mVertices2.size() << " vertices with " << mIndices.size() << " indices" << std::endl;
    std::cout << "initialised  player gl" << std::endl;

}