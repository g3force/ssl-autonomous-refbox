/**
 * @file global.cc
 * @brief Global source file
 */
#include "global.h"
#include "../ConfigFile/ConfigFile.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

// name logger is used in another class :o don't know why...
LoggerPtr logger42(Logger::getLogger("Global"));

/**
 * @brief global reference to the config file
 */
ConfigFile Global::config;
string Global::configFilePath;

/**
 * @brief global reference to the log file to load on start
 */
char* Global::logFile;

/**
 * @brief A list of all rule names
 */
const std::string Global::rulenames[42] = { "Robot Number exceeded", //1
		"Rule not checked", //2
		"Goalie changed", //3
		"Rule not checked", //4
		"Rule not checked", //5
		"Rule not checked", //6
		"Rule not checked", //7
		"Rule not checked", //8
		"Rule not checked", //9
		"Rule not checked", //10
		"Rule not checked", //11
		"Rule not checked", //12
		"scored with chip_kick", //13
		"Bad Position during kickoff", //14
		"Bad Position during Freekick", //15
		"Bad Position during STOPPED", //16
		"Bad Position during Penalty", //17
		"Illegal Position in own defense area", //18
		"Opponent goalie touched", //19
		"Rule not checked", //20
		"Bad Position during Freekick", //21
		"Freekick taken before whistle", //22
		"Freekick Time exceeded", //23
		"Halftime exceeded", //24
		"Rule not checked", //25
		"Timeout taken", //26
		"Too many Timeouts taken", //27
		"Timeout time exceeded", //28
		"Goal scored", //29
		"Ball off the field", //30
		"Ball rolling at freekick", //31
		"Rule not checked", //32
		"Rule does not exist", //33
		"Rule does not exist", //34
		"Rule does not exist", //35
		"Rule does not exist", //36
		"Rule does not exist", //37
		"Rule does not exist", //38
		"Rule does not exist", //39
		"Rule does not exist", //40
		"Rule does not exist", //41
		"Offside position" //42
		};

/**
 * @brief Load config file and provide it with public variable
 * Tries to load the config file from following order:
 * 1. In the current folder
 * 2. In the users home folder (/home/user/.ssl-autonomous-refbox/)
 * 3. In /etc/
 * The config file has to be called ssl-autonomous-refbox.conf
 * @param custConfig custom config file to try first
 */
void Global::loadConfig(string custConfig) {
	if (custConfig.empty()) {
		setConfigPath();
	} else {
		configFilePath = custConfig;
	}

	std::ifstream in(configFilePath.c_str());
	if (in) {
		in >> Global::config;
		LOG4CXX_INFO( logger42, "ConfigFile " + configFilePath + " loaded.");
	} else {
		LOG4CXX_DEBUG( logger42, "ConfigFile " + configFilePath + " not found.");
	}
}

/**
 * @brief Save config file.
 */
bool Global::saveConfig() {

	std::ofstream out(configFilePath.c_str());
	if (out) {
		out << Global::config;
		LOG4CXX_INFO( logger42, "ConfigFile " + configFilePath + " saved.");
		return true;
	}
	return false;
}

/**
 * @brief set config path, if there no config a new config file will be created
 * The config file has to be called ssl-autonomous-refbox.conf
 */
void Global::setConfigPath() {
	string configFile = "ssl-autonomous-refbox.conf";

	string home = getenv("HOME");
	if (home.empty()) {
		home = "/root";
	}

	configFilePath = home + "/.ssl-autonomous-refbox/" + configFile;

	std::ifstream in(configFilePath.c_str());
	if (in) {
		in.close();
		LOG4CXX_INFO( logger42, "ConfigFile " + configFilePath + " found.");
	} else {
		LOG4CXX_INFO( logger42, "No ConfigFile found.");
		createDefaultConfigFile(configFilePath);
	}
}

/**
 * @brief create a config file with default values
 */
void Global::createDefaultConfigFile(string path) {
	config.add("ssl_vision_ip", "224.5.23.2");
	config.add("ssl_vision_port", "10002	");

	config.add("refbox_ip", "224.5.23.1");
	config.add("refbox_port", "10001");

	config.add("cam_height", "580");
	config.add("cam_width", "780");

	struct stat st;
	if (stat(path.c_str(), &st) != 0) {
		char* confPath = new char[path.length()];
		strcpy(confPath, path.c_str());
		mkdir(dirname(confPath), S_IRWXU | S_IRWXG | S_IRWXO);
	}

	if (saveConfig()) {
		LOG4CXX_INFO( logger42, "Default ConfigFile " + path + " created.");
	} else {
		LOG4CXX_ERROR( logger42, "Default ConfigFile " + path + " could not be created.");
	}
}

