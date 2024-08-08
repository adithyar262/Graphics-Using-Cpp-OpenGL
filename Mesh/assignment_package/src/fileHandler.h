/**************************************************************************//**
* @file      fileHandler.h
* @brief     File Handling functions
* @author    Adi
* @date      2024-3-13

******************************************************************************/

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

/******************************************************************************
* Includes
******************************************************************************/
#include <QFile>
#include "Mesh.h"
#include "mygl.h"
using namespace std;
/******************************************************************************
* Defines
******************************************************************************/
#define MAX_STR_LENGTH      100
#define OBJFILE_DELIMITER   "\r\n"
#define DATA_DELIMITER      " "

#define FILE_CMD_V          "v "
#define FILE_CMD_VT         "vt"
#define FILE_CMD_VN         "vn"
#define FILE_CMD_F          "f "
/******************************************************************************
* Variables
******************************************************************************/
typedef enum {
    CMD_V = 0,
    CMD_VT,
    CMD_VN,
    CMD_F
}CmdType_t;
/******************************************************************************
* Function Prototypes
******************************************************************************/

/******************************************************************************
* Classes
******************************************************************************/
class FileHandler {
    QFile file;
public:
    FileHandler();
    FileHandler(const QString &name);

    bool LoadObjFromFile(MyGL *mygl);
};

#endif // FILEHANDLER_H
