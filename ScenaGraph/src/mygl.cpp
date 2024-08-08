#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include "scene/node.h"
#include "Transformation.h"

#define BG_RED_COMP     (0.741)
#define BG_GREEN_COMP   (0.941)
#define BG_BLUE_COMP    (0.969)
#define BG_ALPHA_COMP   (1.0)

#define FLOOR_RED_COMP     (0.369)
#define FLOOR_GREEN_COMP   (0.224)
#define FLOOR_BLUE_COMP    (0.027)
#define FLOOR_ALPHA_COMP   (1.0)

#define SHOE_RED_COMP     (0.149)
#define SHOE_GREEN_COMP   (0.122)
#define SHOE_BLUE_COMP    (0.082)
#define SHOE_ALPHA_COMP   (1.0)

#define LEG_RED_COMP     (0.118)
#define LEG_GREEN_COMP   (0.137)
#define LEG_BLUE_COMP    (0.722)
#define LEG_ALPHA_COMP   (1.0)

#define TORSO_RED_COMP     (0.831)
#define TORSO_GREEN_COMP   (0.047)
#define TORSO_BLUE_COMP    (0.333)
#define TORSO_ALPHA_COMP   (1.0)

#define SKIN_RED_COMP     (0.859)
#define SKIN_GREEN_COMP   (0.663)
#define SKIN_BLUE_COMP    (0.49)
#define SKIN_ALPHA_COMP   (1.0)


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
        prog_flat(this),
        m_geomGrid(this),
        // root(TranslateNode(0, 0)),
        m_cloud(this, 100),
#if (FIRST == 0)
        m_floor(this, {glm::vec3(0.5f, 0.5f, 1.f),
                     glm::vec3(-20.5f, 0.5f, 1.f),
                     glm::vec3(-20.5f, -0.5f, 1.f),
                     glm::vec3(0.5f, -0.5f, 1.f)}),

        Shoe_Left(this, {glm::vec3(-0.4f, -1.6f, 1.f),
                   glm::vec3(-1.4f, -1.6f, 1.f),
                   glm::vec3(-1.4f, -2.6f, 1.f),
                   glm::vec3(-0.4f, -2.6f, 1.f)}),
        Shoe_Right(this, {glm::vec3(-0.4f, -1.6f, 1.f),
                     glm::vec3(-1.4f, -1.6f, 1.f),
                     glm::vec3(-1.4f, -2.6f, 1.f),
                     glm::vec3(-0.4f, -2.6f, 1.f)}),

        Leg_RightUp(this, {glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(-1.f, 0.f, 1.f),
                    glm::vec3(-1.f, -1.f, 1.f),
                    glm::vec3(0.f, -1.f, 1.f)}),
        Leg_LeftUp(this, {glm::vec3(0.f, 0.f, 1.f),
                      glm::vec3(-1.f, 0.f, 1.f),
                      glm::vec3(-1.f, -1.f, 1.f),
                      glm::vec3(0.f, -1.f, 1.f)}),
        Leg_RightDown(this, {glm::vec3(0.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -1.7f, 1.f),
                        glm::vec3(0.f, -1.7f, 1.f)}),
        Leg_LeftDown(this, {glm::vec3(0.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -1.7f, 1.f),
                        glm::vec3(0.f, -1.7f, 1.f)}),

        Arm_RightUp(this, {glm::vec3(0.f, 0.f, 1.f),
                    glm::vec3(-1.f, 0.f, 1.f),
                    glm::vec3(-1.f, -1.f, 1.f),
                    glm::vec3(0.f, -1.f, 1.f)}),
        Arm_LeftUp(this, {glm::vec3(0.f, 0.f, 1.f),
                      glm::vec3(-1.f, 0.f, 1.f),
                      glm::vec3(-1.f, -1.f, 1.f),
                      glm::vec3(0.f, -1.f, 1.f)}),
        Arm_RightDown(this, {glm::vec3(0.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -1.7f, 1.f),
                        glm::vec3(0.f, -1.7f, 1.f)}),
        Arm_LeftDown(this, {glm::vec3(0.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -0.7f, 1.f),
                        glm::vec3(-1.f, -1.7f, 1.f),
                        glm::vec3(0.f, -1.7f, 1.f)}),

        m_torso(this, {glm::vec3(0.5f, 0.5f, 1.f),
                   glm::vec3(0.9f, 0.5f, 1.f),
                   glm::vec3(0.9f, -0.8f, 1.f),
                   glm::vec3(0.5f, -0.8f, 1.f)}),
        m_neck(this, {glm::vec3(0.5f, 0.5f, 1.f),
                    glm::vec3(0.75f, 0.5f, 1.f),
                    glm::vec3(0.75f, 0.3f, 1.f),
                    glm::vec3(0.5f, 0.3f, 1.f)}),
        m_head(this, 16),
        m_fistR(this, 4),
        m_fistL(this, 4),
        m_hair(this, 20),
