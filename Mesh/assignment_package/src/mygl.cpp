#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QFile>
#include <QFileDialog>
#include "fileHandler.h"
#include "Mesh.h"

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      timer(), currTime(0.),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this),
      vao(),
      m_camera(width(), height()),
      m_mousePosPrev(),
      updateUI(false),
      edgeSelection(nullptr),
      faceSelection(nullptr),
      vertexSelection(nullptr),
      face_draw(this),
      point(this),
      line(this)
{
    mesh = mkU<Mesh>(this);
    setFocusPolicy(Qt::StrongFocus);

    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    // Create the instances of Cylinder and Sphere.
    mesh->initializeAndBufferGeometryData();

    // Create and set up the diffuse shader
    m_progLambert.createAndCompileShaderProgram("lambert.vert.glsl", "lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.createAndCompileShaderProgram("flat.vert.glsl", "flat.frag.glsl");


    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::updateBufferData() {
    updateUI = true;
    mesh->catmullClark();
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_camera.recomputeAspectRatio(w, h);

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)
    glm::mat4 viewproj = m_camera.getViewProj();
    m_progLambert.setUnifMat4("u_ViewProj", viewproj);
    m_progFlat.setUnifMat4("u_ViewProj", viewproj);

    printGLErrorLog();
}

bool MyGL::clearMesh(void) {

    mesh->clearMesh();
    return true;
}

void MyGL::drawMesh() {

#if 0
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
    //Send the geometry's transformation matrix to the shader
    m_progLambert.setUnifMat4("u_Model", model);
    m_progLambert.setUnifMat4("u_ModelInvTr", glm::inverse(glm::transpose(model)));
    //Draw the example sphere using our lambert shader
    m_progLambert.draw(m_geomSquare);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    m_progLambert.setUnifMat4("u_Model", model);
    m_progLambert.setUnifMat4("u_ModelInvTr", glm::inverse(glm::transpose(model)));
    m_progLambert.draw(m_geomSquare);
#endif
#if 1

    if(updateUI) {
        unsigned int faceCount = mesh->getCompCount(MESH_FACE);
        // this->mesh->initializeAndBufferGeometryData();

        if(faceCount > 0) {
            // for(unsigned int i = 0; i < faceCount; ++i) {
                // class Polygon p(this);
                // glm::mat4 viewproj = m_camera.getViewProj();
                // m_progLambert.setUnifMat4("u_ViewProj", viewproj);
                // m_progFlat.setUnifMat4("u_ViewProj", viewproj);
                // m_progLambert.setUnifVec3("u_CamPos", m_camera.eye);
                // m_progFlat.setUnifMat4("u_Model", glm::mat4(1.f));

                mesh->setCurrFace(0);
                // // class Polygon p = this->mesh->drawFace(this, i);
                // //Create the instances of Cylinder and Sphere.
                // // p.initializeAndBufferGeometryData();
                this->mesh->initializeAndBufferGeometryData();

                // // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,i,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
                // // //Send the geometry's transformation matrix to the shader
                // glm::mat4 model = glm::mat4();
                // m_progLambert.setUnifMat4("u_Model", model);
                // m_progLambert.setUnifMat4("u_ModelInvTr", glm::inverse(glm::transpose(model)));
                // // //Draw the example sphere using our lambert shader
                // m_progLambert.draw(*(this->mesh));
            // }
            glm::mat4 model = glm::mat4();
            m_progLambert.setUnifMat4("u_Model", model);
            m_progLambert.setUnifMat4("u_ModelInvTr", glm::inverse(glm::transpose(model)));
            // //Draw the example sphere using our lambert shader
            m_progLambert.draw(*(this->mesh));
        }
        updateUI = false;
        update();
    } else {
        glm::mat4 model = glm::mat4();
        m_progLambert.setUnifMat4("u_Model", model);
        m_progLambert.setUnifMat4("u_ModelInvTr", glm::inverse(glm::transpose(model)));
        // //Draw the example sphere using our lambert shader
        m_progLambert.draw(*(this->mesh));
    }
#endif
    return;
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewproj = m_camera.getViewProj();
    m_progLambert.setUnifMat4("u_ViewProj", viewproj);
    m_progFlat.setUnifMat4("u_ViewProj", viewproj);
    m_progLambert.setUnifVec3("u_CamPos", m_camera.eye);
    m_progFlat.setUnifMat4("u_Model", glm::mat4(1.f));

    for (auto& vert : mesh->vertex)
    {
        emit sig_sendListItem(vert.get(), MESH_VERTEX);
    }

    for (auto& face : mesh->face)
    {
        emit sig_sendListItem(face.get(), MESH_FACE);
    }

    for (auto& edge : mesh->halfEdge)
    {
        emit sig_sendListItem(edge.get(), MESH_EDGE);
    }

    //Create a model matrix. This one rotates the square by PI/4 radians then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    // glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
    //Send the geometry's transformation matrix to the shader
    // glm::mat4 model = glm::mat4();
    // m_progLambert.setUnifMat4("u_Model", model);
    // m_progLambert.setUnifMat4("u_ModelInvTr", glm::inverse(glm::transpose(model)));
    // //Draw the example sphere using our lambert shader
    // m_progLambert.draw(m_geomSquare);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    // model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    // m_progLambert.setUnifMat4("u_Model", model);
    // m_progLambert.setUnifMat4("u_ModelInvTr", glm::inverse(glm::transpose(model)));
    // m_progLambert.draw(m_geomSquare);
    drawMesh();
}

void MyGL::keyPressEvent(QKeyEvent *e) {
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        // m_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        // m_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        // m_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        // m_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        // m_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        // m_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        // m_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        // m_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        // m_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        // m_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        // m_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        // m_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_P) {
        // cube_display = 1 - cube_display;
    }
    else if (e->key() == Qt::Key_R) {
        //        gl_camera = Camera(this->width(), this->height());
        // renderMode = 1 - renderMode;
    } else if(e->key() == Qt::Key_N) {
        line.pos.clear();
        line.pos.push_back(glm::vec4(edgeSelection->vNext->position, 1));
        line.pos.push_back(glm::vec4(edgeSelection->next->vNext->position, 1));

        edgeSelection = edgeSelection->next;

        line.destroyGPUData();
        line.initializeAndBufferGeometryData();
    } else if(e->key() == Qt::Key_M) {
        line.pos.clear();
        line.pos.push_back(glm::vec4(edgeSelection->vNext->position, 1));
        line.pos.push_back(glm::vec4(edgeSelection->sym->vNext->position, 1));

        edgeSelection = edgeSelection->sym;
    } else if(e->key() == Qt::Key_F) {
        faceSelection = edgeSelection->face;
        face_draw.col.clear();
        face_draw.pos.clear();

        //start point
        face_draw.pos.push_back(glm::vec4(faceSelection->edge->vNext->position,1));
        face_draw.col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));

        HalfEdge* lastEdge = faceSelection->edge->next;

        while (lastEdge != faceSelection->edge)
        {
            face_draw.pos.push_back(glm::vec4(lastEdge->vNext->position,1));
            face_draw.pos.push_back(glm::vec4(lastEdge->vNext->position,1));
            face_draw.col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));
            face_draw.col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));

            lastEdge = lastEdge->next;
        }
        face_draw.pos.push_back(glm::vec4(faceSelection->edge->vNext->position,1));
        face_draw.col.push_back(glm::vec4(glm::vec3(1,1,1) - faceSelection->color, 1));

        face_draw.destroyGPUData();
        face_draw.initializeAndBufferGeometryData();
    } else if(e->key() == Qt::Key_V) {
        point.pos.clear();
        point.pos.push_back(glm::vec4(edgeSelection->vNext->position,1));

        point.destroyGPUData();
        point.initializeAndBufferGeometryData();
    } else if ((e->key() == Qt::Key_H) && QApplication::keyboardModifiers() && Qt::ShiftModifier) {
        line.pos.clear();

        line.pos.push_back(glm::vec4(faceSelection->edge->vNext->position,1));
        line.pos.push_back(glm::vec4(faceSelection->edge->sym->vNext->position, 1));

        line.destroyGPUData();
        line.initializeAndBufferGeometryData();
    } else if(e->key() == Qt::Key_H) {
        line.pos.clear();

        line.pos.push_back(glm::vec4(vertexSelection->edge->vNext->position,1));
        line.pos.push_back(glm::vec4(vertexSelection->edge->sym->vNext->position, 1));

        line.destroyGPUData();
        line.initializeAndBufferGeometryData();
    }
    // m_camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::mousePressEvent(QMouseEvent *e) {
    if(e->buttons() & (Qt::LeftButton | Qt::RightButton))
    {
        m_mousePosPrev = glm::vec2(e->position().x(), e->position().y());
    }
}

