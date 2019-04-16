#include "propertyeditor.h"


PropertyEditor::PropertyEditor(QWidget* parent): QWidget(parent)
{
    m_layout = new QFormLayout(this);
    setLayout(m_layout);
    m_node = nullptr;
}

void PropertyEditor::editNode(Node* n)
{
    if (n != m_node) {
        if (m_node != nullptr) clear();
        m_node = n;
        for (auto& it  : m_node->properties())
        {
            NodeProperty* p = it.second;
            switch(p->type) {
            case PROP_INT: {
                IntNodeProperty* ip = static_cast<IntNodeProperty*>(p);
                QSlider* slider = new QSlider(this);
                slider->setMinimum(ip->min);
                slider->setMaximum(ip->max);
                slider->setValue(ip->val);
                connect(slider, &QSlider::valueChanged, this, [=] (int val) {
                    ip->val = val;
                    propertyChanged();
                });
                m_layout->addRow(QString::fromStdString(it.first), slider);
                break; }

            case PROP_FLOAT: {
                FloatNodeProperty* fp = static_cast<FloatNodeProperty*>(p);
                QSlider* slider = new QSlider(this);
                slider->setMinimum((int)(fp->min*1000.f));
                slider->setMaximum((int)(fp->max*1000.f));
                slider->setValue((int)(fp->val*1000.f));
                connect(slider, &QSlider::valueChanged, this, [=] (int val) {
                    fp->val = (float)val / 1000.f;
                    propertyChanged();
                });
                m_layout->addRow(QString::fromStdString(it.first), slider);
                break; }

            case PROP_COLOR: {
                ColorNodeProperty* cp = static_cast<ColorNodeProperty*>(p);
                QPushButton* button = new QPushButton("Pick color",this);
                connect(button, &QPushButton::released, this, [=] {
                    QColor init((int)(cp->val.r*255.f), (int)(cp->val.g*255.f), (int)(cp->val.b*255.f));
                    QColor col = QColorDialog::getColor(init, this);
                    cp->val.r = col.redF();
                    cp->val.g = col.greenF();
                    cp->val.b = col.blueF();
                    propertyChanged();
                });
                m_layout->addRow(QString::fromStdString(it.first), button);
                break; }
            }
        }
    }
}

void PropertyEditor::clear()
{
    m_node = nullptr;
    QLayoutItem* item;
    while ((item = m_layout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}

void PropertyEditor::nodeRemoved(Node* n)
{
    if (n == m_node) {
        clear();
    }
}
