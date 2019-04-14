#include "nodeeditor.h"
#include <algorithm>
#include <iostream>

InputSocketUI::InputSocketUI()
{
    sock = nullptr;
    conn = nullptr;
}

void InputSocketUI::connect(OutputSocketUI* out)
{
    conn = out;
    sock->conn = out->sock;
}

NodeEditor::NodeEditor(): m_last_click_pos(0, 0)
{
    setMouseTracking(false);
    m_dragged_node = nullptr;
    m_sel_output_socket = nullptr;
}

void NodeEditor::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    for (auto it : m_nodes)
    {
        p.drawRect(it.pos.x(), it.pos.y(), 90, 90);

        int y = it.pos.y() + 15;

        for (auto sock : it.inputs)
        {
            p.drawEllipse(sock.pos, 10, 10);
            if (sock.conn != nullptr) {
                p.drawLine(sock.pos, sock.conn->pos);
            }
        }

        for (auto sock : it.outputs)
        {
            p.drawEllipse(sock.pos, 10, 10);
        }

    }

    if (m_sel_output_socket != nullptr) {
        QPoint mousepos = mapFromGlobal(QCursor::pos());
        p.drawLine(m_sel_output_socket->pos, mousepos);
    }
}

NodeUI* NodeEditor::getNodeAt(QPoint p)
{
    for (auto& n : m_nodes)
    {
        QRect rect(n.pos, QSize(90, 90));
        if (rect.contains(p)) {
            return &n;
        }
    }
    return nullptr;
}

OutputSocketUI* NodeEditor::getOutputSocketAt(QPoint p)
{
    for (auto& n : m_nodes)
    {
        for (auto& sock : n.outputs)
        {
            QRect rect(sock.pos - QPoint(10, 10), QSize(20, 20));
            if (rect.contains(p)) {
                return &sock;
            }
        }
    }
    return nullptr;
}

InputSocketUI* NodeEditor::getInputSocketAt(QPoint p)
{
    for (auto& n : m_nodes)
    {
        for (auto& sock : n.inputs)
        {
            QRect rect(sock.pos - QPoint(10, 10), QSize(20, 20));
            if (rect.contains(p)) {
                return &sock;
            }
        }
    }
    return nullptr;
}

void NodeEditor::mouseReleaseEvent(QMouseEvent *event)
{
    m_last_click_pos = event->pos();

    if (m_sel_output_socket != nullptr) {
        InputSocketUI* in = getInputSocketAt(event->pos());
        if (in != nullptr) {
            in->connect(m_sel_output_socket);
        }
    }

    m_sel_output_socket = nullptr;
    m_dragged_node = nullptr;
    update();
}

void NodeEditor::mouseMoveEvent(QMouseEvent *event)
{
    QPoint vel = event->pos() - m_last_drag_pos;
    m_last_drag_pos = event->pos();
    if (m_dragged_node != nullptr) {
        m_dragged_node->pos += vel;
        for (auto& sock : m_dragged_node->inputs)
        {
            sock.pos += vel;
        }
        for (auto& sock : m_dragged_node->outputs)
        {
            sock.pos += vel;
        }
    }
    update();
}

void NodeEditor::mousePressEvent(QMouseEvent *event)
{
    m_last_drag_pos = event->pos();
    m_sel_output_socket = getOutputSocketAt(event->pos());
    if (m_sel_output_socket == nullptr) m_dragged_node = getNodeAt(event->pos());
}

void NodeEditor::mouseDoubleClickEvent(QMouseEvent *event)
{
    NodeUI* node = getNodeAt(event->pos());
    if (node != nullptr) {
        previewNode(node->node);
    }
}

void NodeEditor::addNode(Node *n)
{
    NodeUI nui;
    nui.node = n;
    nui.pos = m_last_click_pos;

    int y = nui.pos.y() + 15;
    int x = nui.pos.x() + 90;
    for (auto& it : n->outputs())
    {
        OutputSocketUI out;
        out.sock = &it;
        out.pos = QPoint(x, y);
        y += 30;
        nui.outputs.push_back(out);
    }

    y = nui.pos.y() + 15;
    x = nui.pos.x();
    for (auto& it : n->inputs())
    {
        InputSocketUI in;
        in.sock = &it;
        in.pos = QPoint(x, y);
        y += 30;
        nui.inputs.push_back(in);
    }
    m_nodes.push_back(nui);
    update();
}
