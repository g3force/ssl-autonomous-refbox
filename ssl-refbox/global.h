/**
 * @file global.h
 * @brief Global header file
 */
#ifndef GLOBAL
#define GLOBAL
#include <string>
#include "../ConfigFile/ConfigFile.h"
#include <log4cxx/logger.h>

using namespace std;

/**
 * @class Global
 * @brief class for storing global stuff
 */
class Global {
private:
	static log4cxx::LoggerPtr logger;
	static string getConfigPath();
	static void createDefaultConfigFile(string path);

public:
    static const std::string rulenames[42];
	static ConfigFile config;
	static char* logFile;
	static void loadConfig(string);
	static void saveConfig();
};
#endif
