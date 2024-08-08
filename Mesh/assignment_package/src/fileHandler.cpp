/**************************************************************************//**
* @file      fileHandler.h
* @brief     File Handling functions
* @author    Adi
* @date      2024-3-13

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include "fileHandler.h"
#include <iostream>
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

///----------------------------------------------------------------------
/// Constructors
///----------------------------------------------------------------------
FileHandler::FileHandler() {

}

FileHandler::FileHandler(const QString &name) :
    file(name)
{}

bool extractData(MyGL *mygl, QString cmd, unsigned int type) {
    // char* data = strtok(cmd, DATA_DELIMITER);
    QStringList  data = cmd.split(DATA_DELIMITER);
    switch(type) {
    case CMD_V : {
        float x = data[1].toFloat();
        float y = data[2].toFloat();
        float z = data[3].toFloat();
        mygl->addVertex(x, y, z, MESH_VERTEX);
        break;
    }
    // case CMD_VT : break;
    // case CMD_VN : break;
    case CMD_F : {
        unsigned int edges = data.length() - 1;
        bool flag = false;
        unsigned int faceIdx = mygl->addFace(MESH_FACE);
        // int firstVertex = data[1].split("/")[0].toDouble(&flag);
        std::vector<int> vertices;
        for(size_t i = 0; i < edges; ++i) {
            // int Vertex = data[i + 1].split("/")[0].toDouble(&flag);
            // int currEdgeIdx = mygl->addEdge(Vertex, faceIdx, MESH_EDGE);
            // if(i == 0) {
            //     mygl->addEdgeToFace(faceIdx, currEdgeIdx);
            // }
            int Vertex = data[i + 1].split("/")[0].toDouble(&flag);
            vertices.push_back(Vertex);
            // int currEdgeIdx = mygl->addEdge(Vertex, faceIdx, MESH_EDGE);
            // if(i == 0) {
            //     mygl->addEdgeToFace(faceIdx, currEdgeIdx);
            // }
        }
        mygl->addEdge(vertices, faceIdx, MESH_EDGE);
        // if(i == 0) {
        //     mygl->addEdgeToFace(faceIdx, currEdgeIdx);
        // }

        break;
#if 0
        unsigned int edges = data.length() - 1;
        bool flag = false;
        unsigned int faceIdx = mygl->addFace(MESH_FACE);
        // int firstVertex = data[1].split("/")[0].toDouble(&flag);
        for(size_t i = 0; i < edges; ++i) {
            int Vertex = data[i + 1].split("/")[0].toDouble(&flag);
            mygl->addEdge(Vertex, faceIdx, MESH_EDGE);
        }
        break;
#endif
    }
    default : break;
    }

    return true;
}

bool isValidCmd(MyGL *mygl, QString cmd) {
    if(cmd.length() < 2)
        return false;
    if(cmd.at(0) == 'v' && cmd.at(1) == ' ') {
        extractData(mygl, cmd, CMD_V);
        return true;
    }
    if(cmd.at(0) == 'v' && cmd.at(1) == 't') {
        // extractData(mesh, cmd, CMD_F);
        return true;
    }
    if(cmd.at(0) == 'v' && cmd.at(1) == 'n') {
        // extractData(mesh, cmd, CMD_F);
        return true;
    }
    if(cmd.at(0) == 'f' && cmd.at(1) == ' ') {
        extractData(mygl, cmd, CMD_F);
        return true;
    }

    // if(strncmp(cmd, FILE_CMD_V, 2) == 0) {
    //     extractData(cmd, CMD_V);
    //     return true;
    // }
    // if(strncmp(cmd, FILE_CMD_VT, 2) == 0) {
    //     extractData(cmd, CMD_VT);
    //     return true;
    // }
    // if(strncmp(cmd, FILE_CMD_VN, 2) == 0) {
    //     extractData(cmd, CMD_VN);
    //     return true;
    // }
    // if(strncmp(cmd, FILE_CMD_F, 2) == 0) {
    //     extractData(cmd, CMD_F);
    //     return true;
    // }

    return false;
}

bool FileHandler::LoadObjFromFile(MyGL *mygl) {
    if(file.open(QIODevice::ReadOnly)) {

        QString rawData = file.readAll();
        // std::cout<<"File read!"<<std::endl;

        // char* word = strtok((char *)rawData.data(), OBJFILE_DELIMITER);
        QStringList  word = rawData.split(OBJFILE_DELIMITER);

        mygl->clearMesh();

        foreach(QString element, word) {
            // std::cout<<"File read! "<<element.toStdString()<<std::endl;
            bool valid = isValidCmd(mygl, element);
        }
        // while (word)  {

        //     // print token
        //     std::cout<<"File read! "<<word<<std::endl;

        //     bool valid = isValidCmd(word);

        //     // QString a;

        //     std::cout<<"Valid Command -  "<<valid<<std::endl;

        //     // take subsequent tokens
        //     word = strtok(NULL, OBJFILE_DELIMITER);
        // }

        // std::cout<<"File read! "<<word<<std::endl;

        // word = strtok(NULL, OBJFILE_DELIMITER);
        // std::cout<<"File read! "<<word<<std::endl;

        file.close();

        // mygl->paintGL();
        mygl->updateBufferData();
        // mygl->update();

        return true;
    }
    return false;
}