void MyGL::mouseMoveEvent(QMouseEvent *e) {
    glm::vec2 pos(e->position().x(), e->position().y());
    if(e->buttons() & Qt::LeftButton)
    {
        // Rotation
        glm::vec2 diff = 0.2f * (pos - m_mousePosPrev);
        m_mousePosPrev = pos;
        m_camera.RotateAboutGlobalUp(-diff.x);
        m_camera.RotateAboutLocalRight(-diff.y);
    }
    else if(e->buttons() & Qt::RightButton)
    {
        // Panning
        glm::vec2 diff = 0.05f * (pos - m_mousePosPrev);
        m_mousePosPrev = pos;
        m_camera.PanAlongRight(-diff.x);
        m_camera.PanAlongUp(diff.y);
    }
}

void MyGL::wheelEvent(QWheelEvent *e) {
    m_camera.Zoom(e->angleDelta().y() * 0.001f);
}

void MyGL::makeList(void) {
    // uPtr<MeshVertex> item = mkU<MeshVertex>(QString("abcd"));
    // emit sig_sendListItem(item.get());
    // return;
    // for(int i = 0; i < 10; ++i) {
    //     uPtr<MeshFace> item = mkU<MeshFace>(QString("Item ") + QString::number(i));
    //     emit sig_sendListItem(item.get(), MESH_EDGE);
    //     face.push_back(std::move(item));
    // }
}

