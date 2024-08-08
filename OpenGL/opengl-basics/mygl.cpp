#include "mygl.h"
#include <QDir>
#include <string.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#define WIDTH 640
#define HEIGHT 480

MyGL::MyGL(QWidget *parent)
    : QOpenGLWidget(parent),
    timer(), currTime(0.f),
    vao(),
    vertShader(), fragShader(), shaderProgram(),
    shaderAttribVariableHandles(), shaderUniformVariableHandles(),
    bufferPosition(), bufferIndex(),
    indexBufferLength(-1), m_vertices(3)
{
    // Allow Qt to trigger mouse events
    // even when a mouse button is not held.
    setMouseTracking(true);

    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
}


MyGL::~MyGL(){}

// As noted in mygl.h, use this function in order to get
// the direct file path to this Qt project so that you
// can read the contents of your GLSL files.
QString MyGL::getCurrentPath() const {
    QString path = QDir::currentPath();
    path = path.left(path.lastIndexOf("/"));
#ifdef __APPLE__
    path = path.left(path.lastIndexOf("/"));
    path = path.left(path.lastIndexOf("/"));
    path = path.left(path.lastIndexOf("/"));
#endif
    return path;
}

// A helper function for createAndCompileShaderProgram.
// It reads the contents of a file into a char*.
char* textFileRead(const char* fileName) {
    char* text = nullptr;

    if (fileName != nullptr) {
        FILE *file = fopen(fileName, "rt");

        if (file != nullptr) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);

            if (count > 0) {
                text = (char*)malloc(sizeof(char) * (count + 1));
                count = fread(text, sizeof(char), count, file);
                text[count] = '\0';	//cap off the string with a terminal symbol
            }
            fclose(file);
        }
    }
    return text;
}

void MyGL::createAndCompileShaderProgram(GLuint &vertHandle,
                                         GLuint &fragHandle,
                                         GLuint &shaderProgramHandle,
                                         QString vertFile,
                                         QString fragFile) {

    // Set up the direct filepath to the vertex and fragment
    // shader files.
    QString projectPath = getCurrentPath();
    projectPath.append("/glsl/");
    QString vertPath = projectPath + vertFile;
    QString fragPath = projectPath + fragFile;

    // Make the OpenGL function calls that create a
    // vertex shader object, fragment shader object,
    // and shader program object on the GPU. We store
    // CPU-side handles to these GPU-side objects in
    // the GLuints passed into this function.
    vertHandle = glCreateShader(GL_VERTEX_SHADER);
    fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgramHandle = glCreateProgram();

    // Parse the plain-text contents of the vertex
    // and fragment shader files, storing them in C-style
    // char* strings (how OpenGL expects strings to be formatted).
    char *vertexShaderSource = textFileRead(vertPath.toStdString().c_str());
    char *fragmentShaderSource = textFileRead(fragPath.toStdString().c_str());

    // Send the contents of the shader files to the GPU
    glShaderSource(vertHandle, 1, &vertexShaderSource, 0);
    glShaderSource(fragHandle, 1, &fragmentShaderSource, 0);
    // Manually de-allocate the heap memory used to store the
    // shader contents. We don't need it now that it's been sent
    // to the GPU.
    free(vertexShaderSource);
    free(fragmentShaderSource);

    // Tell OpenGL on the GPU to try compiling the shader code
    // we just sent.
    glCompileShader(vertHandle);
    glCompileShader(fragHandle);
    // Check if everything compiled OK. If not, print out
    // any errors sent to us from the GPU-side shader compiler.
    GLint compiled;
    glGetShaderiv(vertHandle, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        std::cout << "Errors from " << vertFile.toStdString() << ":" << std::endl;
        printShaderInfoLog(vertHandle);
    }
    glGetShaderiv(fragHandle, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        std::cout << "Errors from " << fragFile.toStdString() << ":" << std::endl;
        printShaderInfoLog(fragHandle);
    }

    // Tell shaderProgramHandle that it manages
    // these particular vertex and fragment shaders
    glAttachShader(shaderProgramHandle, vertHandle);
    glAttachShader(shaderProgramHandle, fragHandle);
    glLinkProgram(shaderProgramHandle);

    // Check for linking success
    GLint linked;
    glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &linked);
    if (!linked) {
        printLinkInfoLog(shaderProgramHandle);
    }
}

