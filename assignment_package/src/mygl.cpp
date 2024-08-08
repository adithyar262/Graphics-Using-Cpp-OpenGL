#include "mygl.h"
#include <glm_includes.h>
#include <QDir>
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
    m_worldAxes(this),
    m_progLambert(this), m_progFlat(this), m_progInstanced(this),
    m_progSky(this), m_progWeather(this), m_proInventory(this), m_geomQuad(this), m_bird(this), m_weather(this), m_time(0.0f), m_cube(this),
    m_terrain(this), m_player(glm::vec3(128.f, 180.f, 128.f), m_terrain), m_texture(this)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    // Tell the timer to redraw 60 times per second
    m_timer.start(16);
    setFocusPolicy(Qt::ClickFocus);
    currentMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();

    setMouseTracking(true); // MyGL will track the mouse's movements even if a mouse button is not pressed
    setCursor(Qt::BlankCursor); // Make the cursor invisible
}

MyGL::~MyGL() {
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
}


void MyGL::moveMouseToCenter() {
    QCursor::setPos(this->mapToGlobal(QPoint(width() / 2, height() / 2)));
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
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.37f, 0.74f, 1.0f, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instance of the world axes
    m_worldAxes.createVBOdata();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    // m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");
    // m_progInstanced.create(":/glsl/instanced.vert.glsl", ":/glsl/lambert.frag.glsl");
    m_progSky.create(":/glsl/sky.vert.glsl", ":/glsl/sky.frag.glsl");
    m_progWeather.create(":/glsl/weather.vert.glsl", ":/glsl/weather.frag.glsl");
    m_proInventory.create(":/glsl/inventory.vert.glsl", ":/glsl/inventory.frag.glsl");
    m_geomQuad.createVBOdata();

    // Texture

    m_texture.create(":/textures/minecraft_textures_all.png");

    m_texture.load(0);

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);

    m_terrain.CreateTestScene();
    m_startTime = QDateTime::currentMSecsSinceEpoch();

}


void MyGL::resizeGL(int w, int h) {
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_player.setCameraWidthHeight(static_cast<unsigned int>(w), static_cast<unsigned int>(h));
    glm::mat4 viewproj = m_player.mcr_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setUnifMat4("u_ViewProj", viewproj);
    m_progFlat.setUnifMat4("u_ViewProj", viewproj);
    m_progInstanced.setUnifMat4("u_ViewProj", viewproj);
    m_progSky.setUnifMat4("u_ViewProj", glm::inverse(viewproj));
    m_progWeather.setUnifMat4("u_ViewProj", glm::inverse(viewproj));

    printGLErrorLog();

    m_progSky.useMe();
    m_progSky.setUnifVec3("u_Eye", m_player.mcr_camera.mcr_position);
    m_progSky.setUnifIntVec2("u_Dimensions", width(), height());

    m_progWeather.useMe();
    m_progWeather.setUnifVec3("u_Eye", m_player.mcr_camera.mcr_position);
    m_progWeather.setUnifIntVec2("u_Dimensions", width(), height());
}