void MyGL::addVertex(float x, float y, float z, unsigned int comp) {
    // uPtr<Vertex> newVertex = mkU<Vertex>(glm::vec3(x, y, z));
    // Vertex::count++;
    // mesh.vertex.push_back(std::move(newVertex));
    mesh->addVertex(x, y, z);
    // uPtr<MeshVertex> item = mkU<MeshVertex>(QString("Vertex ") + QString::number(mesh->getCompCount(comp)));
    // emit sig_sendListItem(item.get(), comp);
    // vertex.push_back(std::move(item));
    // Vertex::setCount(1);
}

void MyGL::addEdge(std::vector<int> vertexIdx, unsigned int faceIdx, unsigned int comp) {
    // std::cout<<first<<" "<<second<<std::endl;
    // uPtr<HalfEdge> newHalfEdge = mkU<HalfEdge>(this, second - 1);
    // halfEdge.push_back(std::move(newHalfEdge));
    int currEdgeIdx = mesh->addEdge(faceIdx, vertexIdx[0]);
    addEdgeToFace(faceIdx, currEdgeIdx);
    HalfEdge * curr = mesh->halfEdge[currEdgeIdx - 1].get();
    mesh->vertexCount[vertexIdx[0]]++;

    for(size_t i = 1; i < vertexIdx.size(); ++i) {
        int nextEdgeIdx = mesh->addEdge(faceIdx, vertexIdx[i]);
        mesh->vertexCount[vertexIdx[i]]++;
        HalfEdge * next = mesh->halfEdge[nextEdgeIdx - 1].get();
        curr->setNext(next);
        // uPtr<MeshEdge> item = mkU<MeshEdge>(QString("Edge ") + QString::number(mesh->getCompCount(comp)));
        // emit sig_sendListItem(item.get(), comp);
        // emit sig_sendListItem(next, comp);
        // edge.push_back(std::move(item));
        if(!mesh->addVertexPair(vertexIdx[i - 1], vertexIdx[i], nextEdgeIdx - 1)) {
            mesh->vertex[vertexIdx[i - 1] - 1]->edge->setSym(mesh->vertex[vertexIdx[i] - 1]->edge);
        }
        curr = next;
    }
    curr->setNext(mesh->halfEdge[currEdgeIdx - 1].get());
    if(!mesh->addVertexPair(vertexIdx[vertexIdx.size() - 1], vertexIdx[0], currEdgeIdx - 1)) {
        mesh->vertex[vertexIdx[vertexIdx.size() - 1] - 1]->edge->setSym(mesh->vertex[vertexIdx[0] - 1]->edge);
    }
    return;
}

