#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "texture.h"

class TextureMaterial
{
public:
    ImageTexture* albedoMap, metallicMap, roughnessMap, normalMap;
};
#endif
