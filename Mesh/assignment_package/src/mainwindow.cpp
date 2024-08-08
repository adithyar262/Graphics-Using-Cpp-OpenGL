#include "mainwindow.h"
#include <iostream>
#include <ui_mainwindow.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)//,
    // v(QString("bcd"))
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()),
            this, SLOT(slot_makelist()));

    connect(ui->mygl, SIGNAL(sig_sendListItem(QListWidgetItem*, uint)),
            this, SLOT(sig_addListItem(QListWidgetItem*, uint)));
    // ui->mygl->setFocus();
    connect(ui->R_Slider, SIGNAL(valueChanged(int)),
            this, SLOT(slot_changeColorR(int)));

    connect(ui->facesListWidget, SIGNAL(clicked()),
            this, SLOT(slot_FaceIsClicked(QListWidgetItem*)));
    /*
    connect(ui->G_Slider, SIGNAL(valueChanged(int)),
            this, SLOT(slot_changeValueG(int)));
    connect(ui->B_Slider, SIGNAL(valueChanged(int)),
            this, SLOT(slot_changeValueB(int)));
    connect(ui->A_Slider, SIGNAL(valueChanged(int)),
            this, SLOT(slot_changeValueA(int)));*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    // QApplication::exit();
    ui->mygl->loadObj();
}

void MainWindow::on_actionLoad_Load_Obj_triggered()
{
    ui->mygl->loadObj();
}

void MainWindow::actionLoad_Object_File_Ctrl_F()
{
    // ui->mygl->loadObj();
    QApplication::exit();
}

void MainWindow::slot_LoadObjFile(void) {

    // ui->mygl->makeList();
    // v.addElement();
    return;
}

void MainWindow::slot_makelist() {
    // ui->mygl->makeList();
    ui->mygl->loadObj();
}

void MainWindow::sig_addListItem(QListWidgetItem* i, unsigned int comp) {

    switch(comp) {
    case MESH_FACE : {
        ui->facesListWidget->addItem(i);
        break;
    }
    case MESH_EDGE : {
        ui->halfEdgesListWidget->addItem(i);
        break;
    }
    case MESH_VERTEX : {
        ui->vertsListWidget->addItem(i);
        break;
    }
    default : {
        break;
    }
    }

    // ui->vertsListWidget->addItem(i);
    // v.addElement();
    return;
}

void MainWindow::slot_list() {
    // for(int i = 0; i < 10; ++i) {
    //     uPtr<MeshFace> item = mkU<MeshFace>(QString("Item ") + QString::number(i));
    //     ui->facesListWidget->addItem(item.get());
    //     face.push_back(std::move(item));
    // }
}

void MainWindow::slot_changeColorR(int i) {
    // emit sig_TX(i);
    // TranslateSceneX = i;
    ui->mygl->updateColor(i);
    update();
}

void MainWindow::slot_FaceIsClicked(QListWidgetItem* face) {
    // emit sig_TX(i);
    std::cout<<"";
    // ui->mygl->updateColor(i, 1);
    update();
}

// void MainWindow::slot_changeColorG(int i) {
//     // emit sig_TX(i);
//     // TranslateSceneX = i;
//     ui->mygl->updateColor(i, 1);
//     update();
// }

// void MainWindow::slot_changeColorB(int i) {
//     // emit sig_TX(i);
//     // TranslateSceneX = i;
//     ui->mygl->updateColor(i, 2);
//     update();
// }

// void MainWindow::slot_changeColorA(int i) {
//     // emit sig_TX(i);
//     // TranslateSceneX = i;
//     ui->mygl->updateColor(i, 0);
//     update();
// }
