#include "texturewidget.h"
#include "texture.h"
#include "node.h"

TextureWidget::TextureWidget(QWidget* parent): QOpenGLWidget(parent), m_tex(0)
{
    outsock = nullptr;
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

    m_color_shader.initialize("tex.vert", "tex.frag");
    m_gray_shader.initialize("tex.vert", "texgray.frag");

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
    Shader* shader = &m_color_shader;
    if (outsock != nullptr) {
        auto d = outsock->get();
        if (d.has_value()) {
            assert(d->type == COLOR || d->type == GRAYSCALE);
            m_tex = d->texture;
            if (d->type == GRAYSCALE) shader = &m_gray_shader;
        }
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader->id());
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

void TextureWidget::setNode(Node* n)
{
    assert(n != nullptr);
    for (unsigned int i = 0; i < n->outputs().size(); i++)
    {
        if (n->outputs()[i].type == COLOR || n->outputs()[i].type == GRAYSCALE) {
            outsock = &n->outputs()[i];
            break;
        }
    }
    update();
}