void MyGL::setInventoryColor() {
    // GRASS, DIRT, STONE, WATER, SNOW, SAND, LAVA, BEDROCK
    if (m_player.currentSelectBlock == GRASS) {
        m_inverntoryBlockType = 0;

    } else if (m_player.currentSelectBlock == DIRT) {
        m_inverntoryBlockType = 1;

    } else if (m_player.currentSelectBlock == STONE) {
        m_inverntoryBlockType = 2;

    } else if (m_player.currentSelectBlock == WATER) {
        m_inverntoryBlockType = 3;

    } else if (m_player.currentSelectBlock == SNOW) {
        m_inverntoryBlockType = 4;

    } else if (m_player.currentSelectBlock == SAND) {
        m_inverntoryBlockType = 5;

    } else if (m_player.currentSelectBlock == LAVA) {
        m_inverntoryBlockType = 6;

    } else if (m_player.currentSelectBlock == BEDROCK) {
        m_inverntoryBlockType = 7;
    } else {
        m_inverntoryBlockType = 8;

    }

}
// MyGL's constructor links tick() to a timer that fires 60 times per second.
// We're treating MyGL as our game engine class, so we're going to perform
// all per-frame actions here, such as performing physics updates on all
// entities in the scene.
void MyGL::tick() {
    qint64 curr = QDateTime::currentMSecsSinceEpoch();
    float timeSinceStart = (curr - m_startTime) / 1000.0f;
    m_progLambert.setUnifFloat("u_Time", timeSinceStart);
    m_progSky.setUnifFloat("u_Time", timeSinceStart);
    m_progWeather.setUnifFloat("u_Time", timeSinceStart);

    float lightAngle = timeSinceStart * 0.1;  // Control speed of day-night cycle
    glm::vec3 sunDir = glm::vec3(0.0, sin(lightAngle), cos(lightAngle)); // Improved for realism
    m_progLambert.setUnifVec3("sunDir", sunDir);
    m_progSky.setUnifVec3("sunDir", sunDir);
    m_progWeather.setUnifVec3("sunDir", sunDir);

    setInventoryColor();
    m_proInventory.setUnifFloat("blockType", m_inverntoryBlockType);

    glm::vec3 prevPos = m_player.m_position;
    float dT = (curr - currentMSecsSinceEpoch) / 1000.0f;
    m_player.tick(dT, m_inputs);
    glm::vec3 curPos = m_player.m_position;

    update(); // Calls paintGL() as part of a larger QOpenGLWidget pipeline
    sendPlayerDataToGUI(); // Updates the info in the secondary window displaying player data

    m_terrain.multiTheadedWorker(curPos, prevPos, dT);


    currentMSecsSinceEpoch = curr;
}

void MyGL::sendPlayerDataToGUI() const {
    emit sig_sendPlayerPos(m_player.posAsQString());
    emit sig_sendPlayerVel(m_player.velAsQString());
    emit sig_sendPlayerAcc(m_player.accAsQString());
    emit sig_sendPlayerLook(m_player.lookAsQString());
    glm::vec2 pPos(m_player.mcr_position.x, m_player.mcr_position.z);
    glm::ivec2 chunk(16 * glm::ivec2(glm::floor(pPos / 16.f)));
    glm::ivec2 zone(64 * glm::ivec2(glm::floor(pPos / 64.f)));
    emit sig_sendPlayerChunk(QString::fromStdString("( " + std::to_string(chunk.x) + ", " + std::to_string(chunk.y) + " )"));
    emit sig_sendPlayerTerrainZone(QString::fromStdString("( " + std::to_string(zone.x) + ", " + std::to_string(zone.y) + " )"));
}

// This function is called whenever update() is called.
// MyGL's constructor links update() to a timer that fires 60 times per second,
// so paintGL() called at a rate of 60 frames per second.
void MyGL::paintGL() {
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewproj = m_player.mcr_camera.getViewProj();
    m_progLambert.setUnifMat4("u_ViewProj", viewproj);
    m_progFlat.setUnifMat4("u_ViewProj", viewproj);
    // m_progInstanced.setUnifMat4("u_ViewProj", viewproj);
    m_progSky.setUnifMat4("u_ViewProj", glm::inverse(viewproj));
    m_progWeather.setUnifMat4("u_ViewProj", glm::inverse(viewproj));

    m_progSky.useMe();
    m_progSky.setUnifVec3("u_Eye", m_player.mcr_camera.mcr_position);
    m_progSky.setUnifFloat("u_Time", m_time++);

    m_progSky.draw(m_geomQuad);

    glm::vec3 p = m_player.mcr_position + glm::vec3(0.f, sin(m_time) / 4, 0.f) / 100.f;
    m_bird.draw(&m_progLambert, p);

    renderTerrain();

    m_progWeather.useMe();
    m_progWeather.setUnifVec3("u_Eye", m_player.mcr_camera.mcr_position);
    m_progWeather.setUnifFloat("u_Time", m_time++);
    // m_weather.draw(&m_progWeather, p, m_time);

    glDisable(GL_DEPTH_TEST);

    m_proInventory.draw(m_geomQuad);
    glEnable(GL_DEPTH_TEST);
}


