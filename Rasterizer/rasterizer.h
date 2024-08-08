#pragma once
#include <polygon.h>
#include <QImage>
#include "camera.h"

#define MAX_WIDTH   512
#define MAX_HEIGHT  512

typedef enum {
    GETCOLOR_2D = 0,
    GETCOLOR_Z,
    GETCOLOR_UV,
    GETCOLOR_NOR
}GetColor_t;

class Rasterizer
{
private:
    //This is the set of Polygons loaded from a JSON scene file
    std::vector<Polygon> m_polygons;
    // std::vector<int> zBuffer(INT_MIN);
    // std::vector<int> zBuffer;
public:
    Rasterizer(const std::vector<Polygon>& polygons);
    void getPolygonLimits(const glm::vec4 vert[3], float *minX, float *maxX, float *minY, float *maxY, float* minZ, float* maxZ);
    bool getIntersection(const glm::vec4 vertex[3], const float y, float* xStart, float* xEnd);
    glm::vec4 getColor(Vertex v1, Vertex v2, Vertex v3, glm::vec4 point, GetColor_t type);
    QImage RenderScene(Camera camera);
    void ClearScene();
};