void MyGL::getHandlesForShaderVariables(GLuint shaderProgramHandle) {
    // TODO: Write code that gets the handle IDs for each "in"
    // and "uniform" variable used in your shader program,
    // and put them in shaderAttribVariableHandles and
    // shaderUniformVariableHandles. The key for each ID handle
    // should be the string representation of its name, e.g.
    // "vs_Pos" or "u_ScreenDimensions", etc.
    // You will need to make use of the following OpenGL API calls:
    // - glGetUniformLocation
    // - glGetAttribLocation

    glUseProgram(shaderProgramHandle);

    // DELETEME VVVVV
    printGLErrorLog();

    shaderUniformVariableHandles["u_ScreenDimensions"] = glGetUniformLocation(shaderProgramHandle, "u_ScreenDimensions");
    shaderUniformVariableHandles["u_MousePosition"] = glGetUniformLocation(shaderProgramHandle, "u_MousePosition");
    shaderUniformVariableHandles["u_Model"] = glGetUniformLocation(shaderProgramHandle, "u_Model");
    shaderUniformVariableHandles["u_Time"] = glGetUniformLocation(shaderProgramHandle, "u_Time");

    printGLErrorLog();
    shaderAttribVariableHandles["vs_Pos"] = glGetAttribLocation(shaderProgramHandle, "vs_Pos");

    printGLErrorLog();
}

void MyGL::initializeAndBufferGeometryData() {
    // TODO: Populate these two std::vectors
    // with data representing the vertex positions
    // of your shape, and the indices that will
    // triangulate that shape using the fan method.
    std::vector<glm::vec3> pos;
    std::vector<GLuint> idx;

    // Your code here

    // Set indexBufferLength to the total number of
    // indices used to draw your polygon so that it
    // can be passed to glDrawElements in the
    // drawGeometry function.
    // indexBufferLength = idx.size();

    // TODO: Use glGenBuffers, glBindBuffer, and glBufferData
    // to set up the GPU-side storage for your geometry data
    // via bufferPosition and bufferIndex.
    // Don't forget to use the GL_ARRAY_BUFFER flag for bufferPosition,
    // but use GL_ELEMENT_ARRAY_BUFFER for bufferIndex since it represents
    // triangle indices rather than mesh vertices.
    // const int numSides = m_vertices;
    const int numSides = 50;
    float deg = 360.f / numSides;

    glm::vec3 p(0.5f, 0.f, 1.f);

    for (int i = 0; i < numSides; i++)
    {
        glm::mat3 M = glm::rotate(glm::mat3(), i * deg);
        pos.push_back(M * p);

    }

    unsigned int n = numSides - 2;
    for (unsigned int j = 0; j < n; ++j)
    {
        idx.push_back(0);
        idx.push_back(j+1);
        idx.push_back(j+2);
    }

    // std::cout << "Finished Generating the Index..." << std::endl;


    // Set indexBufferLength to the total number of
    // indices used to draw your polygon so that it
    // can be passed to glDrawElements in the
    // drawGeometry function.
    indexBufferLength = idx.size();

    // TODO: Use glGenBuffers, glBindBuffer, and glBufferData
    // to set up the GPU-side storage for your geometry data
    // via bufferPosition and bufferIndex.
    // Don't forget to use the GL_ARRAY_BUFFER flag for bufferPosition,
    // but use GL_ELEMENT_ARRAY_BUFFER for bufferIndex since it represents
    // triangle indices rather than mesh vertices.
    // position buffer
    glGenBuffers(1, &bufferPosition);
    glBindBuffer(GL_ARRAY_BUFFER, bufferPosition);
    glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec3), pos.data(), GL_STATIC_DRAW);
    MyGL::printGLErrorLog();

    glGenBuffers(1, &bufferIndex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);
    MyGL::printGLErrorLog();
}

