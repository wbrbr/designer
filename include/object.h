#ifndef OBJECT_HPP
#define OBJECT_HPP
#include "transform.h"
#include "mesh.h"

struct Object
{
    Transform transform;
    Mesh mesh;
};
#endif
