#include "rasterizer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Rasterizer::Rasterizer(const std::vector<Polygon>& polygons)
    : m_polygons(polygons)
{}

void Rasterizer::getPolygonLimits(const glm::vec4 vertex[3], float* minX,
                                  float* maxX, float* minY, float* maxY,
                                  float* minZ, float* maxZ) {
    for(int i = 0; i < 3; ++i) {
        *minX = (*minX < vertex[i].x) ? *minX : vertex[i].x;
        *maxX = (*maxX > vertex[i].x) ? *maxX : vertex[i].x;
        *minY = (*minY < vertex[i].y) ? *minY : vertex[i].y;
        *maxY = (*maxY > vertex[i].y) ? *maxY : vertex[i].y;
    }
    *minX = (*minX > 0.0) ? *minX : 0.0;
    *maxX = (*maxX < 512.0) ? *maxX : 512.0;
    *minY = (*minY > 0.0) ? *minY : 0.0;
    *maxY = (*maxY < 512.0) ? *maxY : 512.0;
    return;
}

bool inBetween(const float y, const float limit1, const float limit2) {
    if((y <= limit1) && (y >= limit2))
        return true;
    else if((y >= limit1) && (y <= limit2))
        return true;
    return false;
}

bool Rasterizer::getIntersection(const glm::vec4 vertex[3], const float y, float* xStart, float* xEnd) {

#if 1
    Line line1(vertex[0], vertex[1]);
    Line line2(vertex[1], vertex[2]);
    Line line3(vertex[2], vertex[0]);

    float int1 = 0.0, int2 = 0.0, x = 0.0;
    bool int1Set = false, int2Set = false;
    if(line1.checkIntersept(y)) {
        x = line1.getIntercept();
        int1 = x;
        int1Set = true;
        if(line1.slopeY == 0 && y == line1.start[1]) {
            x = line1.getIntercept2();
            int2 = x;
            int2Set = true;
        }
    }

    if(!(int1Set & int2Set)) {
        if(line2.checkIntersept(y)) {
            x = line2.getIntercept();
            if(int1Set) {
                int2 = x;
                int2Set = true;
            } else {
                int1 = x;
                int1Set = true;
                if(line2.slopeY == 0 && y == line2.start[1]) {
                    x = line2.getIntercept2();
                    int2 = x;
                    int2Set = true;
                }
            }
        }
    }

    if(!(int1Set & int2Set)) {
        if(line3.checkIntersept(y)) {
            x = line3.getIntercept();
            if(int1Set) {
                int2 = x;
                int2Set = true;
            } else {
                int1 = x;
                int1Set = true;
                if(line3.slopeY == 0 && y == line3.start[1]) {
                    x = line3.getIntercept2();
                    int2 = x;
                    int2Set = true;
                }
            }
        }
    }
    if(int1 > int2) {
        *xStart = int2;
        *xEnd = int1;
    } else if(int1 < int2) {
        *xStart = int1;
        *xEnd = int2;
    } else {
        *xStart = int1;
        *xEnd = int1;
    }

    *xStart = (*xStart > 0.0) ? *xStart : 0.0;
    *xEnd = (*xEnd < 512.0) ? *xEnd : 512.0;

    return (int1Set | int2Set);

#endif


}

