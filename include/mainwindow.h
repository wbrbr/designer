#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "viewportwidget.h"
#include "texturewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel* m_label;
    // QDockWidget* m_dockleft;
    QLabel* m_label2;
    ViewportWidget* m_viewport;
    TextureWidget* m_texview;
};

#endif // MAINWINDOW_H
