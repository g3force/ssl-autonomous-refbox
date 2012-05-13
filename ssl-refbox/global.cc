/**
 * @file global.cc
 * @brief Global source file
 */
#include "global.h"
#include "../ConfigFile/ConfigFile.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

// name logger is used in another class :o don't know why...
LoggerPtr logger42(Logger::getLogger("Global"));

/**
 * @brief global reference to the config file
 */
ConfigFile Global::config;

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
	string configPath = NULL;
	if (custConfig.empty()) {
		configPath = getConfigPath();
	} else {
		configPath = custConfig;
	}

	std::ifstream in(configPath.c_str());
	if (in) {
		in >> Global::config;
		LOG4CXX_INFO( logger42, "ConfigFile " + configPath + " loaded.");
	} else {
		LOG4CXX_DEBUG( logger42, "ConfigFile " + configPath + " not found.");
	}
}

/**
 * @brief Save config file.
 */
void Global::saveConfig() {
	string configPath = getConfigPath();

	std::ofstream out(configPath.c_str());
	if (out) {
		out << Global::config;
		LOG4CXX_INFO( logger42, "ConfigFile " + configPath + " saved.");
	}
}

/**
 * @brief Locate config file and return it
 * Tries to locate the config file from following order:
 * 1. In the current folder
 * 2. In the users home folder (/home/user/.ssl-autonomous-refbox/)
 * 3. In /etc/
 * The config file has to be called ssl-autonomous-refbox.conf
 */
string Global::getConfigPath() {
	string configFile = "ssl-autonomous-refbox.conf";
	string home = getenv("HOME");
	if (home.empty()) {
		home = "/root";
	}

	string configPath[] =
			{ configFile, home + "/.ssl-autonomous-refbox/" + configFile, "/etc/" + configFile };

	for (int i = 0; i < 3; i++) {
		std::ifstream in(configPath[i].c_str());
		if (in) {
			in.close();
			LOG4CXX_INFO( logger42, "ConfigFile " + configPath[i] + " found.");
			return configPath[i];
		}
	}
	LOG4CXX_INFO( logger42, "No ConfigFile found.");

	createDefaultConfigFile(configPath[1]);
	return configPath[1];
}

/**
 * TODO create a config file with default values
 */
void Global::createDefaultConfigFile(string path) {
	LOG4CXX_INFO( logger42, "Default ConfigFile " + path + " created.");
}

