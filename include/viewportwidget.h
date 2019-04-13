#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H
#include <QtWidgets>
#include <QOpenGLFunctions_4_3_Core>
#include "shader.h"

class ViewportWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    ViewportWidget(QWidget* parent);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    QSize sizeHint() const override;

private:
    Shader m_shader;
    unsigned int m_vao, m_vbo;
};

#endif // VIEWPORTWIDGET_H
