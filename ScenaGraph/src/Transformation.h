/**************************************************************************//**
* @file      Transformation.h
* @brief     Scene Transformation
* @author    Adi
* @date      2024-2-4

******************************************************************************/

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

/******************************************************************************
* Includes
******************************************************************************/
#include "scene/node.h"
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
* Classes
******************************************************************************/

class TranslateNode : public Node
{
    // Translate along x
    float TranslateX;
    // Translate along y
    float TranslateY;
    // Tranlation matrix
    glm::mat3 TranslateMat;
public:
    //Constructor
    TranslateNode();
    TranslateNode(float x, float y);
    // Destructor
    virtual ~TranslateNode();
    // Function that computes and returns a 3x3 homogeneous matrix representing the transformation in the node.
    virtual glm::mat3 Node_Transformation(void);
};

class RotateNode : public Node
{
    // Rotation angle
    float angle;
    // Rotation Matrix
    glm::mat3 RotateMat;
public:
    //Constructor
    RotateNode(float angle1);
    // Destructor
    virtual ~RotateNode();
    // Function that computes and returns a 3x3 homogeneous matrix representing the transformation in the node.
    virtual glm::mat3 Node_Transformation(void);
};


class ScaleNode : public Node
{
    // Scale along x
    float ScaleX;
    // Scale along y
    float ScaleY;
    // Scale matrix
    glm::mat3 ScaleMat;
public:
    //Constructor
    ScaleNode(float x, float y);
    // Destructor
    virtual ~ScaleNode();
    // Function that computes and returns a 3x3 homogeneous matrix representing the transformation in the node.
    virtual glm::mat3 Node_Transformation(void);
};


#endif // TRANSFORMATION_H
