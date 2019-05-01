#ifndef MESH_HPP
#define MESH_HPP
#include <optional>
#include <QOpenGLFunctions_4_3_Core>
#include <string>

struct Mesh: public QOpenGLFunctions_4_3_Core
{
    Mesh(std::string path);
    unsigned int vao, vbo, numVertices;
};

#endif
