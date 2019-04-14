#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "viewportwidget.h"
#include "texturewidget.h"
#include "nodeeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ViewportWidget* m_viewport;
    TextureWidget* m_texview;
    NodeEditor* m_nodeedit;

    QAction* m_addCheckersAction;
    QAction* m_addGrayToColorAction;
    QAction* m_addColorToGrayAction;

    QMenu* m_addMenu;
};

#endif // MAINWINDOW_H
