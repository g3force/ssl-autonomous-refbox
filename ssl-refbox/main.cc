#include <QApplication>
#include <QMainWindow>
#include <GL/glut.h>
#include "ui_GuiControls.h"
#include "guiactions.h"
#include <iostream>
#include <stdio.h>
#include "ssl_refbox_rules.h"
#include <string.h>

int main(int argc, char* argv[])
{
    argv_global = argv[0];//externe Variable in ssl_refbox_rules.h

    QApplication app(argc,argv);
    QMainWindow* refbox = new QMainWindow;
    Ui::GuiControls* gui = new Ui::GuiControls;
    GuiActions act(gui, refbox);
    gui->setupUi(refbox);
    act.connectActions();

    glutInit(&argc, argv);

    refbox->show();

    //main stuff
    int res = app.exec();

    return res;
}