#else
    m_floor(this, {glm::vec3(0.5f, 0.5f, 1.f),
                   glm::vec3(-20.5f, 0.5f, 1.f),
                   glm::vec3(-20.5f, -0.5f, 1.f),
                   glm::vec3(0.5f, -0.5f, 1.f)}),

    Shoe_Left(this, {glm::vec3(0.5f, 0.5f, 1.f),
                     glm::vec3(-0.5f, 0.5f, 1.f),
                     glm::vec3(-0.75f, 0.25f, 1.f),
                     glm::vec3(-0.75f, -0.25f, 1.f),
                     glm::vec3(-0.5f, -0.5f, 1.f),
                     glm::vec3(0.5f, -0.5f, 1.f)}),
    Shoe_Right(this, {  glm::vec3(0.5f, 0.5f, 1.f),
                        glm::vec3(-0.5f, 0.5f, 1.f),
                        glm::vec3(-0.75f, 0.25f, 1.f),
                        glm::vec3(-0.75f, -0.25f, 1.f),
                        glm::vec3(-0.5f, -0.5f, 1.f),
                        glm::vec3(0.5f, -0.5f, 1.f)}),

    // Shoe_Left(this, 10),
    // Shoe_Right(this, 8),

    Leg_RightUp(this, {glm::vec3(0.5f, 0.5f, 1.f),
                       glm::vec3(0.25f, 0.75f, 1.f),
                       glm::vec3(-0.25f, 0.75f, 1.f),
                       glm::vec3(-0.5f, 0.5f, 1.f),
                       glm::vec3(-0.5f, -0.5f, 1.f),
                       glm::vec3(-0.25f, -0.75f, 1.f),
                       glm::vec3(0.25f, -0.75f, 1.f),
                       glm::vec3(0.5f, -0.5f, 1.f)}),
    // Leg_RightUp(this, 4),
    Leg_LeftUp(this, {glm::vec3(0.5f, 0.5f, 1.f),
                      glm::vec3(0.25f, 0.75f, 1.f),
                      glm::vec3(-0.25f, 0.75f, 1.f),
                      glm::vec3(-0.5f, 0.5f, 1.f),
                      glm::vec3(-0.5f, -0.5f, 1.f),
                      glm::vec3(-0.25f, -0.75f, 1.f),
                      glm::vec3(0.25f, -0.75f, 1.f),
                      glm::vec3(0.5f, -0.5f, 1.f)}),
    Leg_RightDown(this, {glm::vec3(0.5f, 0.5f, 1.f),
                           glm::vec3(0.25f, 0.75f, 1.f),
                           glm::vec3(-0.25f, 0.75f, 1.f),
                           glm::vec3(-0.5f, 0.5f, 1.f),
                           glm::vec3(-0.5f, -0.5f, 1.f),
                           glm::vec3(-0.25f, -0.75f, 1.f),
                           glm::vec3(0.25f, -0.75f, 1.f),
                           glm::vec3(0.5f, -0.5f, 1.f)}),
    // Leg_RightDown(this, 20),
    Leg_LeftDown(this, {glm::vec3(0.5f, 0.5f, 1.f),
                        glm::vec3(0.25f, 0.75f, 1.f),
                        glm::vec3(-0.25f, 0.75f, 1.f),
                        glm::vec3(-0.5f, 0.5f, 1.f),
                        glm::vec3(-0.5f, -0.5f, 1.f),
                        glm::vec3(-0.25f, -0.75f, 1.f),
                        glm::vec3(0.25f, -0.75f, 1.f),
                        glm::vec3(0.5f, -0.5f, 1.f)}),

    Arm_RightUp(this, {glm::vec3(0.5f, 0.5f, 1.f),
                       glm::vec3(0.25f, 0.75f, 1.f),
                       glm::vec3(-0.25f, 0.75f, 1.f),
                       glm::vec3(-0.5f, 0.5f, 1.f),
                       glm::vec3(-0.5f, -0.5f, 1.f),
                       glm::vec3(-0.25f, -0.75f, 1.f),
                       glm::vec3(0.25f, -0.75f, 1.f),
                       glm::vec3(0.5f, -0.5f, 1.f)}),
    Arm_LeftUp(this, {glm::vec3(0.5f, 0.5f, 1.f),
                        glm::vec3(0.25f, 0.75f, 1.f),
                        glm::vec3(-0.25f, 0.75f, 1.f),
                        glm::vec3(-0.5f, 0.5f, 1.f),
                        glm::vec3(-0.5f, -0.5f, 1.f),
                        glm::vec3(-0.25f, -0.75f, 1.f),
                        glm::vec3(0.25f, -0.75f, 1.f),
                        glm::vec3(0.5f, -0.5f, 1.f)}),
    Arm_RightDown(this, {glm::vec3(0.5f, 0.5f, 1.f),
                           glm::vec3(0.25f, 0.75f, 1.f),
                           glm::vec3(-0.25f, 0.75f, 1.f),
                           glm::vec3(-0.5f, 0.5f, 1.f),
                           glm::vec3(-0.5f, -0.5f, 1.f),
                           glm::vec3(-0.25f, -0.75f, 1.f),
                           glm::vec3(0.25f, -0.75f, 1.f),
                           glm::vec3(0.5f, -0.5f, 1.f)}),
    Arm_LeftDown(this, {glm::vec3(0.5f, 0.5f, 1.f),
                          glm::vec3(0.25f, 0.75f, 1.f),
                          glm::vec3(-0.25f, 0.75f, 1.f),
                          glm::vec3(-0.5f, 0.5f, 1.f),
                          glm::vec3(-0.5f, -0.5f, 1.f),
                          glm::vec3(-0.25f, -0.75f, 1.f),
                          glm::vec3(0.25f, -0.75f, 1.f),
                          glm::vec3(0.5f, -0.5f, 1.f)}),

    // m_torso(this, {glm::vec3(0.5f, 0.5f, 1.f),
    //                glm::vec3(-0.5f, 0.5f, 1.f),
    //                glm::vec3(-0.5f, -0.5f, 1.f),
    //                glm::vec3(0.5f, -0.5f, 1.f)}),
    m_torso(this, 16),
    m_neck(this, {glm::vec3(0.5f, 0.5f, 1.f),
                    glm::vec3(-0.5f, 0.5f, 1.f),
                    glm::vec3(-0.5f, -0.5f, 1.f),
                    glm::vec3(0.5f, -0.5f, 1.f)}),
    m_head(this, 16),
    m_fistR(this, 4),
    m_fistL(this, 4),
    m_hair(this, 20),
