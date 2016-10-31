#include "objLoader.h"

objectLoader::objectLoader(std::string _inputFile) {
    filePath = _inputFile;
    ret=tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str());


    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                float vx = attrib.vertices[3 * idx.vertex_index + 0];
                float vy = attrib.vertices[3 * idx.vertex_index + 1];
                float vz = attrib.vertices[3 * idx.vertex_index + 2];
                float nx = attrib.normals[3 * idx.normal_index + 0];
                float ny = attrib.normals[3 * idx.normal_index + 1];
                float nz = attrib.normals[3 * idx.normal_index + 2];
                float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                mVertices.push_back(Vertex2(
                    Vertex(
                        glm::vec3(vx,vy,vz),
                        glm::vec3(0.5f- vz*vz /5.f, 0.5f - vz / 5.f, 0.5f - vz / 5.f)),
                    glm::vec3(nx,ny,nz)));
                mIndices.push_back(3 * idx.vertex_index + 0);
                mIndices.push_back(3 * idx.vertex_index + 1);
                mIndices.push_back(3 * idx.vertex_index + 2);
            }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}


std::vector<Vertex2> objectLoader::getVertices() {
    return mVertices;

}
std::vector<GLushort> objectLoader::getIndices() {
    return mIndices;
}

//
//
//std::vector<Vertex2> objectLoader::getVertices() {
//
//    std::vector<GLushort> vertexIndices, uvIndices, normalIndices;
//    std::vector< glm::vec3 > temp_vertices;
//    std::vector< glm::vec2 > temp_uvs;
//    std::vector< glm::vec3 > temp_normals;
//
//    //open the file
//    FILE * file = fopen(filePath.c_str(), "r");
//    if (file == NULL) {
//        printf("Impossible to open the file !\n");
//    }
//
//    //read from file
//    while (1) {
//
//        char lineHeader[128];
//        // read the first word of the line
//        int res = fscanf(file, "%s", lineHeader);
//        if (res == EOF) {
//            break; // EOF = End Of File. Quit the loop.
//
//                   // else : parse lineHeader
//        }
//        if (strcmp(lineHeader, "v") == 0) {
//            //read vertex data
//            glm::vec3 vertex;
//            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
//            temp_vertices.push_back(vertex);
//        }
//        else if (strcmp(lineHeader, "vt") == 0) {
//            //read uv
//            glm::vec2 uv;
//            fscanf(file, "%f %f\n", &uv.x, &uv.y);
//            temp_uvs.push_back(uv);
//        }
//        else if (strcmp(lineHeader, "vn") == 0) {
//        glm::vec3 normal;
//        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
//        temp_normals.push_back(normal);
//        }
//        else if (strcmp(lineHeader, "f") == 0) {
//            std::string vertex1, vertex2, vertex3;
//            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//            if (matches != 9) {
//                printf("File can't be read  Try exporting with other options\n");
//            }
//            vertexIndices.push_back(vertexIndex[0]);
//            vertexIndices.push_back(vertexIndex[1]);
//            vertexIndices.push_back(vertexIndex[2]);
//            uvIndices.push_back(uvIndex[0]);
//            uvIndices.push_back(uvIndex[1]);
//            uvIndices.push_back(uvIndex[2]);
//            normalIndices.push_back(normalIndex[0]);
//            normalIndices.push_back(normalIndex[1]);
//            normalIndices.push_back(normalIndex[2]);
//
//        }
//    }
//    mIndices = vertexIndices;
//    std::cout << temp_normals.size()<<" "<<temp_vertices.size();
//    for (unsigned int i = 0; i < temp_vertices.size(); i++) {
//        mVertices.push_back(Vertex2(Vertex(
//            temp_vertices[i],
//            glm::vec3(temp_vertices[i].z / 10.0, temp_vertices[i].z / 10.0, temp_vertices[i].z / 10.0)),
//            glm::vec3(0, 0, 0)));
//    }
//   return mVertices;
//}
//std::vector<GLushort> objectLoader::getIndices() {
//    return mIndices;
//}