/**************************************************************************//**
* @file      Mesh.cpp
* @brief     Mesh classes and implementation
* @author    Adi
* @date      2024-3-13

******************************************************************************/

#ifndef MESH_H
#define MESH_H

/******************************************************************************
* Includes
******************************************************************************/
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <utils.h>
#include "drawable.h"
#include "scene/Polygon.h"
#include <QListWidgetItem>

using namespace std;
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/

typedef enum {
    MESH_FACE,
    MESH_EDGE,
    MESH_VERTEX
} MeshComponent_t;
/******************************************************************************
* Function Prototypes
******************************************************************************/

/******************************************************************************
* Classes
******************************************************************************/
class HalfEdge;
class Mesh;

// A hash function used to hash a pair of any kind
// https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }

        // If hash1 == hash2, their XOR is zero.
        return hash1;
    }
};

class Vertex : public QListWidgetItem {
    glm::vec3 position;
    HalfEdge * edge;
    std::unique_ptr<int> idx;
    glm::vec3 nor;
public:
    friend class HalfEdge;
    friend class Mesh;
    friend class MyGL;
    static int count;
    Vertex();
    Vertex(glm::vec3 pos);
    void operator=(const Vertex &v);
    static void setCount(int count);
    static int getCount(void);
    void setHalfEdge(HalfEdge* edge);
    void setPosition(glm::vec3 pos);
};

class Face : public QListWidgetItem {
    glm::vec3 color;
    HalfEdge * edge;
    // unsigned int vertices;
    std::vector<unsigned int> vertices;
    std::unique_ptr<int> idx;
public:
    friend class HalfEdge;
    friend class Mesh;
    friend class MyGL;
    Face();
    void operator=(const Face &f);
    void setHalfEdge(HalfEdge* edge);
    void setColor(glm::vec3 color);
    unsigned int getVerticesCount(void);
    static void incCount(void);
    static int count;
    static void setCount(int count);
    static int getCount(void);
};

class HalfEdge : public QListWidgetItem {
    HalfEdge * next;
    HalfEdge * sym;
    Face * face;
    Vertex * vNext;
    std::unique_ptr<int> idx;
public:
    friend class Face;
    friend class Vertex;
    friend class Mesh;
    friend class MyGL;
    static int count;
    void operator=(const HalfEdge &h);
    // HalfEdge();
    HalfEdge(/*MyGL * myGL, */unsigned int vNext);
    HalfEdge(Mesh * mesh, unsigned int vNext, unsigned int faceIdx);
    HalfEdge(Mesh * mesh, Vertex * v, Face * f);
    void setVertex(Vertex * v);
    static void setCount(int count);
    static int getCount(void);
    void setFace(Face* face);
    void setNext(HalfEdge* next);
    void setSym(HalfEdge* sym);
};

class Mesh : public Drawable {
    std::vector<std::unique_ptr<Face>> face;
    std::vector<std::unique_ptr<HalfEdge>> halfEdge;
    std::vector<std::unique_ptr<Vertex>> vertex;
    class Polygon m_face;
    int currFace;
    int colorR;
    int colorG;
    int colorB;

    std::vector<glm::vec4> mesh_vert_col;
    std::vector<glm::vec4> mesh_vert_pos;
    std::vector<glm::vec4> mesh_vert_nor;
    // Sending the hash function as a third argument
    unordered_map<pair<uint, uint>, bool, hash_pair> vertexPair;
    unordered_map<int, glm::vec3> centroid;
    unordered_map<int, glm::vec3> edgeMid;
    unordered_map<int, glm::vec3> vertexSmooth;
    unordered_map<int, int> vertexCount;
    // unordered_map<pair<unsigned int, unsigned int>, unsigned int> endPoints;
public:
    friend class HalfEdge;
    friend class MyGL;
    Mesh(OpenGLContext *context);
    void initializeAndBufferGeometryData() override;
    void addVertex(float x, float y, float z);
    int addEdge(unsigned int faceIdx, unsigned int idx);
    unsigned int addFace();
    bool addPair(unsigned int first, unsigned int second);
    unsigned int getCompCount(unsigned int);
    bool computeCentroid(void);
    bool computeHalfEdge(void);
    bool computeVertices(void);
    class Polygon drawFace(OpenGLContext *context, unsigned int idx);
    void setCurrFace(int f);
    bool clearMesh(void);
    bool updateColor(int i);
    bool addVertexPair(const uint v1, const uint v2, const int halEdgeIdx);
    void catmullClark(void);
    void quarangulate(void);
    void redraw(void);
};

#endif // MESH_H