#if 1
glm::vec4 Rasterizer::getColor(Vertex v1, Vertex v2, Vertex v3, glm::vec4 point, GetColor_t type) {
    float area_1 = glm::length(glm::cross(glm::vec3(v2.m_pos[0] - point[0],
                                                    v2.m_pos[1] - point[1], 0),
                                          glm::vec3(v3.m_pos[0] - point[0],
                                                    v3.m_pos[1] - point[1], 0))) / 2;
    float area_2 = glm::length(glm::cross(glm::vec3(v1.m_pos[0] - point[0],
                                                    v1.m_pos[1] - point[1], 0),
                                          glm::vec3(v3.m_pos[0] - point[0],
                                                    v3.m_pos[1] - point[1], 0))) / 2;
    float area_3 = glm::length(glm::cross(glm::vec3(v1.m_pos[0] - point[0],
                                                    v1.m_pos[1] - point[1], 0),
                                          glm::vec3(v2.m_pos[0] - point[0],
                                                    v2.m_pos[1] - point[1], 0))) / 2;
    float area = area_1 + area_2 + area_3;

    float s1 = area_1 / area;
    float s2 = area_2 / area;
    float s3 = area_3 / area;

    if (type == GETCOLOR_2D) {
        int r_value = v1.m_color[0] * s1 + v2.m_color[0] * s2 + v3.m_color[0] * s3;
        int g_value = v1.m_color[1] * s1 + v2.m_color[1] * s2 + v3.m_color[1] * s3;
        int b_value = v1.m_color[2] * s1 + v2.m_color[2] * s2 + v3.m_color[2] * s3;
        return glm::vec4(r_value, g_value, b_value, 0);
    }
    else if (type == GETCOLOR_Z){
        float z = 1.0 / (s1 / v1.m_pos[2] + s2 / v2.m_pos[2] + s3 / v3.m_pos[2]);
        return glm::vec4(point[0], point[1], z, 0);
    }
    else if (type == GETCOLOR_UV){
        glm::vec2 uv1 = point[2] * glm::vec2((v1.m_uv * s1 / v1.m_pos[2] + v2.m_uv * s2 / v2.m_pos[2] + v3.m_uv * s3 / v3.m_pos[2]));
        return glm::vec4(uv1[0], uv1[1], 0, 0);
    }
    else if (type == GETCOLOR_NOR) {
        glm::vec4 normal = point[2] * (v1.m_normal * s1 / v1.m_pos[2] + v2.m_normal * s2 / v2.m_pos[2] + v3.m_normal * s3 / v3.m_pos[2]);
        return normal;
    }
    else
        return glm::vec4(0, 0, 0, 0);
}
#endif