void MyGL::addEdgeToFace(int faceIdx, int currEdgeIdx) {
    HalfEdge * edge = mesh->halfEdge[currEdgeIdx - 1].get();
    mesh->face[faceIdx]->setHalfEdge(edge);
}

unsigned int MyGL::addFace(unsigned int comp) {
    // std::cout<<first<<" "<<second<<std::endl;
    // uPtr<HalfEdge> newHalfEdge = mkU<HalfEdge>(this, second - 1);
    // halfEdge.push_back(std::move(newHalfEdge));
    unsigned int idx = mesh->addFace();

    // uPtr<MeshFace> item = mkU<MeshFace>(QString("Face ") + QString::number(mesh->getCompCount(comp)));
    // emit sig_sendListItem(item.get(), comp);
    // face.push_back(std::move(item));
    return idx;
}

void MyGL::loadObj() {
    QString path = getCurrentPath();
    path.append("/assignment_package/obj_files/");
    QString filepath = QFileDialog::getOpenFileName(
        0, QString("Load JSON Scene"),
        path, tr("*.obj"));

    FileHandler f_handle(filepath);
    if(f_handle.LoadObjFromFile(this)) {

    } else {
        std::cout << "Could not load obj file" << std::endl;
    }
    // QFile file(filepath);
    // TODO: Use the JSONReader class to parse the file
    // Then destroy() the pathtracer shader, write a new pathtracer.all.glsl,
    // then create() the pathtracer shader again.

    // JSONReader reader(this);
    // if(reader.LoadSceneFromFile(file, path, m_scene, m_glCamera)) {
    //     QString fragASCII = writeFullShaderFile();
    //     QString vertASCII = qTextFileRead(":/glsl/passthrough.vert.glsl");
    //     m_progPathTracer.m_isReloading = true;
    //     m_progPathTracer.destroyGPUData();
    //     m_progPathTracer.create(vertASCII, fragASCII);
    //     initShaderHandles(true);
    //     m_progPathTracer.setUnifVec2("u_ScreenDims", glm::vec2(width(), height()));
    //     m_progPathTracer.m_isReloading = false;

    //     m_glCamera.RecomputeAttributes();
    //     m_progPathTracer.setUnifVec3("u_Eye", m_glCamera.eye);
    //     m_progPathTracer.setUnifVec3("u_Forward", m_glCamera.look);
    //     m_progPathTracer.setUnifVec3("u_Right", m_glCamera.right);
    //     m_progPathTracer.setUnifVec3("u_Up", m_glCamera.up);
    //     if(m_scene.textures.size() > 0) {
    //         m_progPathTracer.addUniform("u_TexSamplers");
    //     }
    //     resetPathTracer();
    //     update();
    // }
    // else {
    //     std::cout << "Could not load JSON scene" << std::endl;
    // }
}

void MyGL::tick() {
    ++currTime;
    update();
}


bool MyGL::updateColor(int i) {
    mesh->updateColor(i);
    return true;
}

