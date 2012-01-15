#include <QApplication>
#include <QMainWindow>
#include <GL/glut.h>
#include "ui_GuiControls.h"
#include "guiactions.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]) {
	// if you want to start reading the code, 
	// you might want to look in guiactions.cc
	// from there, actions start ;)
	
	// external variable in ssl_refbox_rules.h
	argv_global = argv[0];

	// initialize qt app and window
	QApplication app(argc, argv);
	QMainWindow* refbox = new QMainWindow;
	Ui::GuiControls* gui = new Ui::GuiControls;
	GuiActions act(gui, refbox);
	gui->setupUi(refbox);

	// connect GUI components with actions
	act.connectActions();

	// initialize the GLUT library
	glutInit(&argc, argv);

	// display MainWindow
	refbox->show();

	//main stuff
	int res = app.exec();

	return res;
}