#endif
        m_showGrid(true),
        mp_selectedNode(nullptr),
    root()
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();

    glDeleteVertexArrays(1, &vao);
    m_cloud.destroy();
    m_geomGrid.destroy();
    m_floor.destroy();
    Shoe_Left.destroy();
    Shoe_Right.destroy();
    Leg_RightUp.destroy();
    Leg_LeftUp.destroy();
    Leg_RightDown.destroy();
    Leg_LeftDown.destroy();

    Arm_RightUp.destroy();
    Arm_LeftUp.destroy();
    Arm_RightDown.destroy();
    Arm_LeftDown.destroy();

    m_torso.destroy();
    m_neck.destroy();
    m_head.destroy();
    m_fistR.destroy();
    m_fistL.destroy();
    m_hair.destroy();

}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(BG_RED_COMP, BG_GREEN_COMP, BG_BLUE_COMP, BG_ALPHA_COMP);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the scene geometry
    m_geomGrid.create();
    m_cloud.create();
    m_floor.create();
    Shoe_Left.create();
    Shoe_Right.create();
    Leg_RightUp.create();
    Leg_LeftUp.create();
    Leg_RightDown.create();
    Leg_LeftDown.create();

    Arm_RightUp.create();
    Arm_LeftUp.create();
    Arm_RightDown.create();
    Arm_LeftDown.create();

    m_torso.create();
    m_neck.create();
    m_head.create();
    m_fistR.create();
    m_fistL.create();

    m_hair.create();

    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);


    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::scale(glm::mat3(), glm::vec2(2.f, 1.3f)) * glm::translate(glm::mat3(), glm::vec2((float)(((float)TranslateSceneX + 2.f)/2), 2.5f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 1.f + 2.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 1.f + 2.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f + 2.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f + 2.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f + 2.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f + 2.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    // Cloud 2

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::scale(glm::mat3(), glm::vec2(2.f, 1.3f)) * glm::translate(glm::mat3(), glm::vec2((float)(((float)TranslateSceneX - 3.f)/2), 2.5f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 1.f - 3.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 1.f - 3.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f - 3.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f - 3.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f - 3.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f - 3.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    // Floor
    m_floor.setColor(glm::vec3(FLOOR_RED_COMP, FLOOR_GREEN_COMP, FLOOR_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f, -4.5f)) * glm::scale(glm::mat3(), glm::vec2(25.f, 1.5f)));
    prog_flat.draw(*this, m_floor);

    // Right Arm

    Root = std::make_unique<TranslateNode>();
    Body = std::make_unique<TranslateNode>();

    UpperArmRight = std::make_unique<TranslateNode>();
    LowerArmRight = std::make_unique<TranslateNode>();
    UpperArmLeft = std::make_unique<TranslateNode>();
    LowerArmLeft = std::make_unique<TranslateNode>();
    UpperLegRight = std::make_unique<TranslateNode>();
    LowerLegRight = std::make_unique<TranslateNode>();
    UpperLegLeft = std::make_unique<TranslateNode>();
    LowerLegLeft = std::make_unique<TranslateNode>();

    // cloud->Node_SetName("Cloud");
    // Body->Node_AddChild(std::move (body));

    //---------------------------------------------------------------------------------------------
    //  Right Arm Up
    //---------------------------------------------------------------------------------------------

    ScaleNode Scale_Arm_RightUp(0.2f, 0.6f);
    TranslateNode Translate_Arm_RightUp1(0.f, -0.4f);
    RotateNode Rotate_Arm_RightUp((float)(Rotate_LegUp));
    TranslateNode Translate_Arm_RightUpPosition(TranslateSceneX + 2.65f, -1.f);

    // uPtr<Node> Node_Arm_RightUp = std::make_unique<TranslateNode>();
    UpperArmRight->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    UpperArmRight->Node_SetPoly(&Arm_RightUp);
    UpperArmRight->Node_SetTransform(Translate_Arm_RightUpPosition.Node_Transformation() *
                                     Rotate_Arm_RightUp.Node_Transformation() *
                                     Translate_Arm_RightUp1.Node_Transformation() *
                                     Scale_Arm_RightUp.Node_Transformation());

    // Body->Node_AddChild(std::move(UpperArmRight));

    // Arm_RightUp.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    // // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.5f)) * glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.2f, 0.7f)));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -1.f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.2f, 0.6f)));
    // prog_flat.draw(*this, Arm_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Arm Down
    //---------------------------------------------------------------------------------------------

    ScaleNode Scale_Arm_RightDown(0.2f, 0.6f);
    TranslateNode Translate_Arm_RightDown1(0.f, -0.35f);
    RotateNode Rotate_Arm_RightDown1((float)(Rotate_LegDown));
    TranslateNode Translate_Arm_RightDown2(0.f, -0.7f);
    RotateNode Rotate_Arm_RightDown2((float)(Rotate_LegUp));
    TranslateNode Translate_Arm_RightDownPosition(TranslateSceneX + 2.65f, -1.f);

    LowerArmRight->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    LowerArmRight->Node_SetPoly(&Arm_RightDown);
    LowerArmRight->Node_SetTransform(Translate_Arm_RightDownPosition.Node_Transformation() *
                                     Rotate_Arm_RightDown2.Node_Transformation() *
                                     Translate_Arm_RightDown2.Node_Transformation() *
                                     Rotate_Arm_RightDown1.Node_Transformation() *
                                     Translate_Arm_RightDown1.Node_Transformation() *
                                     Scale_Arm_RightDown.Node_Transformation());

    UpperArmRight->Node_AddChild(std::move(LowerArmRight));

    // Arm_RightUp.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    // // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.55f)) * glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.2f, 0.7f)));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -1.f)) *
    //                          // glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
    //                          // glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegDown))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.2f, 0.6f)));
    // prog_flat.draw(*this, Arm_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Leg Up
    //---------------------------------------------------------------------------------------------

    ScaleNode Scale_Leg_RightUp(0.3f, 0.7f);
    TranslateNode Translate_Leg_RightUp1(0.f, -0.4f);
    RotateNode Rotate_Leg_RightUp((float)(-Rotate_LegUp));
    TranslateNode Translate_Leg_RightUpPosition(TranslateSceneX + 2.65f, -2.2f);

    UpperLegRight->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    UpperLegRight->Node_SetPoly(&Leg_RightUp);
    UpperLegRight->Node_SetTransform(Translate_Leg_RightUpPosition.Node_Transformation() *
                                     Rotate_Leg_RightUp.Node_Transformation() *
                                     Translate_Leg_RightUp1.Node_Transformation() *
                                     Scale_Leg_RightUp.Node_Transformation());

    // Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    // // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.45f)) * glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    // prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Leg Down
    //---------------------------------------------------------------------------------------------
    ScaleNode Scale_Leg_RightDown(0.3f, 0.7f);
    TranslateNode Translate_Leg_RightDown1(0.f, -0.35f);
    RotateNode Rotate_Leg_RightDown1((float)(30 - Rotate_LegDown));
    TranslateNode Translate_Leg_RightDown2(0.f, -0.7f);
    RotateNode Rotate_Leg_RightDown2((float)(-Rotate_LegUp));
    TranslateNode Translate_Leg_RightDownPosition(TranslateSceneX + 2.65f, -2.2f);

    LowerLegRight->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    LowerLegRight->Node_SetPoly(&Leg_RightDown);
    LowerLegRight->Node_SetTransform(Translate_Leg_RightDownPosition.Node_Transformation() *
                                     Rotate_Leg_RightDown2.Node_Transformation() *
                                     Translate_Leg_RightDown2.Node_Transformation() *
                                     Rotate_Leg_RightDown1.Node_Transformation() *
                                     Translate_Leg_RightDown1.Node_Transformation() *
                                     Scale_Leg_RightDown.Node_Transformation());

    UpperLegRight->Node_AddChild(std::move(LowerLegRight));

    // Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
    //                          // glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
    //                          // glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(30-Rotate_LegDown))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    // prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Shoe
    //---------------------------------------------------------------------------------------------
    // Shoe_Right.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.55f, -2.45f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(30-Rotate_LegDown))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.15f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.6f, 0.3f))/* *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp)))*/);
    // prog_flat.draw(*this, Shoe_Right);

    //---------------------------------------------------------------------------------------------
    //  Left Leg Up
    //---------------------------------------------------------------------------------------------
    ScaleNode Scale_Leg_LeftUp(0.3f, 0.7f);
    TranslateNode Translate_Leg_LeftUp1(0.f, -0.4f);
    RotateNode Rotate_Leg_LeftUp((float)(Rotate_LegUp));
    TranslateNode Translate_Leg_LeftUpPosition(TranslateSceneX + 2.65f, -2.2f);

    UpperLegLeft->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    UpperLegLeft->Node_SetPoly(&Leg_LeftUp);
    UpperLegLeft->Node_SetTransform(Translate_Leg_LeftUpPosition.Node_Transformation() *
                                    Rotate_Leg_LeftUp.Node_Transformation() *
                                    Translate_Leg_LeftUp1.Node_Transformation() *
                                    Scale_Leg_LeftUp.Node_Transformation());

    // Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    // // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.45f)) * glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    // prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Left Leg Down
    //---------------------------------------------------------------------------------------------
    ScaleNode Scale_Leg_LeftDown(0.3f, 0.7f);
    TranslateNode Translate_Leg_LeftDown1(0.f, -0.35f);
    RotateNode Rotate_Leg_LeftDown1((float)(30 - Rotate_LegDown));
    TranslateNode Translate_Leg_LeftDown2(0.f, -0.7f);
    RotateNode Rotate_Leg_LeftDown2((float)(-Rotate_LegUp));
    TranslateNode Translate_Leg_LeftDownPosition(TranslateSceneX + 2.65f, -2.2f);

    LowerLegLeft->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    LowerLegLeft->Node_SetPoly(&Leg_LeftDown);
    LowerLegLeft->Node_SetTransform(Translate_Leg_LeftDownPosition.Node_Transformation() *
                                    Rotate_Leg_LeftDown2.Node_Transformation() *
                                    Translate_Leg_LeftDown2.Node_Transformation() *
                                    Rotate_Leg_LeftDown1.Node_Transformation() *
                                    Translate_Leg_LeftDown1.Node_Transformation() *
                                    Scale_Leg_LeftDown.Node_Transformation());

    UpperLegLeft->Node_AddChild(std::move(LowerLegLeft));

    // Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
    //                          // glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
    //                          // glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegDown))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    // prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Left Shoe
    //---------------------------------------------------------------------------------------------
    // Shoe_Right.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.55f, -2.45f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegDown))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.15f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.6f, 0.3f))/* *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp)))*/);
    // prog_flat.draw(*this, Shoe_Right);

    //---------------------------------------------------------------------------------------------
    //  Torso
    //---------------------------------------------------------------------------------------------
    m_torso.setColor(glm::vec3(TORSO_RED_COMP, TORSO_GREEN_COMP, TORSO_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.6f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.6f, 1.4f)));
    prog_flat.draw(*this, m_torso);

    //---------------------------------------------------------------------------------------------
    //  Neck
    //---------------------------------------------------------------------------------------------
    m_neck.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -0.8f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.25f, 0.2f)));
    prog_flat.draw(*this, m_neck);

    //---------------------------------------------------------------------------------------------
    //  Head
    //---------------------------------------------------------------------------------------------
    m_head.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.57f, -0.33f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.65f, 0.8f)));
    prog_flat.draw(*this, m_head);

    //---------------------------------------------------------------------------------------------
    //  Hair
    //---------------------------------------------------------------------------------------------
    m_hair.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.55f, -0.05f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.7f, 0.3f)));
    prog_flat.draw(*this, m_hair);

    m_hair.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.8f, -0.33f)) *
                             glm::rotate(glm::mat3(), glm::radians(90.f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.7f, 0.2f)));
    prog_flat.draw(*this, m_hair);

    //---------------------------------------------------------------------------------------------
    //  Eye
    //---------------------------------------------------------------------------------------------
    m_hair.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.4f, -0.33f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.1f, 0.1f)));
    prog_flat.draw(*this, m_hair);

    //---------------------------------------------------------------------------------------------
    //  Nose
    //---------------------------------------------------------------------------------------------
    m_hair.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.27f, -0.4f)) *
                             glm::rotate(glm::mat3(), glm::radians(-10.f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.1f, 0.2f)));
    prog_flat.draw(*this, m_hair);

    //---------------------------------------------------------------------------------------------
    //  Left Arm Up
    //---------------------------------------------------------------------------------------------
    ScaleNode Scale_Arm_LeftUp(0.2f, 0.6f);
    TranslateNode Translate_Arm_LeftUp1(0.f, -0.4f);
    RotateNode Rotate_Arm_LeftUp((float)(Rotate_LegUp));
    TranslateNode Translate_Arm_LeftUpPosition(TranslateSceneX + 2.65f, -1.f);

    // uPtr<Node> Node_Arm_RightUp = std::make_unique<TranslateNode>();
    UpperArmLeft->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    UpperArmLeft->Node_SetPoly(&Arm_LeftUp);
    UpperArmLeft->Node_SetTransform(Translate_Arm_LeftUpPosition.Node_Transformation() *
                                    Rotate_Arm_LeftUp.Node_Transformation() *
                                    Translate_Arm_LeftUp1.Node_Transformation() *
                                    Scale_Arm_LeftUp.Node_Transformation());

    // Arm_RightUp.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    // // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.5f)) * glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.2f, 0.7f)));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -1.f)) *
    //                          glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
    //                          glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
    //                          glm::scale(glm::mat3(), glm::vec2(0.2f, 0.6f)));
    // prog_flat.draw(*this, Arm_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Left Arm Down
    //---------------------------------------------------------------------------------------------

    ScaleNode Scale_Arm_LeftDown(0.2f, 0.6f);
    TranslateNode Translate_Arm_LeftDown1(0.f, -0.35f);
    RotateNode Rotate_Arm_LeftDown1((float)(Rotate_LegDown));
    TranslateNode Translate_Arm_LeftDown2(0.f, -0.7f);
    RotateNode Rotate_Arm_LeftDown2((float)(Rotate_LegUp));
    TranslateNode Translate_Arm_LeftDownPosition(TranslateSceneX + 2.65f, -1.f);

    LowerArmLeft->Node_SetRGB(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    LowerArmLeft->Node_SetPoly(&Arm_LeftDown);
    LowerArmLeft->Node_SetTransform(Translate_Arm_LeftDownPosition.Node_Transformation() *
                                    Rotate_Arm_LeftDown2.Node_Transformation() *
                                    Translate_Arm_LeftDown2.Node_Transformation() *
                                    Rotate_Arm_LeftDown1.Node_Transformation() *
                                    Translate_Arm_LeftDown1.Node_Transformation() *
                                    Scale_Arm_LeftDown.Node_Transformation());

    UpperArmLeft->Node_AddChild(std::move(LowerArmLeft));

    Body->Node_AddChild(std::move(UpperArmLeft));
    Body->Node_AddChild(std::move(UpperArmRight));
    Body->Node_AddChild(std::move(UpperLegLeft));
    Body->Node_AddChild(std::move(UpperLegRight));

    TranslateNode RootTranslate(0, 0);
    RotateNode RootRotate(0);
    ScaleNode RootScale(1, 1);
    Root->Node_SetTransform(RootTranslate.Node_Transformation() *
                            RootRotate.Node_Transformation() *
                            RootScale.Node_Transformation());

    Root->Node_AddChild(std::move(Body));

    // emit sig_scenegraph(stdthis->Root);

}

