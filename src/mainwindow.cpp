#include "mainwindow.h"
// #include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);
    m_nodeedit = new NodeEditor();
    /* m_nodeedit->addNode(nullptr, 0, 0);
    m_nodeedit->addNode(nullptr, 2, 2); */
    setCentralWidget(m_nodeedit);

    QDockWidget* m_dock = new QDockWidget("Viewport", this);
    m_dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, m_dock);
    m_viewport = new ViewportWidget(m_dock);
    m_dock->setWidget(m_viewport);

    m_dock = new QDockWidget("Texture", this);
    m_dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, m_dock);
    m_texview = new TextureWidget(m_dock);
    m_dock->setWidget(m_texview);

    connect(m_nodeedit, &NodeEditor::previewNode, m_texview, &TextureWidget::setNode);

    m_addCheckersAction = new QAction("Add Checkers", this);
    connect(m_addCheckersAction, &QAction::triggered, [=]{
        m_nodeedit->addNode(new CheckersNode);
    });
    m_addColorToGrayAction = new QAction("Add ColorToGrayscale", this);
    connect(m_addColorToGrayAction, &QAction::triggered, [=]{
        m_nodeedit->addNode(new ColorToGrayscaleNode);
    });
    m_addGrayToColorAction = new QAction("Add GrayscaleToColor", this);
    connect(m_addGrayToColorAction, &QAction::triggered, [=]{
        m_nodeedit->addNode(new GrayscaleToColorNode);
    });

    m_addMenu = menuBar()->addMenu("&Add");
    m_addMenu->addAction(m_addCheckersAction);
    m_addMenu->addAction(m_addColorToGrayAction);
    m_addMenu->addAction(m_addGrayToColorAction);
}

MainWindow::~MainWindow()
{
}