void MyGL::initializeGL() {
    // Create an OpenGL context using Qt's
    // QOpenGLFunctions_3_2_Core class.
    // If you were programming in a non-Qt context
    // you might use GLEW (GL Extension Wrangler)
    // or GLFW (Graphics Library Framework) instead.
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();
    // Create a Vertex Array Object so that we can render
    // our geometry using Vertex Buffer Objects.
    glGenVertexArrays(1, &vao);
    // We have to have a VAO bound in OpenGL 3.2 Core.
    // But if we're not using multiple VAOs, we
    // can just bind one once.
    glBindVertexArray(vao);

    createAndCompileShaderProgram(this->vertShader, this->fragShader, this->shaderProgram,
                                  "passthrough.vert.glsl", "coloring.frag.glsl");
    printGLErrorLog();
    getHandlesForShaderVariables(this->shaderProgram);
    printGLErrorLog();

    initializeAndBufferGeometryData();
    printGLErrorLog();
}

void MyGL::resizeGL(int w, int h) {
    w *= this->devicePixelRatio();
    h *= this->devicePixelRatio();

    // TODO: Add code here to pass your screen dimensions to
    // your vertex shader's uniform variable that stores that information.
    // You'll need to use the glUniform2i OpenGL API function.
    GLuint screenDimUniformHandle = shaderUniformVariableHandles["u_ScreenDimensions"];
    glUniform2i(screenDimUniformHandle, w, h);
    glUseProgram(shaderProgram);
}