void MyGL::resizeGL(int w, int h)
{
    glm::mat3 viewMat = glm::scale(glm::mat3(), glm::vec2(0.2, 0.2)); // Screen is -5 to 5

    // Upload the view matrix to our shader (i.e. onto the graphics card)
    prog_flat.setViewMatrix(viewMat);

    printGLErrorLog();
}

// This function is called by Qt any time your GL window is supposed to update
// For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_showGrid)
    {
        prog_flat.setModelMatrix(glm::mat3());
        prog_flat.draw(*this, m_geomGrid);
    }

    //VVV CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL VVV///////////////////

    // Shapes will be drawn on top of one another, so the last object
    // drawn will appear in front of everything else

    // prog_flat.setModelMatrix(glm::mat3());
    // m_geomSquare.setColor(glm::vec3(0,1,0));
    // prog_flat.draw(*this, m_geomSquare);

    // m_geomSquare.setColor(glm::vec3(1,0,0));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(-1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(-30.f)));
    // prog_flat.draw(*this, m_geomSquare);

    // m_geomSquare.setColor(glm::vec3(0,0,1));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(1.f, 0.f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    // prog_flat.draw(*this, m_geomSquare);

    // uPtr<Node> root(new TranslateNode(0, 0));

    // Cloud

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::scale(glm::mat3(), glm::vec2(2.f, 1.3f)) * glm::translate(glm::mat3(), glm::vec2((float)(((float)TranslateSceneX + 2.f)/2), 2.5f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 1.f + 2.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 1.f + 2.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f + 2.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f + 2.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f + 2.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f + 2.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    // Cloud 2

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::scale(glm::mat3(), glm::vec2(2.f, 1.3f)) * glm::translate(glm::mat3(), glm::vec2((float)(((float)TranslateSceneX - 3.f)/2), 2.5f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 1.f - 3.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 1.f - 3.f, 3.2f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f - 3.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f - 3.f, 3.6f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX - 0.4f - 3.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    m_cloud.setColor(glm::vec3(1,1,1));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f - 3.f, 2.8f)) * glm::rotate(glm::mat3(), glm::radians(30.f)));
    prog_flat.draw(*this, m_cloud);

    // Floor
    m_floor.setColor(glm::vec3(FLOOR_RED_COMP, FLOOR_GREEN_COMP, FLOOR_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 0.4f, -4.5f)) * glm::scale(glm::mat3(), glm::vec2(25.f, 1.5f)));
    prog_flat.draw(*this, m_floor);

    //---------------------------------------------------------------------------------------------
    //  Right Arm Up
    //---------------------------------------------------------------------------------------------

    Arm_RightUp.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.5f)) * glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.2f, 0.7f)));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -1.f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.2f, 0.6f)));
    prog_flat.draw(*this, Arm_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Arm Down
    //---------------------------------------------------------------------------------------------

    Arm_RightUp.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.55f)) * glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.2f, 0.7f)));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -1.f)) *
                             // glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             // glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegDown))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.2f, 0.6f)));
    prog_flat.draw(*this, Arm_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Leg Up
    //---------------------------------------------------------------------------------------------

    Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.45f)) * glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Leg Down
    //---------------------------------------------------------------------------------------------
    Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
                             // glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             // glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(30-Rotate_LegDown))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Right Shoe
    //---------------------------------------------------------------------------------------------
    Shoe_Right.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.55f, -2.45f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(30-Rotate_LegDown))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.15f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.6f, 0.3f))/* *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp)))*/);
    prog_flat.draw(*this, Shoe_Right);

    //---------------------------------------------------------------------------------------------
    //  Left Leg Up
    //---------------------------------------------------------------------------------------------
    Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.45f)) * glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Left Leg Down
    //---------------------------------------------------------------------------------------------
    Leg_RightUp.setColor(glm::vec3(LEG_RED_COMP, LEG_GREEN_COMP, LEG_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -2.2f)) *
                             // glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             // glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegDown))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.3f, 0.7f)));
    prog_flat.draw(*this, Leg_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Left Shoe
    //---------------------------------------------------------------------------------------------
    Shoe_Right.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.55f, -2.45f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegDown))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.15f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.6f, 0.3f))/* *
                             glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp)))*/);
    prog_flat.draw(*this, Shoe_Right);

    //---------------------------------------------------------------------------------------------
    //  Torso
    //---------------------------------------------------------------------------------------------
    m_torso.setColor(glm::vec3(TORSO_RED_COMP, TORSO_GREEN_COMP, TORSO_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.6f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.6f, 1.4f)));
    prog_flat.draw(*this, m_torso);

    //---------------------------------------------------------------------------------------------
    //  Neck
    //---------------------------------------------------------------------------------------------
    m_neck.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -0.8f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.25f, 0.2f)));
    prog_flat.draw(*this, m_neck);

    //---------------------------------------------------------------------------------------------
    //  Head
    //---------------------------------------------------------------------------------------------
    m_head.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.57f, -0.33f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.65f, 0.8f)));
    prog_flat.draw(*this, m_head);

    //---------------------------------------------------------------------------------------------
    //  Hair
    //---------------------------------------------------------------------------------------------
    m_hair.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.55f, -0.05f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.7f, 0.3f)));
    prog_flat.draw(*this, m_hair);

    m_hair.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.8f, -0.33f)) *
                             glm::rotate(glm::mat3(), glm::radians(90.f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.7f, 0.2f)));
    prog_flat.draw(*this, m_hair);

    //---------------------------------------------------------------------------------------------
    //  Eye
    //---------------------------------------------------------------------------------------------
    m_hair.setColor(glm::vec3(SHOE_RED_COMP, SHOE_GREEN_COMP, SHOE_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.4f, -0.33f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.1f, 0.1f)));
    prog_flat.draw(*this, m_hair);

    //---------------------------------------------------------------------------------------------
    //  Nose
    //---------------------------------------------------------------------------------------------
    m_hair.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.27f, -0.4f)) *
                             glm::rotate(glm::mat3(), glm::radians(-10.f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.1f, 0.2f)));
    prog_flat.draw(*this, m_hair);

    //---------------------------------------------------------------------------------------------
    //  Left Arm Up
    //---------------------------------------------------------------------------------------------
    Arm_RightUp.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.5f)) * glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.2f, 0.7f)));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -1.f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.4f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.2f, 0.6f)));
    prog_flat.draw(*this, Arm_RightUp);

    //---------------------------------------------------------------------------------------------
    //  Left Arm Down
    //---------------------------------------------------------------------------------------------
    Arm_RightUp.setColor(glm::vec3(SKIN_RED_COMP, SKIN_GREEN_COMP, SKIN_BLUE_COMP));
    // prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.6f, -1.55f)) * glm::rotate(glm::mat3(), glm::radians((float)(Rotate_LegUp))) * glm::scale(glm::mat3(), glm::vec2(0.2f, 0.7f)));
    prog_flat.setModelMatrix(glm::translate(glm::mat3(), glm::vec2(TranslateSceneX + 2.65f, -1.f)) *
                             // glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             // glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegUp))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.7f)) *
                             glm::rotate(glm::mat3(), glm::radians((float)(-Rotate_LegDown))) *
                             glm::translate(glm::mat3(), glm::vec2(0.f, -0.35f)) *
                             glm::scale(glm::mat3(), glm::vec2(0.2f, 0.6f)));
    prog_flat.draw(*this, Arm_RightUp);

    //^^^ CLEAR THIS CODE WHEN YOU IMPLEMENT SCENE GRAPH TRAVERSAL ^^^/////////////////

    // traverse(std::move(Root), Root->Node_GetTransform(), prog_flat);

    // Here is a good spot to call your scene graph traversal function.
    // Any time you want to draw an instance of geometry, call
    // prog_flat.draw(*this, yourNonPointerGeometry);
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    switch(e->key())
    {
    case(Qt::Key_Escape):
        QApplication::quit();
        break;

    case(Qt::Key_G):
        m_showGrid = !m_showGrid;
        break;
    }
}

