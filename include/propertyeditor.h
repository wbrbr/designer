#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H
#include <QtWidgets>
#include "node.h"

class PropertyEditor: public QWidget
{
    Q_OBJECT

public:
    PropertyEditor(QWidget* parent);

public slots:
    void editNode(Node* n);
    void nodeRemoved(Node* n);

signals:
    void propertyChanged();

private:
    Node* m_node;
    QBoxLayout* m_layout;
    std::vector<QWidget*> m_widgets;

    void clear();
};

#endif // PROPERTYEDITOR_H
