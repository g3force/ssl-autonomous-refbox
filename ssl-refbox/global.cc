/**
 * @file global.cc
 * @brief Global source file
 */
#include "global.h"
#include "../ConfigFile/ConfigFile.h"
#include <iostream>
#include <stdlib.h>

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace std;

// name logger is used in another class :o don't know why...
LoggerPtr logger42 ( Logger::getLogger ( "Global" ) );

/**
 * @brief global reference to the config file
 */
ConfigFile Global::config;

/**
 * @brief Load config file and provide it with public variable
 * Tries to load the config file from following order:
 * 1. In the current folder
 * 2. In the users home folder (/home/user/.ssl-autonomous-refbox/)
 * 3. In /etc/
 * The config file has to be called ssl-autonomous-refbox.conf
 */
void Global::loadConfig()
{
	string configPath = getConfigPath();

	std::ifstream in ( configPath.c_str() );
	if ( in ) {
		in >> Global::config;
		LOG4CXX_INFO ( logger42, "ConfigFile " + configPath + " loaded." );
	}
}

/**
 * @brief Save config file.
 */
void Global::saveConfig() {
	string configPath = getConfigPath();

	std::ofstream out ( configPath.c_str() );
	if ( out ) {
		out << Global::config;
		LOG4CXX_INFO ( logger42, "ConfigFile " + configPath + " saved." );
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
    string home = getenv ( "HOME" );
    if ( home.empty() ) {
        home = "/root";
    }

    string configPath[] = {configFile,
                           home + "/.ssl-autonomous-refbox/" + configFile,
                           "/etc/" + configFile
                          };


    for ( int i=0; i<3; i++ ) {
    	std::ifstream ifile( configPath[i].c_str() );

    	if( ifile ) {
    		LOG4CXX_INFO ( logger42, "ConfigFile " + configPath[i] + " found." );
    		return configPath[i];
    	}
    }

    LOG4CXX_INFO ( logger42, "No ConfigFile found." );

    createDefaultConfigFile( configPath[1] );
    return configPath[1];
}

/**
 *
 */
void Global::createDefaultConfigFile(string path) {
	// TODO

	LOG4CXX_INFO ( logger42, "Default ConfigFile " + path + " created." );
}
