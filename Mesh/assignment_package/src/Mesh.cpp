/**************************************************************************//**
* @file      Mesh.cpp
* @brief     Mesh classes and implementation
* @author    Adi
* @date      2024-3-13

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include "Mesh.h"
#include <iostream>
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/
int Vertex::count = 0;
int HalfEdge::count = 0;
int Face::count = 0;
/******************************************************************************
* Function Prototypes
******************************************************************************/

/******************************************************************************
* Function Implementation
******************************************************************************/

///----------------------------------------------------------------------
/// Face
///----------------------------------------------------------------------
Face::Face()
{
    count++;
    idx = std::make_unique<int>(count);
    QString Qface = QString("Face")+QString().number(count);
    setText(Qface);
}

uint Face::getVerticesCount(void) {
    return vertices.size();
}

void Face::incCount(void) {
    count++;
    return;
}

void Face::setCount(int idx) {
    count = idx;
    return;
}

int Face::getCount(void) {
    return count;
}

void Face::operator=(const Face &f) {
    this->edge = f.edge;
    this->color = f.color;
    this->count = f.count;
}

void Face::setHalfEdge(HalfEdge* edge) {
    this->edge = edge;
}

void Face::setColor(glm::vec3 color) {
    this->color = color;
}
///----------------------------------------------------------------------
/// HalfEdge
///----------------------------------------------------------------------
HalfEdge::HalfEdge(uint ver)
{
    count++;
    idx = std::make_unique<int>(count);
    face = NULL;
    vNext - NULL;
    // vNext = mesh->vertex[ver].get();
    // std::unique_ptr<int> a = std::make_unique<int>(count);
}

HalfEdge::HalfEdge(Mesh * mesh, uint vNext, uint faceIdx) {
    count++;
    idx = std::make_unique<int>(count);
    face = mesh->face[faceIdx].get();
}

HalfEdge::HalfEdge(Mesh * mesh, Vertex * v, Face * f) {
    count++;
    idx = std::make_unique<int>(count);
    vNext = v;
    v->edge = this;
    face = f;
    sym = NULL;
    QString QEdge = QString("Edge")+QString().number(count);
    setText(QEdge);
    // face = mesh->face[faceIdx].get();

}

void HalfEdge::setVertex(Vertex * v) {
    this->vNext = v;
    v->edge = this;
    return;
}

void HalfEdge::setCount(int idx) {
    count = idx;
    return;
}

int HalfEdge::getCount(void) {
    return count;
}

void HalfEdge::operator=(const HalfEdge &h) {
    this->face = h.face;
    this->vNext = h.vNext;
    this->next = h.next;
    this->sym = h.sym;
    this->count = h.count;
}

void HalfEdge::setFace(Face* face) {
    this->face = face;
}

void HalfEdge::setNext(HalfEdge* next) {
    this->next = next;
}

void HalfEdge::setSym(HalfEdge* sym) {
    this->sym = sym;
    sym->sym = this;
}
///----------------------------------------------------------------------
/// Vertex
///----------------------------------------------------------------------
Vertex::Vertex() :
    position(glm::vec3())
{
    count++;
    std::unique_ptr<int> a = std::make_unique<int>(count);
    QString QVert = QString("Vertex")+QString().number(count);
    setText(QVert);
}

Vertex::Vertex(glm::vec3 pos) :
    position(pos)
{
    count++;
    idx = std::make_unique<int>(count);
    QString QVert = QString("Vertex")+QString().number(count);
    setText(QVert);
    // std::unique_ptr<int> a = std::make_unique<int>(count);
}

void Vertex::setCount(int idx) {
    count = idx;
    return;
}

void Vertex::setPosition(glm::vec3 pos) {
    this->position = pos;
}

void Vertex::setHalfEdge(HalfEdge* edge) {
    this->edge = edge;
}

void Vertex::operator=(const Vertex &v) {
    this->position = v.position;
    this->edge = v.edge;
    this->count = v.count;
}

///----------------------------------------------------------------------
/// Mesh
///----------------------------------------------------------------------
Mesh::Mesh(OpenGLContext *context) : Drawable(context),
    m_face(context),
    currFace(-1)
{}

void Mesh::addVertex(float x, float y, float z) {
    uPtr<class Vertex> newVertex = mkU<class Vertex>(glm::vec3(x, y, z));
    // Vertex::count++;
    vertex.push_back(std::move(newVertex));
    // Vertex::setCount(1);
}

int Mesh::addEdge(uint faceIdx, uint idx) {
    // std::cout<<first<<" "<<second<<std::endl;
    Vertex * v = vertex[idx - 1].get();
    Face * f = face[faceIdx].get();
    uPtr<HalfEdge> newHalfEdge = mkU<HalfEdge>(this, v, f);
    halfEdge.push_back(std::move(newHalfEdge));
    face[faceIdx]->vertices.push_back(idx - 1);
    return HalfEdge::getCount();
}

