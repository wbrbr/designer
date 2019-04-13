#include "node.h"
#include <cassert>
#include <iostream>

Node::Node() {
    initializeOpenGLFunctions();
}

Node::~Node() {}

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
        std::cout << "coucou" << std::endl;
    }
}

CheckersNode::CheckersNode()
{
    m_outputs.push_back(OutputSocket());
    m_outputs[0].num = 0;
    m_outputs[0].name = "out";
    m_outputs[0].owner = this;
    m_outputs[0].type = COLOR;

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
    comp_shader.compileSourceFile("damier.glsl");
    m_shader.addShader(&comp_shader);
    m_shader.link();
}

std::optional<Data> CheckersNode::compute(unsigned int num)
{
    assert(num == 0);

    m_shader.bind();
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
    comp_shader.compileSourceFile("color2gray.glsl");
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
    comp_shader.compileSourceFile("gray2color.glsl");
    m_shader.addShader(&comp_shader);
    m_shader.link();
}

std::optional<Data> GrayscaleToColorNode::compute(unsigned int num)
{
    assert(num == 0);
    bindImageInput(0, 0);

    m_shader.bind();
    glBindImageTexture(1, m_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(32, 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    Data d;
    d.type = COLOR;
    d.texture = m_tex;

    return d;
}
