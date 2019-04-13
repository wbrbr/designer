#ifndef SHADER_HPP
#define SHADER_HPP
#include <string>
#include <unordered_map>
#include <QOpenGLFunctions_4_3_Core>


unsigned int loadShaderProgramFromSource(const char* vSource, const char* fSource);
unsigned int loadShaderProgram(std::string vPath, std::string fPath);

class Shader: protected QOpenGLFunctions_4_3_Core
{
public:
    Shader();
    Shader(std::string vPath, std::string fPath);
    ~Shader();
    void initialize(std::string vPath, std::string fPath);
    int getLoc(std::string name);
    unsigned int id();

private:
    unsigned int loadShaderFromSource(unsigned int type, const char* source);
    unsigned int loadShaderProgramFromSource(const char* vSource, const char* fSource);
    unsigned int loadShaderProgram(std::string vPath, std::string fPath);
    unsigned int m_id;
    std::unordered_map<std::string, int> m_locs;
    std::string m_name;
};
#endif
