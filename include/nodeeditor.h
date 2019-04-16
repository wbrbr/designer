#ifndef NODEEDITOR_H
#define NODEEDITOR_H
#include <QtWidgets>
#include "node.h"


class OutputSocketUI
{
public:
    OutputSocket* sock;
    QPoint pos;
};

class InputSocketUI
{
public:
    InputSocket* sock;
    OutputSocketUI* conn;
    QPoint pos;

    InputSocketUI();
    void connect(OutputSocketUI* out);
};

class NodeUI
{
public:
    Node* node;
    std::vector<OutputSocketUI> outputs;
    std::vector<InputSocketUI> inputs;
    QPoint pos;
};

class NodeEditor: public QWidget
{
    Q_OBJECT

public:
    NodeEditor();
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void addNode(Node* n);

signals:
    void previewNode(Node* n);
    void selectNode(Node* n);

private:
    std::vector<NodeUI> m_nodes;
    QPoint m_last_click_pos;
    NodeUI* m_dragged_node;
    QPoint m_last_drag_pos;
    OutputSocketUI* m_sel_output_socket;

    NodeUI* getNodeAt(QPoint p);
    OutputSocketUI* getOutputSocketAt(QPoint p);
    InputSocketUI* getInputSocketAt(QPoint p);
};

#endif // NODEEDITOR_H