void MyGL::renderTerrain() {
    m_texture.bind(0);
    m_progLambert.setUnifInt("u_Texture", 0);

    m_progLambert.setUnifMat4("u_Model", glm::mat4());
    m_progLambert.setUnifMat4("u_ModelInvTr", glm::mat4());
    m_terrain.draw(&m_progLambert);
}


void MyGL::keyPressEvent(QKeyEvent *e) {
    if(e->modifiers() & Qt::ShiftModifier) m_inputs.shiftPressed = true;

    switch (e->key()) {
    case Qt::Key_Escape:
        QApplication::quit();
        break;
    case Qt::Key_Right:
        m_inputs.rightPressed = true;
        break;
    case Qt::Key_Left:
        m_inputs.leftPressed = true;
        break;
    case Qt::Key_Up:
        m_inputs.upPressed = true;
        break;
    case Qt::Key_Down:
        m_inputs.downPressed = true;
        break;
    case Qt::Key_W:
        m_inputs.wPressed = true;
        break;
    case Qt::Key_S:
        m_inputs.sPressed = true;
        break;
    case Qt::Key_D:
        m_inputs.dPressed = true;
        break;
    case Qt::Key_A:
        m_inputs.aPressed = true;
        break;
    case Qt::Key_Q:
        m_inputs.qPressed = true;
        break;
    case Qt::Key_E:
        m_inputs.ePressed = true;
        break;
    case Qt::Key_F:
        m_inputs.fPressed = true;
        break;
    case Qt::Key_R:
        m_inputs.rPressed = true;
        break;
    case Qt::Key_1:
        m_inputs.n1Pressed = true;
        break;
    case Qt::Key_2:
        m_inputs.n2Pressed = true;
        break;
    case Qt::Key_Space:
        m_inputs.spacePressed = true;
        break;
    case Qt::Key_I:
        m_inputs.iPressed = true;
        break;

    default:
        break;
    }

}

void MyGL::keyReleaseEvent(QKeyEvent *e) {
    if(!(e->modifiers() & Qt::ShiftModifier)) m_inputs.shiftPressed = false;

    switch (e->key()) {
    case Qt::Key_Right:
        m_inputs.rightPressed = false;
        break;
    case Qt::Key_Left:
        m_inputs.leftPressed = false;
        break;
    case Qt::Key_Up:
        m_inputs.upPressed = false;
        break;
    case Qt::Key_Down:
        m_inputs.downPressed = false;
        break;
    case Qt::Key_W:
        m_inputs.wPressed = false;
        break;
    case Qt::Key_S:
        m_inputs.sPressed = false;
        break;
    case Qt::Key_D:
        m_inputs.dPressed = false;
        break;
    case Qt::Key_A:
        m_inputs.aPressed = false;
        break;
    case Qt::Key_Q:
        m_inputs.qPressed = false;
        break;
    case Qt::Key_E:
        m_inputs.ePressed = false;
        break;
    case Qt::Key_F:
        m_inputs.fPressed = false;
        break;
    case Qt::Key_R:
        m_inputs.rPressed = false;
        break;
    case Qt::Key_1:
        m_inputs.n1Pressed = false;
        break;
    case Qt::Key_2:
        m_inputs.n2Pressed = false;
        break;
    case Qt::Key_Space:
        m_inputs.spacePressed = false;
        break;
    case Qt::Key_I:
        m_inputs.iPressed = false;
        break;
    default:
        break;
    }
}


void MyGL::mouseMoveEvent(QMouseEvent *e) {
    if (m_inputs.centering) {
        m_inputs.centering = false;
        return;
    }

    QPointF center = mapToGlobal(rect().center());
    QPointF delta = e->globalPosition();
    if (!delta.isNull()) {
        float sensitivity = 0.05f;
        m_inputs.mouseX = delta.x();
        m_inputs.mouseX = delta.y();
        m_player.rotateOnUpGlobal((center.x() - delta.x()) * sensitivity);
        m_player.rotateOnRightLocal((center.y() - delta.y()) * sensitivity);
    }
    m_inputs.centering = true;
    moveMouseToCenter();
}


void MyGL::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        m_player.blockOpreation(false, m_terrain, m_player.currentSelectBlock);

    } else if (e->button() == Qt::RightButton) {
        m_player.blockOpreation(true, m_terrain, m_player.currentSelectBlock);
    }
    update();
}