void MyGL::drawGeometry() {
    glUseProgram(shaderProgram);

    // TODO: Use the functions listed below to
    // associate bufferPosition with the shader
    // "in" variable named "vs_Pos", and tell
    // OpenGL that every three consecutive floats
    // in the buffer represent one vec3.
    // - glBindBuffer
    // - glEnableVertexAttribArray
    // - glVertexAttribPointer

    // TODO: Use the functions listed below to
    // draw your mesh using the triangle indices stored
    // in bufferIndex.
    // - glBindBuffer
    // - glDrawElements

    // TODO: Use glDisableVertexAttribArray
    // after drawing your geometry to "lock"
    // the GPU-side variable named vs_Pos.
    static unsigned int x = 0;
    static float translationX = -1.f;
    static float translationY = -1.f;
    static bool incX = true;
    static bool incY = true;
    static unsigned int bounces = 1;
    x++;

    // if(bounces % 4) {
    //     m_vertices = ((m_vertices++ % 20) + 3);
    //     initializeAndBufferGeometryData();
    // }

    glBindBuffer(GL_ARRAY_BUFFER, bufferPosition);
    GLint attrPos = glGetAttribLocation(shaderProgram, "vs_Pos");
    glEnableVertexAttribArray(attrPos);
    glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndex);
    glDrawElements(GL_TRIANGLES, indexBufferLength, GL_UNSIGNED_INT, 0);



    // scale based on time
    // float scaleFactor = 1.0 + 0.5 * sin(currTime)/2;


    float scaleFactor = 0.1;
    // float scaleFactor = 1.0 + ((int)currTime % 10) / 2;
    // glm::mat3 modelScale = glm::mat3(scaleFactor * ((translationX + 1) * 10), 0, 0,
    //                                  0, scaleFactor * ((translationY + 1) * 10), 0,
    //                                  0, 0, 1);

    glm::mat3 modelScale = glm::mat3((scaleFactor / (std::max(abs(translationX), 0.05f))), 0, 0,
                                     0, (scaleFactor / (std::max(abs(translationY), 0.05f))), 0,
                                                                                  0, 0, 1);

    // rotate based on time
    float angle = 0.f;
    if(incX) {
        angle = (incY) ? 45.f : -45.f;
    } else {
        angle = (incY) ? 135.f : -135.f;
    }
    // glm::mat3 modelRotate = glm::rotate(glm::mat3(), angle);
    glm::mat3 modelRotate = glm::rotate(glm::mat3(), 0.f);

    // translation based on time
    // float translationX = (sin(currTime)/2 - 0.1);
    // float translationY = (cos(currTime)/2);
    if(x == 5) {
        // if(abs(translationX) <= 1.f) {
        //     if(incX) {
        //         translationX += (40.f / WIDTH);
        //     } else {
        //         translationX -= (40.f / WIDTH);
        //     }
        // } else {
        //     !incX;
        //     if(incX) {
        //         translationX += (40.f / WIDTH);
        //     } else {
        //         translationX -= (40.f / WIDTH);
        //     }
        // }
        // if(abs(translationY) <= 1.f) {
        //     if(incY) {
        //         translationY += (40.f / HEIGHT);
        //     } else {
        //         translationY -= (40.f / HEIGHT);
        //     }
        // } else {
        //     !incY;
        //     if(incY) {
        //         translationY += (40.f / HEIGHT);
        //     } else {
        //         translationY -= (40.f / HEIGHT);
        //     }
        // }

        // translationX = (translationX <= 1.f) ? translationX + (40.f / WIDTH) : -1;
        // translationY = (translationY <= 1.f) ? translationY + (40.f / HEIGHT) : -1;

        if(abs(translationX) <= 1.f) {
            float inc = (incX) ? (10.f / WIDTH) : (-10.f / WIDTH);
            translationX += (inc);
        } else {
            if(translationX > 1.f) {
                incX = false;
                translationX = 1 - (10.f / WIDTH);
            } else {
                incX = true;
                translationX = -1 + (10.f / WIDTH);
            }
            // bounces++;
        }

        if(abs(translationY) <= 1.f) {
            float inc = (incY) ? (10.f / HEIGHT) : (-10.f / HEIGHT);
            translationY += (inc);
        } else {
            if(translationY > 1.f) {
                incY = false;
                translationY = 1 - (10.f / HEIGHT);
            } else {
                incY = true;
                translationY = -1 + (10.f / HEIGHT);
            }
            // bounces++;
        }

        x = 0;
    }

    glm::mat3 modelTranslation = glm::mat3(1, 0, 0,
                                           0, 1, 0,
                                           translationX, translationY, 1);

    glm::mat3 modelMatrix = modelTranslation * modelRotate * modelScale;

    GLuint modelUniformHandle = shaderUniformVariableHandles["u_Model"];
    glUniformMatrix3fv(modelUniformHandle, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // lock GPU variable
    glDisableVertexAttribArray(attrPos);
    printGLErrorLog();
}

void MyGL::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawGeometry();
}

void MyGL::mouseMoveEvent(QMouseEvent *e) {
    // Extra credit: Pass the mouse event's position
    // as an X and Y coordinate to the shader program's
    // mouse position uniform variable. Then, use the mouse
    // position to affect your scene in some way.
    // Don't forget to call glUseProgram first!
    glm::ivec2 pos(e->pos().x(), e->pos().y());
    glUseProgram(shaderProgram);

    GLuint mouseEventHandle = shaderUniformVariableHandles["u_MousePosition"];
    glUniform2i(mouseEventHandle, pos.x, pos.y);

    update();
}

void MyGL::tick() {
    // TODO: Add code to update your shader's
    // u_Time variable.

    // Do not delete this function call! This
    // repaints the GL viewport every time the timer
    // invokes tick().
    currTime += 0.01f;

    glUseProgram(shaderProgram);

    GLuint timeUniformHandle = shaderUniformVariableHandles["u_Time"];
    glUniform1f(timeUniformHandle, currTime);
    update();
}
