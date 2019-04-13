#ifndef NODE_H
#define NODE_H
#include <string>
#include <vector>
#include <optional>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>

enum SocketType
{
    GRAYSCALE,
    COLOR,
    SCALAR
};

struct Data
{
    SocketType type;
    unsigned int texture;
    float scalar;
};

class Socket
{
public:
    std::string name;
    SocketType type;
    unsigned int num;
};

class Node;

class OutputSocket: public Socket
{
public:
    Node* owner;
    std::optional<Data> get();
};

class InputSocket: public Socket
{
public:
    OutputSocket* conn;
    std::optional<Data> get();
};

class Node: protected QOpenGLFunctions_4_3_Core
{
public:
    Node();
    virtual ~Node();
    virtual std::optional<Data> compute(unsigned int num) = 0;
    void connect(unsigned int inNum, Node* other, unsigned int outNum);

protected:
    std::vector<InputSocket> m_inputs;
    std::vector<OutputSocket> m_outputs;
};

class ImageComputeShaderNode: public Node
{
public:
    virtual std::optional<Data> compute(unsigned int num) = 0;
    void bindImageInput(unsigned int num, unsigned int binding);
};

class CheckersNode: public Node
{
public:
    CheckersNode();
    std::optional<Data> compute(unsigned int num) override;

private:
    unsigned int m_tex;
    QOpenGLShaderProgram m_shader;
};

class ColorToGrayscaleNode: public ImageComputeShaderNode
{
public:
    ColorToGrayscaleNode();
    std::optional<Data> compute(unsigned int num) override;

private:
    unsigned int m_tex;
    QOpenGLShaderProgram m_shader;
};

class GrayscaleToColorNode: public ImageComputeShaderNode
{
public:
    GrayscaleToColorNode();
    std::optional<Data> compute(unsigned int num) override;

private:
    unsigned int m_tex;
    QOpenGLShaderProgram m_shader;
};

#endif // NODE_H