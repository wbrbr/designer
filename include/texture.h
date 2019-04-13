#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <QOpenGLFunctions_4_3_Core>

class ImageTexture: protected QOpenGLFunctions_4_3_Core
{
public:
    ImageTexture();
    ~ImageTexture();
    void load(std::string path, bool srgb=false);
    unsigned int id();
    unsigned int width();
    unsigned int height();

private:
    void set_tex_params();
    unsigned int create_texture(unsigned int width, unsigned int height, int internal_format, int format);

    unsigned int m_id, m_width, m_height, m_channels;
    std::string m_path;
    bool m_srgb;
};

class Cubemap: protected QOpenGLFunctions_4_3_Core
{
public:
    Cubemap(std::string up, std::string down, std::string left, std::string right, std::string front, std::string back);
    ~Cubemap();
    unsigned int id();

private:
    unsigned int m_id;
};
#endif
