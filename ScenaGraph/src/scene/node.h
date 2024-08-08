#pragma once
#include <QTreeWidgetItem>
#include "smartpointerhelp.h"
#include "polygon.h"

class Node : public QTreeWidgetItem {
private:
    std::vector<uPtr<Node>> children;
    // pointer to one instance of Polygon2D
    Polygon2D * poly;
    // The color with which to draw the Polygon2D
    glm::vec3 RGB;
    // QString to represent a name for the node
    QString name;
    // Transformation matrix
    glm::mat3 transform;
public:
    //Constructor
    Node();
    Node(glm::mat3 transformationMatrix, QString name);
    Node(Polygon2D * poly,
               // The color with which to draw the Polygon2D
               glm::vec3 RGB,
               // QString to represent a name for the node
               QString name,
               // Transformation matrix
               glm::mat3 transform);
    // Function that computes and returns a 3x3 homogeneous matrix representing the transformation in the node.
    virtual glm::mat3 Node_Transformation(void) = 0;
    // Function that adds a given unique_ptr as a child to this node
    Node& Node_AddChild(uPtr<Node> child);
    // A function that allows the user to modify the color stored in this node.
    void Node_ChangeColor(glm::vec3 Color);
    // Name
    void Node_SetName(QString nodename);
    glm::vec3 Node_GetRGB(void);
    // Color
    void Node_SetRGB(glm::vec3 color);
    // Transform
    void Node_SetTransform(glm::mat3 t1);
    glm::mat3 Node_GetTransform(void);
    // Polygon
    Polygon2D * Node_GetPoly(void);
    // void traverse(uPtr<Node> N, glm::mat3 T, ShaderProgram shader);
    void Node_SetPoly(Polygon2D * p);
    // Children
    uPtr<Node> Node_GetChild(int i);
    int Node_GetChildrenSize(void);
};
