#include "node.h"
#include <cassert>
#include <iostream>

Node::Node() {
    initializeOpenGLFunctions();
}

Node::~Node()
{
    for (auto& it : m_properties)
    {
        delete it.second;
    }
}

std::vector<InputSocket>& Node::inputs() {
    return m_inputs;
}

std::vector<OutputSocket>& Node::outputs() {
    return m_outputs;
}

std::map<std::string, NodeProperty*>& Node::properties() {
    return m_properties;
}

std::string Node::name() {
    return m_name;
}

template<class T> T* Node::getProp(std::string name) {
    return static_cast<T*>(m_properties.at(name));
}

void Node::connect(unsigned int inNum, Node* other, unsigned int outNum)
{
    assert(inNum < m_inputs.size());

    m_inputs[inNum].conn = &other->m_outputs[outNum];
}


std::optional<Data> OutputSocket::get()
{
    if (owner == nullptr) {
        return {};
    } else {
        return owner->compute(num);
    }
}

std::optional<Data> InputSocket::get()
{
    if (conn == nullptr) {
        return {};
    } else {
        return conn->get();
    }
}

void ImageComputeShaderNode::bindImageInput(unsigned int num, unsigned int binding)
{
    assert(num < m_inputs.size());
    auto d = m_inputs[num].get();
    if (d.has_value()) {
        assert(d->type == COLOR || d->type == GRAYSCALE);
        glBindImageTexture(binding, d->texture, 0, GL_FALSE, 0, GL_READ_ONLY, d->type == COLOR ? GL_RGBA32F : GL_R32F);
    } else {
        glBindImageTexture(binding, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
    }
}

CheckersNode::CheckersNode()
{
    m_name = "Checkers";
    m_outputs.push_back(OutputSocket());
    m_outputs[0].num = 0;
    m_outputs[0].name = "out";
    m_outputs[0].owner = this;
    m_outputs[0].type = COLOR;

    ColorNodeProperty* c1 = new ColorNodeProperty;
    c1->type = PROP_COLOR;
    c1->val.r = 1.f;
    c1->val.b = 0.f;
    c1->val.g = 0.f;
    ColorNodeProperty* c2 = new ColorNodeProperty;
    c2->type = PROP_COLOR;
    c2->val.r = 0.f;
    c2->val.b = 1.f;
    c2->val.g = 1.f;
    m_properties["color1"] = c1;
    m_properties["color2"] = c2;


    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    QOpenGLShader comp_shader(QOpenGLShader::Compute);
    comp_shader.compileSourceFile("compute/damier.glsl");
    m_shader.addShader(&comp_shader);
    m_shader.link();
}

std::optional<Data> CheckersNode::compute(unsigned int num)
{
    assert(num == 0);
    ColorNodeProperty* c1 = getProp<ColorNodeProperty>("color1");
    ColorNodeProperty* c2 = getProp<ColorNodeProperty>("color2");
    m_shader.bind();
    m_shader.setUniformValue("color1", c1->val.r, c1->val.g, c1->val.b);
    m_shader.setUniformValue("color2", c2->val.r, c2->val.g, c2->val.b);
    glBindImageTexture(0, m_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(32, 32, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    Data d;
    d.type = COLOR;
    d.texture = m_tex;

    return d;
}

ColorToGrayscaleNode::ColorToGrayscaleNode()
{
    m_name = "Color to Grayscale";
    m_outputs.push_back(OutputSocket());
    m_outputs[0].num = 0;
    m_outputs[0].name = "grayscale";
    m_outputs[0].owner = this;
    m_outputs[0].type = GRAYSCALE;

    m_inputs.push_back(InputSocket());
    m_inputs[0].num = 0;
    m_inputs[0].name = "color";
    m_inputs[0].type = COLOR;
    m_inputs[0].conn = nullptr;

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    QOpenGLShader comp_shader(QOpenGLShader::Compute);
    comp_shader.compileSourceFile("compute/color2gray.glsl");
    m_shader.addShader(&comp_shader);
    m_shader.link();
}

std::optional<Data> ColorToGrayscaleNode::compute(unsigned int num)
{
    assert(num == 0);

    bindImageInput(0, 0);
    m_shader.bind();
    glBindImageTexture(1, m_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
    glDispatchCompute(32, 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    Data d;
    d.type = GRAYSCALE;
    d.texture = m_tex;

    return d;
}

GrayscaleToColorNode::GrayscaleToColorNode()
{
    m_name = "Grayscale to Color";
    m_outputs.push_back(OutputSocket());
    m_outputs[0].num = 0;
    m_outputs[0].name = "color";
    m_outputs[0].owner = this;
    m_outputs[0].type = COLOR;

    m_inputs.push_back(InputSocket());
    m_inputs[0].num = 0;
    m_inputs[0].name = "grayscale";
    m_inputs[0].type = GRAYSCALE;
    m_inputs[0].conn = nullptr;

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    QOpenGLShader comp_shader(QOpenGLShader::Compute);
    comp_shader.compileSourceFile("compute/gray2color.glsl");
    m_shader.addShader(&comp_shader);
    m_shader.link();

    ColorNodeProperty* tint_prop = new ColorNodeProperty();
    tint_prop->val.r = 1.f;
    tint_prop->val.g = 1.f;
    tint_prop->val.b = 1.f;
    m_properties["tint"] = tint_prop;
}

std::optional<Data> GrayscaleToColorNode::compute(unsigned int num)
{
    assert(num == 0);
    bindImageInput(0, 0);

    ColorNodeProperty* tint = getProp<ColorNodeProperty>("tint");

    m_shader.bind();
    m_shader.setUniformValue("tint", tint->val.r, tint->val.g, tint->val.b);
    glBindImageTexture(1, m_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(32, 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    Data d;
    d.type = COLOR;
    d.texture = m_tex;

    return d;
}
