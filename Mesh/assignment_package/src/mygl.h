#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include <scene/Polygon.h>
#include "camera.h"
#include "Mesh.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <memory>

class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    QTimer timer;
    float currTime;

    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_camera;
    // A variable used to track the mouse's previous position when
    // clicking and dragging on the GL viewport. Used to move the camera
    // in the scene.
    glm::vec2 m_mousePosPrev;
    uPtr<Mesh> mesh/* = mkU<Mesh>()*/;
    bool updateUI;

    HalfEdge *edgeSelection;
    Face* faceSelection;
    Vertex* vertexSelection;

    DrawFace face_draw;
    DrawPoint point;
    DrawLine line;

    std::map<int, Vertex*> centroid;
    std::map<int, std::vector<Vertex*>> mid; // each face -> midpoint
    std::map<int, Vertex*> midpoint; // each original halfedge -> midpoint

public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void drawMesh();
    bool clearMesh(void);
    void makeList(void);
    void addVertex(float x, float y, float z, unsigned int comp);
    void addEdge(std::vector<int> vertexIdx, unsigned int faceIdx, unsigned int comp);
    void addEdgeToFace(int faceIdx, int currEdgeIdx);
    unsigned int addFace(unsigned int comp);
    bool updateColor(int i);
    void updateBufferData();

protected:
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

public slots:
    void tick();
    void loadObj();

signals:
    void sig_sendListItem(QListWidgetItem*, unsigned int);

};


#endif // MYGL_H
