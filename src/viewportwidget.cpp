#include "viewportwidget.h"

ViewportWidget::ViewportWidget(QWidget* parent): QOpenGLWidget(parent)
{
}

void ViewportWidget::initializeGL()
{
    initializeOpenGLFunctions();
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glClearColor(0.f, 0.f, 1.f, 1.f);

    m_shader.initialize("shaders/simple.vert", "shaders/simple.frag");
}

void ViewportWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void ViewportWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_shader.id());
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

QSize ViewportWidget::sizeHint() const
{
    return QSize(300, 300);
}