QImage Rasterizer::RenderScene(Camera camera)
{
#if 1

    QImage result(512, 512, QImage::Format_RGB32);
    // Fill the image with black pixels.
    // Note that qRgb creates a QColor,
    // and takes in values [0, 255] rather than [0, 1].
    // result.fill(qRgb(0.f, 0.f, 0.f));
    // TODO: Complete the various components of code that make up this function.
    // It should return the rasterized image of the current scene.

    // Make liberal use of helper functions; writing your rasterizer as one
    // long RenderScene function will make it (a) hard to debug and
    // (b) hard to write without copy-pasting. Also, Adam will be sad when
    // he reads your code.

    // Also! As per the style requirements for this assignment, make sure you
    // use std::arrays to store things like your line segments, Triangles, and
    // vertex coordinates. This lets you easily use loops to perform operations
    // on your scene components, rather than copy-pasting operations three times
    // each!

#if 1
    std::vector<float> zBuffer(MAX_HEIGHT * MAX_WIDTH, FLT_MIN);
    std::vector<std::vector<float> > fog(MAX_HEIGHT, std::vector<float>(MAX_WIDTH, FLT_MIN));

    glm::mat4 transform_matrix = camera.getProjectionMatrix() * camera.getViewMatrix();

    float ambientIntensity = 0.3f;

    glm::vec4 lightDirection = -glm::normalize(camera.getForwardVector());

    for(auto& poly : m_polygons) {
        for(auto& tris : poly.m_tris) {
            float minX = 512.f, maxX = 0.f, minY = 512.f, maxY = 0.f, minZ = FLT_MAX, maxZ = FLT_MIN;
            Vertex v1 = poly.VertAt(tris.m_indices[0]);
            Vertex v2 = poly.VertAt(tris.m_indices[1]);
            Vertex v3 = poly.VertAt(tris.m_indices[2]);

            glm::vec4 v1_pos = transform_matrix * v1.m_pos;
            glm::vec4 v2_pos = transform_matrix * v2.m_pos;
            glm::vec4 v3_pos = transform_matrix * v3.m_pos;

            v1_pos /= v1_pos[3];
            v2_pos /= v2_pos[3];
            v3_pos /= v3_pos[3];

            glm::vec4 triVertex[3] = {glm::vec4((v1_pos[0] + 1) * 512 / 2, (1 - v1_pos[1]) * 512 / 2, v1_pos[3], 1),
                                      glm::vec4((v2_pos[0] + 1) * 512 / 2, (1 - v2_pos[1]) * 512 / 2, v1_pos[3], 1),
                                      glm::vec4((v3_pos[0] + 1) * 512 / 2, (1 - v3_pos[1]) * 512 / 2, v1_pos[3], 1)};


            Vertex v4 = Vertex(triVertex[0], v1.m_color, v1.m_normal, v1.m_uv);
            Vertex v5 = Vertex(triVertex[1], v2.m_color, v2.m_normal, v2.m_uv);
            Vertex v6 = Vertex(triVertex[2], v3.m_color, v3.m_normal, v3.m_uv);

            getPolygonLimits(triVertex, &minX, &maxX, &minY, &maxY, &minZ, &maxZ);
            float h = minY;
            minZ = (minZ < v1.m_pos.z) ? minZ : v1.m_pos.z;
            maxZ = (maxZ > v1.m_pos.z) ? maxZ : v1.m_pos.z;
            minZ = (minZ < v2.m_pos.z) ? minZ : v2.m_pos.z;
            maxZ = (maxZ > v2.m_pos.z) ? maxZ : v2.m_pos.z;
            minZ = (minZ < v3.m_pos.z) ? minZ : v3.m_pos.z;
            maxZ = (maxZ > v3.m_pos.z) ? maxZ : v3.m_pos.z;

            // // Calculate final color with Lambertian reflection model
            // glm::vec3 finalColor1 = glm::vec3(v1.m_color) * (ambientIntensity + intensity1);
            // glm::vec3 finalColor2 = glm::vec3(v2.m_color) * (ambientIntensity + intensity2);
            // glm::vec3 finalColor3 = glm::vec3(v3.m_color) * (ambientIntensity + intensity3);
            // minZ = (minZ == 0.f) ? 0.0000001 : minZ;
            while(h < maxY) {
                float xStart = 0.f, xEnd = 0.f;
                if(getIntersection(triVertex, h, &xStart, &xEnd)) {
                    float w = xStart;

                    while(w < xEnd) {
                        glm::vec4 pos = glm::vec4(w, h, 0.f, 0.f);
                        glm::vec4 z_correct_vec = getColor(v4, v5, v6, pos, GETCOLOR_Z);
                        glm::vec4 uv_vec = getColor(v4, v5, v6, glm::vec4(w, h, z_correct_vec[2], 0.f), GETCOLOR_UV);
                        glm::vec4 nor_vec = getColor(v4, v5, v6, glm::vec4(w, h, z_correct_vec[2], 0.f), GETCOLOR_NOR);

                        float comp = (minZ == 0.f) ? 0.000001 : minZ;

                        QImage *texture = poly.mp_texture;
                        float attenuate = glm::clamp(glm::dot(nor_vec, lightDirection), 0.f, 1.f);
                        glm::vec3 c = attenuate * GetImageColor(glm::vec2(uv_vec[0], uv_vec[1]), texture);

                        if(zBuffer[(unsigned int)w + ((unsigned int)h * MAX_WIDTH)] < comp) {
                            zBuffer[(unsigned int)w + ((unsigned int)h * MAX_WIDTH)] = minZ;
                            result.setPixelColor(glm::clamp(w, 0.f, 512.f),
                                                 glm::clamp(h, 0.f, 512.f),
                                                 qRgb(c.r, c.g, c.b)/*value*/);
                        }
                        w++;
                    }
                }
                h+=0.5;
            }
        }
    }
#endif
    return result;
#endif
}

void Rasterizer::ClearScene()
{
    m_polygons.clear();
}
