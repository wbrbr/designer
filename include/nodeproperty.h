#ifndef NODE_PROPERTY_H
#define NODE_PROPERTY_H
#include "color.h"


enum NodePropertyType
{
    PROP_FLOAT,
    PROP_INT,
    PROP_COLOR
};

class NodeProperty
{
public:
    NodePropertyType type;
};

class IntNodeProperty: public NodeProperty
{
public:
    IntNodeProperty();
    int val, max, min;
};

class FloatNodeProperty: public NodeProperty
{
public:
    FloatNodeProperty();
    float val, max, min;
};

class ColorNodeProperty: public NodeProperty
{
public:
    ColorNodeProperty();
    Color val;
};

#endif // NODE_PROPERTY_H