uint Mesh::addFace(void) {
    // std::cout<<first<<" "<<second<<std::endl;
    uPtr<Face> newFace = mkU<Face>();
    face.push_back(std::move(newFace));
    // Face::incCount();
    return Face::getCount() - 1;
}

bool Mesh::addPair(uint first, uint second) {
    pair<uint, uint> vertexPair;
    if(second < first) {
        vertexPair.first = second;
        vertexPair.second = first;
        // pair<uint, uint> vertexPair(second, first);
    } else {
        vertexPair.first = first;
        vertexPair.second = second;
        // pair<uint, uint> vertexPair(first, second);
    }

    // if (endPoints.find(0) == endPoints.end()) {
    //     // cout << key << " not found\n";
    //     endPoints[vertexPair] = endPoints.size();
    // } else {
    //     // cout << "Found " << key << endl;
    //     halfEdge[endPoints[vertexPair]];
    // }

    // unordered_map<pair<uint, uint>, uint>::iterator itr;
    // for (itr = endPoints.begin();
    //      itr != endPoints.end(); itr++)
    // {

    // }

    return false;
}

bool Mesh::computeCentroid() {
    for(auto& faces : face) {
        HalfEdge * currEdge = faces->edge;
        glm::vec3 avg(0.f);
        int edgeCount = 0;
        // int verticeOnFace = 0;
        do {
            avg += currEdge->vNext->position;
            edgeCount++;
        } while(currEdge != faces->edge);
        avg /= edgeCount;
        // centroid.insert(*faces->idx, avg);
        centroid.insert(make_pair((*faces->idx) - 1, avg));
    }
    return true;
}

bool Mesh::computeHalfEdge() {
    // for(auto& faces : face) {
    //     HalfEdge * currEdge = faces->edge;
    //     glm::vec3 avg(0.f);
    //     int edgeCount = 0;

    //     // int verticeOnFace = 0;
    //     do {
    //         if(currEdge->sym != NULL)
    //             avg = currEdge->vNext->position + currEdge->sym->vNext->position;
    //         else
    //             avg = currEdge->vNext->position;
    //         int faceIdx1 = *currEdge->face->idx;
    //         int faceIdx2 = (currEdge->sym != NULL) ? *currEdge->sym->face->idx : 0;
    //         avg += centroid[faceIdx1] + centroid[faceIdx2];
    //         avg /= 4;
    //         edgeMid.insert(make_pair(*currEdge->idx, avg));
    //         currEdge = currEdge->next;
    //     } while(currEdge != faces->edge);
    // }
    for(auto& edge : halfEdge) {

        glm::vec3 avg(0.f);
        if(edge->sym != NULL)
            avg = edge->vNext->position + edge->sym->vNext->position;
        else
            avg = edge->vNext->position;
        int faceIdx1 = *edge->face->idx;
        int faceIdx2 = (edge->sym != NULL) ? *edge->sym->face->idx : 0;
        avg += centroid[faceIdx1] + centroid[faceIdx2];
        avg /= 4;
        edgeMid.insert(make_pair(*edge->idx, avg));
    }
    return true;
}


bool Mesh::computeVertices() {
    int numVertices = vertex.size();
    std::vector<glm::vec3> vertSum(numVertices, {0.0, 0.0, 0.0});
    std::vector<glm::vec3> edgeSum(numVertices, {0.0, 0.0, 0.0});
    std::vector<glm::vec3> faceSum(numVertices, {0.0, 0.0, 0.0});
    int currIdx = 0;
    for(auto& faces : face) {
        HalfEdge * currEdge = faces->edge;
        glm::vec3 avg(0.f);
        int edgeCount = 0;

        // int verticeOnFace = 0;
        do {
            if(vertSum[*(currEdge->vNext->idx) - 1] == glm::vec3(0.0)) {
                vertSum[*(currEdge->vNext->idx) - 1] += currEdge->vNext->position;
            }
            edgeSum[*(currEdge->vNext->idx) - 1] += edgeMid[*currEdge->idx];
            faceSum[*(currEdge->vNext->idx) - 1] += centroid[*faces->idx];
            currEdge = currEdge->next;
        } while(currEdge != faces->edge);
    }
    for(auto& vert:vertex) {
        int id = *vert->idx;
        int n = vertexCount[id];
        int n_2 = n * n;
        vertexSmooth[id] = ((float)(n - 2) * vertSum[id - 1]) / ((float)n) +
                            edgeSum[id - 1] / ((float)n_2) +
                            faceSum[id - 1] / ((float)n_2);
    }

    return true;
}