void MyGL::slot_setSelectedNode(QTreeWidgetItem *i) {
    mp_selectedNode = static_cast<Node*>(i);
}

void MyGL::slot_setTX(double x) {
    // TODO update the currently selected Node's
    // X translation value IF AND ONLY IF
    // the currently selected node can be validly
    // dynamic_cast to a TranslateNode.
    // Remember that a failed dynamic_cast
    // will return a null pointer.
}

void MyGL::slot_addTranslateNode() {
    // TODO invoke the currently selected Node's
    // addChild function on a newly-instantiated
    // TranslateNode.
}

void MyGL::slot_addTranslateXNode(int i) {
    // emit sig_TX(i);
    TranslateSceneX = i;
    update();
}

void MyGL::slot_addRotateArmUpNode(float angle) {
    // emit sig_TX(i);
    rotateArmUp = angle;
    update();
}

void MyGL::slot_addRotateLegUp(int i) {
    // emit sig_TX(i);
    Rotate_LegUp = i;
    update();
}

void MyGL::slot_addRotateLegDown(int i) {
    // emit sig_TX(i);
    Rotate_LegDown = i;
    update();
}

Node * ConstructSceneGraph(Node * root) {
    // Node root;
    // Limbs
    // Head
    // Torso
    return root;
}

void MyGL::traverse(uPtr<Node> N, glm::mat3 T, ShaderProgram shader) {
    // N->traverse(std::move(N), T, shader);
    // T = T * N->Node_GetTransform();
    // for (int i = 0; i < N->Node_GetChildrenSize(); i++) {
    //     traverse(N->Node_GetChild(i), T, shader);
    // }

    T = T * N->Node_GetTransform();
    for (int i = 0; i < N->Node_GetChildrenSize(); i++) {
        traverse(N->Node_GetChild(i), T, shader);
    }

    if (N->Node_GetPoly() != NULL) {
        shader.setModelMatrix(T);
        // shader.setGeometryColor(N->Node_GetRGB());
        // shader.draw(*(N->Node_GetPoly()));
        shader.draw(*this, *(N->Node_GetPoly()));
    }
}
