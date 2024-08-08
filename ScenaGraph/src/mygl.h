#pragma once

#include <openglcontext.h>
#include <shaderprogram.h>
#include <scene/grid.h>
#include <scene/polygon.h>
#include <QTreeWidgetItem>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include "scene/node.h"

#define FIRST 2

class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    Grid m_geomGrid; // The instance of the object used to render the 5x5 grid

    Polygon2D m_cloud; // The singular instance of our circle object that can be re-drawn with different colors
        // to create the appearance of there being multiple squares

    Polygon2D m_floor;

    Polygon2D Shoe_Left;
    Polygon2D Shoe_Right;

    Polygon2D Leg_RightUp;
    Polygon2D Leg_LeftUp;
    Polygon2D Leg_RightDown;
    Polygon2D Leg_LeftDown;

    Polygon2D Arm_RightUp;
    Polygon2D Arm_LeftUp;
    Polygon2D Arm_RightDown;
    Polygon2D Arm_LeftDown;

    Polygon2D m_torso;
    Polygon2D m_neck;
    Polygon2D m_head;
    Polygon2D m_fistR;
    Polygon2D m_fistL;

    Polygon2D m_hair;

    bool m_showGrid; // Read in paintGL to determine whether or not to draw the grid.

    int TranslateSceneX;
    int Rotate_LegUp;
    int Rotate_LegDown;
    float rotateArmUp;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.

    Node *mp_selectedNode; // A pointer to the Node that was last clicked on in the GUI's Tree Widget

    // Root node
    uPtr<Node> root;

public:
    explicit MyGL(QWidget *parent = 0);

    uPtr<Node> Root;
    uPtr<Node> Body;
    // Node* Head;
    uPtr<Node> UpperLimb;
    // Node* LowerLimb;

    uPtr<Node> UpperArmRight;
    uPtr<Node> LowerArmRight;
    uPtr<Node> UpperArmLeft;
    uPtr<Node> LowerArmLeft;

    uPtr<Node> UpperLegRight;
    uPtr<Node> LowerLegRight;
    uPtr<Node> UpperLegLeft;
    uPtr<Node> LowerLegLeft;

    uPtr<Node> Neck;
    uPtr<Node> Head;
    // uPtr<Node> UpperLegLeft;
    // uPtr<Node> LowerLegLeft;

    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void traverse(uPtr<Node> N, glm::mat3 T, ShaderProgram shader);

protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sig_sendRootNode(QTreeWidgetItem*);
    void sig_TX(int);
    void sig_TX_Test(int);
    void sig_rotateArmUp(float);
    void sig_scenegraph(QTreeWidgetItem*);

public slots:
    // Assigns mp_selectedNode to the input pointer.
    // Is connected to a signal from the Tree Widget in the GUI
    // that is emitted every time an element in the widget is clicked.
    void slot_setSelectedNode(QTreeWidgetItem*);

    // TODO: Add slots for altering the currently selected Node's
    // translate / rotate / scale value(s). We have provided
    // an example for updating a Translate Node's X value.
    void slot_setTX(double);

    // TODO: Add slots to add a new Translate / Rotate / Scale Node
    // as a child to the currently selected Node. We have provided
    // an example for adding a Translate Node.
    void slot_addTranslateNode();

    void slot_addTranslateXNode(int);
    void slot_addRotateLegUp(int);
    void slot_addRotateLegDown(int);
    void slot_addRotateArmUpNode(float);

    // TODO: Add a slot to set the Polygon2D pointer of the currently
    // selected Node to the provided m_geomSquare
};

