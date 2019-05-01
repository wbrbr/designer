#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);
    if (!scene) {
        throw importer.GetErrorString();
    }
    aiMesh* m = scene->mMeshes[0];

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, m->mNumVertices * 14 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m->mNumVertices * 3 * sizeof(float), m->mVertices);
    glBufferSubData(GL_ARRAY_BUFFER, m->mNumVertices * 3 * sizeof(float), m->mNumVertices * 3 * sizeof(float), m->mNormals);
    if (m->HasTextureCoords(0)) {
        float* texcoords = static_cast<float*>(malloc(m->mNumVertices * 2 * sizeof(float)));
        for (unsigned int i = 0; i < m->mNumVertices; i++)
        {
            auto vec = m->mTextureCoords[0][i];
            texcoords[2*i] = vec.x;
            texcoords[2*i+1] = vec.y;
        }
        glBufferSubData(GL_ARRAY_BUFFER, m->mNumVertices * 6 * sizeof(float), m->mNumVertices * 2 * sizeof(float), texcoords);
        free(texcoords);
    } else {
        float* zero = static_cast<float*>(malloc(m->mNumVertices * 2 * sizeof(float)));
        for (unsigned int i = 0; i < m->mNumVertices * 2; i++)
        {
            zero[i] = 0.f;
        }
        glBufferSubData(GL_ARRAY_BUFFER, m->mNumVertices * 6 * sizeof(float), m->mNumVertices * 2 * sizeof(float), zero);
        free(zero);
    }
    glBufferSubData(GL_ARRAY_BUFFER, m->mNumVertices * 8 * sizeof(float), m->mNumVertices * 3 * sizeof(float), m->mTangents);
    glBufferSubData(GL_ARRAY_BUFFER, m->mNumVertices * 11 * sizeof(float), m->mNumVertices * 3 * sizeof(float), m->mBitangents);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); /* positions */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m->mNumVertices * 3 * sizeof(float))); /* normals */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m->mNumVertices * 6 * sizeof(float))); /* UV coords */
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m->mNumVertices * 8 * sizeof(float))); /* tangents */
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(m->mNumVertices * 11 * sizeof(float))); /* bitangents */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
}
