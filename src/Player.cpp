#include "Player.h"
#include <SDL.h>
Player::Player() {

}
Player::~Player() {
    cleanup();
}

void Player::loadGeometry(std::string filePath) {

    //gets Vertex2 objects, so it contains normals
    objectLoader objLoader(filePath);
    mVertices2 = objLoader.getVertices();
    std::cout << "Player Geometry loaded" << std::endl;
    //createNormals();
    createIndices();
}

void Player::setTransform(float x, float y, float z, float angle) {
    //set the transformation info, then create the transformationMatrix
    mX = x;
    mY = y;
    mZ = z;
    mRotAngle = angle;

}

void Player::setShape(std::vector<Vertex> vertices) {
    //pass in a vector with vertex info (pos and color) do this with origin at the center of the object
    mVertices = vertices;
    //create a new vector with all the normal information
    //createNormals();

}

void Player::createIndices() {
    for (unsigned int i = 0; i < mVertices2.size(); i += 3) {
        mIndices.push_back(i);
        mIndices.push_back(i + 1);
        mIndices.push_back(i + 2);
    }
    printf("created indices for player \n");
}

Player::Player(std::string _geomPath) {
    mGeomPath = _geomPath;
}

void Player::createNormals() {
    //Create normals here
    //take segments of 3 vertices per iteration, add the calculated normal to the normallist
    for (unsigned int i = 0; i < mVertices.size(); i += 3) {
        //on next loop if i was 0, i will start from 3. [3] [4] [5] ..
        Vertex tempVertex1 = mVertices[i];
        Vertex tempVertex2 = mVertices[i + 1];
        Vertex tempVertex3 = mVertices[i + 2];
        glm::vec3 a = tempVertex3.Pos - tempVertex1.Pos;//from vert 1 to vert 2
        glm::vec3 b = tempVertex2.Pos - tempVertex1.Pos;//from vert1 to vert 3
        glm::vec3 normal = glm::normalize(glm::cross(a, b));
        mVertices2.push_back(Vertex2(tempVertex1, normal, glm::vec3(32, 0.5f, 0.0f)));
        mVertices2.push_back(Vertex2(tempVertex2, normal, glm::vec3(32, 0.5f, 0.0f)));
        mVertices2.push_back(Vertex2(tempVertex3, normal, glm::vec3(32, 0.5f, 0.0f)));


    }
    std::cout << "normals Created for player" << std::endl;
}

glm::vec3 Player::getPosition() {
    return glm::vec3(mX, mY, mZ);
}
void Player::fillOffsets() {
    //make only 3 instances
    std::cout << "filling offsets" << std::endl;
    for (unsigned int i = 0; i < mInstances; i++) {
        for (unsigned int j = 0; j < mInstances; j++) {
            mOffsets.push_back(glm::vec3((14.0f)*(j - (mInstances - 1) / 2.0f), 0, (-11.0f)*i));
        }
    }

}

void Player::update() {

    mTransformation = glm::mat4(glm::translate(glm::mat4(1), glm::vec3(mX, mY, mZ)) * glm::rotate(glm::mat4(1), mRotAngle, glm::vec3(0, 0, 1)));

}

void Player::render(int shaderProgramID) {
    //set transform
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(mTransformation));
    //glUniform1f(glGetUniformLocation(shaderProgramID, "time"), SDL_GetTicks()/1000.0f);
    glBindVertexArray(mVaoPlayer);

    glDrawElementsInstanced(GL_TRIANGLES, mVertices2.size(), GL_UNSIGNED_INT, &mIndices[0], mOffsets.size());
    //glDrawElements(GL_TRIANGLES,mIndices.size(), GL_UNSIGNED_INT, &mIndices[0]);
    refreshShaderTransforms(shaderProgramID);
}

void Player::refreshShaderTransforms(int shaderProgramID) {
    //revert too unity as model matrix, ie no transforms
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
}

void Player::setInstances(int amount) {
    mInstances = amount;
}

void Player::initGL() {
    //test fill the offsetarray (contains offset vectors)
    fillOffsets();

    //vertexBuffer
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mInstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, mOffsets.size() * sizeof(glm::vec3), &mOffsets[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2)*mVertices2.size(), &mVertices2[0], GL_STATIC_DRAW);

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
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid *)(2 * sizeof(glm::vec3)));
    //Materials
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (GLvoid *)(3 * sizeof(glm::vec3)));

    //instancing
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glVertexAttribDivisor(4, 1);


    ////indices 
    //glGenBuffers(1, &mIbo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
    std::cout << "drawing " << mVertices2.size() << " vertices with " << mIndices.size() << " indices" << std::endl;
    std::cout << "initialised  player gl" << std::endl;

}

void Player::loadDefaultGeometry() {
    this->loadGeometry(mGeomPath);
}
void Player::cleanup() {
    glDeleteVertexArrays(1, &mVaoPlayer);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mInstanceVBO);
}