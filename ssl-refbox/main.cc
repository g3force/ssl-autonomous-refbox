/**
 * @mainpage ssl-autonomous-refbox
 *
 * The developed System uses the SSL Shared Vision to get the Knowledge about the positions of all objects.
 * On these information a Particle Filter is run for object tracking.
 * Some heuristical functions are implemented for getting information about ball collisions,
 * and at the end a Prolog Rule System analyses if some of the rules given for the SSL are broken.
 * The feedback is given on a QT and OpenGL-based GUI.
 *
 * @author Ulfert Nehmiz
 * @author Nicolai Ommer
 * @author Daniel Andres
 * @author Felix Pistorius
 */

/**
 * @file main.cc
 * @brief This file includes the main() function that is the entrance to the application
 */
#include <QApplication>
#include <QMainWindow>
#include <GL/glut.h>
#include "ui_GuiControls.h"
#include "guiactions.h"
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/fileappender.h>

#include "global.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

LoggerPtr logger(Logger::getLogger("main"));

/**
 * @brief Returns if the given file exists
 * @param filename full path to the file
 * @return true, if file exists
 */
bool fexists(string filename) {
	std::ifstream ifile(filename.c_str());
	return ifile;
}

/**
 * @brief This is the main function of the application from where all start :)
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
	// if you want to start reading the code,
	// you might want to look in guiactions.cc
	// from there, actions start ;)

	if (fexists("log4j.conf")) {
		PropertyConfigurator::configure("log4j.conf");
	} else {
		string home = getenv("HOME");
		if (home.empty()) {
			home = "/root";
		}
		mkdir(string(home + "/.ssl-autonomous-refbox").c_str(), 0775);

		string logfile = home + "/.ssl-autonomous-refbox/refbox.log";
		LayoutPtr layout(new PatternLayout("\%6r \%-5p \%-16c{1} \%-50m \%n"));
		FileAppender * fileAppender = new FileAppender(layout, logfile, false);
		ConsoleAppender * consoleAppender = new ConsoleAppender(layout);

		helpers::Pool p;
		fileAppender->activateOptions(p);

		BasicConfigurator::configure(AppenderPtr(fileAppender));
		BasicConfigurator::configure(AppenderPtr(consoleAppender));
		Logger::getRootLogger()->setLevel(Level::getInfo());
	}

	LOG4CXX_INFO(logger, "Entering application.");

	Global::loadConfig();

	// external variable in ssl_refbox_rules.h for initializing prolog
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

	LOG4CXX_INFO(logger, "Exit application");

	return res;
}

