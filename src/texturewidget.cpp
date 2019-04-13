#include "texturewidget.h"
#include "texture.h"
#include "node.h"

TextureWidget::TextureWidget(QWidget* parent): QOpenGLWidget(parent), m_tex(0)
{
}

void TextureWidget::initializeGL()
{
    initializeOpenGLFunctions();

    float vertices[] = {
        -1.f, -1.f, 0.f, 0.f,
        1.f, -1.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f,

        -1.f, -1.f, 0.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        -1.f, 1.f, 0.f, 1.f
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glClearColor(0.f, 1.f, 0.f, 1.f);

    m_shader.initialize("tex.vert", "tex.frag");
    ImageTexture* tex = new ImageTexture();
    tex->load("tex.png");

    /* unsigned int gen_tex;
    glGenTextures(1, &gen_tex);
    glBindTexture(GL_TEXTURE_2D, gen_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    QOpenGLShader comp_shader(QOpenGLShader::Compute);
    comp_shader.compileSourceFile("damier.glsl");
    QOpenGLShaderProgram comp_program;
    comp_program.addShader(&comp_shader);
    comp_program.link();

    comp_program.bind();
    glBindTexture(GL_TEXTURE_2D, gen_tex);
    glBindImageTexture(0, gen_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(32, 32, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0); */

    CheckersNode ck;
    ColorToGrayscaleNode cg;
    GrayscaleToColorNode gc;

    cg.connect(0, &ck, 0);
    gc.connect(0, &cg, 0);

    m_tex = gc.compute(0)->texture;
    // m_tex = tex->id();
}

void TextureWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void TextureWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_shader.id());
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

QSize TextureWidget::sizeHint() const
{
    return QSize(300, 300);
}

void TextureWidget::setTexture(unsigned int tex)
{
    m_tex = tex;
}
