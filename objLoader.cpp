#include "objLoader.h"

objectLoader::objectLoader(std::string _inputFile) {
    filePath = _inputFile;
    std::cout << "Loading object: " << filePath << std::endl;
    //mtlPath sets the path automaticly, but you have to keep the materials in the same folder as the mesh
    std::string mtlPath = _inputFile.substr(0, _inputFile.find_last_of("/")+1);
    ret=tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str(),mtlPath.c_str());


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
                /*float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                float ty = attrib.texcoords[2 * idx.texcoord_index + 1];*/
                float c1 = 1, c2 = 1, c3 = 1;
                if (f < shapes[s].mesh.material_ids.size()&& shapes[s].mesh.material_ids.size()>=0) {
                    c1 = materials[shapes[s].mesh.material_ids[f]].diffuse[0];
                    c2 = materials[shapes[s].mesh.material_ids[f]].diffuse[1];
                    c3 = materials[shapes[s].mesh.material_ids[f]].diffuse[2];
                }
                mVertices.push_back(Vertex2(
                    Vertex(
                        glm::vec3(vx,vy,vz),
                        glm::vec3(c1,c2,c3)),
                    glm::vec3(-nx,-ny,-nz)));

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

objectLoader::~objectLoader() {
    std::cout << "an objectLoader got deleted" << std::endl;
}