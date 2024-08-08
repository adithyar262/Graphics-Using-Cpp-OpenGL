#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "mygl.h"
#include "customlist.h"
#include <vector>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slot_LoadObjFile();
    void sig_addListItem(QListWidgetItem*, uint);
    void slot_list();
    void slot_makelist();
    void slot_changeColorR(int i);
    void slot_FaceIsClicked(QListWidgetItem* face);
    /*
    void slot_changeColorG(int i);
    void slot_changeColorB(int i);
    void slot_changeColorA(int i);*/
private slots:
    void on_actionQuit_triggered();

    void on_actionLoad_Load_Obj_triggered();

    void actionLoad_Object_File_Ctrl_F();

private:
    Ui::MainWindow *ui;

    // MyGL myGL;
    // MeshVertex v;
    std::vector<uPtr<CustomList>> list;
};


#endif // MAINWINDOW_H
