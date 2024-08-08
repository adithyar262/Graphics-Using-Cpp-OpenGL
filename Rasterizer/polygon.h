#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <QString>
#include <QImage>
#include <QColor>

// A Vertex is a point in space that defines one corner of a polygon.
// Each Vertex has several attributes that determine how they contribute to the
// appearance of their Polygon, such as coloration.
struct Vertex
{
    glm::vec4 m_pos;    // The position of the vertex. In hw02, this is in pixel space.
    glm::vec3 m_color;  // The color of the vertex. X corresponds to Red, Y corresponds to Green, and Z corresponds to Blue.
    glm::vec4 m_normal; // The surface normal of the vertex (not yet used)
    glm::vec2 m_uv;     // The texture coordinates of the vertex (not yet used)

    Vertex(glm::vec4 p, glm::vec3 c, glm::vec4 n, glm::vec2 u)
        : m_pos(p), m_color(c), m_normal(n), m_uv(u)
    {}
};

// Each Polygon can be decomposed into triangles that fill its area.
struct Line
{

    glm::vec4 start;
    glm::vec4 end;
    glm::vec4 dir;
    glm::vec4 c;
    // Slope
    float slopeX;
    float slopeY;
    // Intercept
    float intercept;
    float intercept2;

    Line(glm::vec4 p1, glm::vec4 p2) : start(p1), end(p2)
    {
        slopeY = (end.y - start.y);
        slopeX = (end.x - start.x);
        dir = (end - start) / glm::length(end - start);
        // intercept = (start.y - start.x) / slope;
    }

    float getIntercept(void) {
        // return (y - intercept) / abs(slopeY);
        return intercept;
    }
    float getIntercept2(void) {
        // return (y - intercept) / abs(slopeY);
        return intercept2;
    }
    bool checkIntersept(const float y) {
        if ((start[1] - y) * (end[1] - y) > 0)
            return false;
        else if (slopeX == 0 && (start[1] - y) * (end[1] - y) <= 0) {
            intercept = start[0];
            intercept2 = end[0];
            return true;
        }
        else if (slopeY == 0 && start[1] != y)
            return false; // not intersect
        else if (slopeY == 0 && start[1] == y) {
            intercept = fmin(start[0], end[0]);
            intercept2 = fmax(start[0], end[0]);
            return true; // but has two intersection points
        }
        else {
            float slope = slopeY / slopeX;
            intercept = start[0] - (start[1] - y) / slope;
            return true;
        }
    }
};

// Each Polygon can be decomposed into triangles that fill its area.
struct Triangle
{
    // The indices of the Vertices that make up this triangle.
    // The indices correspond to the std::vector of Vertices stored in the Polygon
    // which stores this Triangle
    unsigned int m_indices[3];
};

class Polygon
{
public:
    // TODO: Populate this list of triangles in Triangulate()
    std::vector<Triangle> m_tris;
    // The list of Vertices that define this polygon. This is alreaslopeY filled by the Polygon constructor.
    std::vector<Vertex> m_verts;
    // The name of this polygon, primarily to help you debug
    QString m_name;
    // The image that can be read to determine pixel color when used in conjunction with UV coordinates
    // Not used until homework 3.
    QImage* mp_texture;
    // The image that can be read to determine surface normal offset when used in conjunction with UV coordinates
    // Not used until homework 3
    QImage* mp_normalMap;

    // Polygon class constructors
    Polygon(const QString& name, const std::vector<glm::vec4>& pos, const std::vector<glm::vec3> &col);
    Polygon(const QString& name, int sides, glm::vec3 color, glm::vec4 pos, float rot, glm::vec4 scale);
    Polygon(const QString& name);
    Polygon();
    Polygon(const Polygon& p);
    ~Polygon();

    // TODO: Complete the boslopeY of Triangulate() in polygon.cpp
    // Creates a set of triangles that, when combined, fill the area of this convex polygon.
    void Triangulate();

    // Copies the input QImage into this Polygon's texture
    void SetTexture(QImage*);

    // Copies the input QImage into this Polygon's normal map
    void SetNormalMap(QImage*);

    // Various getter, setter, and adder functions
    void AddVertex(const Vertex&);
    void AddTriangle(const Triangle&);
    void ClearTriangles();

    Triangle& TriAt(unsigned int);
    Triangle TriAt(unsigned int) const;

    Vertex& VertAt(unsigned int);
    Vertex VertAt(unsigned int) const;
};

// Returns the color of the pixel in the image at the specified texture coordinates.
// Returns white if the image is a null pointer
glm::vec3 GetImageColor(const glm::vec2 &uv_coord, const QImage* const image);