void Mesh::quarangulate() {

    return;
}

void Mesh::redraw() {

}

void Mesh::catmullClark() {
    computeCentroid();
    computeHalfEdge();
    computeVertices();
    quarangulate();
    redraw();
}

uint Mesh::getCompCount(uint comp) {
    switch(comp) {
    case MESH_FACE: return face.size();
    case MESH_EDGE: return halfEdge.size();
    case MESH_VERTEX: return vertex.size();
    default: return 0;
    }
}

void Mesh::setCurrFace(int f) {
    currFace = f;
    return;
}

class Polygon Mesh::drawFace(OpenGLContext *context, uint idx) {
    uint vertices = face[idx]->getVerticesCount();
    m_face.initializeAndBufferGeometryData();
    return m_face;
}

void Mesh::initializeAndBufferGeometryData() {
#if 1
    unsigned int facesCount = getCompCount(MESH_FACE);
    if(facesCount == 0) {
        std::vector<glm::vec3> pos {glm::vec3(-2, -2, 0),
                                   glm::vec3(2, -2, 0),
                                   glm::vec3(2, 2, 0),
                                   glm::vec3(-2, 2, 0)};

        std::vector<glm::vec3> nor {glm::vec3(0, 0, 1),
                                   glm::vec3(0, 0, 1),
                                   glm::vec3(0, 0, 1),
                                   glm::vec3(0, 0, 1)};

        std::vector<glm::vec3> col {glm::vec3(1, 0, 0),
                                   glm::vec3(0, 1, 0),
                                   glm::vec3(0, 0, 1),
                                   glm::vec3(1, 1, 0)};

        std::vector<GLuint> idx {0, 1, 2, 0, 2, 3};
        indexBufferLength = 6;

        generateBuffer(INDEX);
        bindBuffer(INDEX);
        bufferData(INDEX, idx);

        generateBuffer(POSITION);
        bindBuffer(POSITION);
        bufferData(POSITION, pos);

        generateBuffer(NORMAL);
        bindBuffer(NORMAL);
        bufferData(NORMAL, nor);

        generateBuffer(COLOR);
        bindBuffer(COLOR);
        bufferData(COLOR, col);

        return;
    }

    std::vector<glm::vec3> pos;
    std::vector<glm::vec3> nor;
    std::vector<glm::vec3> col;
    std::vector<GLuint> idx;

    int totalVertices = 0;
    for(auto& faces:face) {
        // Face* f0 = face[0].get();
        HalfEdge * currEdge = faces->edge;
        int verticeOnFace = 0;
        do {
            pos.push_back(currEdge->vNext->position);
            col.push_back(faces->color);
            // col.push_back(glm::vec3(1, 1, 1));
            if(currEdge->sym != NULL) {
                glm::vec3 v0 = currEdge->vNext->position - currEdge->sym->vNext->position;
                glm::vec3 v1 = currEdge->next->vNext->position - currEdge->vNext->position;
                nor.push_back(glm::normalize(glm::cross(v0, v1)));
            } else {
                nor.push_back(glm::vec3(0, 0, 1));
            }
            ++verticeOnFace;
            currEdge = currEdge->next;
        } while(currEdge != faces->edge);

        for(int i = 0; i < verticeOnFace - 2; ++i) {
            idx.push_back(totalVertices);
            idx.push_back(totalVertices + i + 1);
            idx.push_back(totalVertices + i + 2);
        }
        totalVertices += verticeOnFace;
    }

    indexBufferLength = idx.size();

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    generateBuffer(NORMAL);
    bindBuffer(NORMAL);
    bufferData(NORMAL, nor);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, col);

    return;

#endif
}

bool Mesh::clearMesh(void) {

    face.clear();
    vertex.clear();
    halfEdge.clear();
    currFace = -1;
    HalfEdge::setCount(0);
    Face::setCount(0);
    Vertex::setCount(0);
    return true;
}

bool Mesh::updateColor(int i) {
    colorR = i;
    std::vector<glm::vec3> colors;
    std::fill(colors.begin(), colors.end(), glm::vec3(i, 0, 0));
    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, colors);
    // if(color == 0)
    //     colorR = i;
    // else if(color == 1)
    //     colorG = i;
    // else if(color == 2)
    //     colorB = i;
    return true;
}

bool Mesh::addVertexPair(const uint v1, const uint v2, const int halEdgeIdx) {
    pair<uint, uint> v(v1, v2);
    if(v1 > v2) {
        v.first = v2;
        v.second = v1;
    }
    if (vertexPair.find(v) == vertexPair.end())
        vertexPair.insert(make_pair(v, false));
    else {
        vertexPair.insert(make_pair(v, true));
        return false;
    }

    return true;
}
