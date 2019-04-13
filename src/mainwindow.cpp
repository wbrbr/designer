#include "mainwindow.h"
// #include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    m_label = new QLabel(this);
    m_label->setText("coucou !");
    setCentralWidget(m_label);
    QDockWidget* m_dock = new QDockWidget("Dock", this);
    m_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_dock);
    m_label2 = new QLabel(m_dock);
    m_label2->setText("hello");
    m_dock->setWidget(m_label2);

    m_dock = new QDockWidget("Viewport", this);
    m_dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, m_dock);
    m_viewport = new ViewportWidget(m_dock);
    m_dock->setWidget(m_viewport);

    m_dock = new QDockWidget("Texture", this);
    m_dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, m_dock);
    m_texview = new TextureWidget(m_dock);
    m_dock->setWidget(m_texview);
}

MainWindow::~MainWindow()
{
}
