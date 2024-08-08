/**************************************************************************//**
* @file      Node.cpp
* @brief     Node configuration and control
* @author    Adi
* @date      2024-2-4

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include "node.h"
#include "Transformation.h"
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/

/******************************************************************************
* Function Prototypes
******************************************************************************/

/******************************************************************************
* Function Implementation
******************************************************************************/
// Constructor
Node::Node() : Node(nullptr, {1,1,1}, "Default", glm::mat3())
{}

Node::Node(glm::mat3 transformationMatrix, QString name)
{
    this->transform = transformationMatrix;
    this->name = name;
    setText(0, name);
}

Node::Node(Polygon2D * poly1,
           // The color with which to draw the Polygon2D
           glm::vec3 RGB1,
           // QString to represent a name for the node
           QString name1,
           // Transformation matrix
           glm::mat3 transform1) {
    // children[0] = nullptr;
    // TranslateNode(0, 0);
    RGB = RGB1;

    poly = poly1;
    name = name1;
    transform = transform1;
}

// Function that adds a given unique_ptr as a child to this node
Node& Node::Node_AddChild(uPtr<Node> child) {
    Node& ref = *child;
    this->children.push_back(std::move(child));
    return ref;
}

// A function that allows the user to modify the color stored in this node.
void Node::Node_ChangeColor(glm::vec3 Color) {
    this->RGB[0] = Color[0];
    this->RGB[1] = Color[1];
    this->RGB[2] = Color[2];

    return;
}

void Node::Node_SetName(QString n) {
    name = n;
    return;
}

void Node::Node_SetRGB(glm::vec3 color) {
    RGB[0] = color[0];
    RGB[1] = color[1];
    RGB[2] = color[2];
    return;
}

glm::vec3 Node::Node_GetRGB(void) {
    return RGB;
}

void Node::Node_SetTransform(glm::mat3 t1) {
    transform = t1;
    return;
}

glm::mat3 Node::Node_GetTransform(void) {
    return transform;
}

void Node::Node_SetPoly(Polygon2D * p) {
    poly = p;
    return;
}

Polygon2D * Node::Node_GetPoly(void) {
    return poly;
}

uPtr<Node> Node::Node_GetChild(int i) {
    if(children[i] != nullptr) {
        return std::move(children[i]);
    }
    return nullptr;
}

int Node::Node_GetChildrenSize(void) {
    return children.size();
}

// void Node::traverse(uPtr<Node> N, glm::mat3 T, ShaderProgram shader) {
//     T = T * this->transform;
//     for (size_t i = 0; i < this->children.size(); i++) {
//         traverse(std::move(this->children[i]), T);
//     }
// }
