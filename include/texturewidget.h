#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H
#include <QtWidgets>
#include <QOpenGLFunctions_4_3_Core>
#include "shader.h"

class TextureWidget: public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    TextureWidget(QWidget* parent);
    void setTexture(unsigned int tex);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    QSize sizeHint() const override;

private:
    unsigned int m_tex;
    unsigned int m_vao, m_vbo;
    Shader m_shader;
};

#endif // TEXTUREWIDGET_H
