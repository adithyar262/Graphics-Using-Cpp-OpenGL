/**************************************************************************//**
* @file      Transformation.cpp
* @brief     Scene Transformation
* @author    Adi
* @date      2024-2-4

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
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
/****************
* Translate
****************/
TranslateNode::TranslateNode() /*:
    Node()*/
{
    TranslateNode(0, 0);
}

TranslateNode::TranslateNode(float x, float y) :
    TranslateX(x),
    TranslateY(y)
{
    TranslateMat = glm::mat3{{1,0,0}, {0,1,0}, {x,y,1}};
}

TranslateNode::~TranslateNode()
{}

glm::mat3 TranslateNode::Node_Transformation(void) {
    return TranslateMat;
}

/****************
* Rotate
****************/
RotateNode::RotateNode(float angle1) :
    angle(angle1)
{
    RotateMat = glm::mat3{{cos(angle1), -sin(angle1),0}, {sin(angle1), cos(angle1),0}, {0,0,1}};
}

RotateNode::~RotateNode()
{}

glm::mat3 RotateNode::Node_Transformation(void) {
    return RotateMat;
}

/****************
* Scale
****************/
ScaleNode::ScaleNode(float x, float y) :
    ScaleX(x),
    ScaleY(y)
{
    ScaleMat = glm::mat3{{x,0,0}, {0,y,0}, {0,0,1}};
}

ScaleNode::~ScaleNode()
{}

glm::mat3 ScaleNode::Node_Transformation(void) {
    return ScaleMat;
}
