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
public:
	static ConfigFile config;
	static void loadConfig();
};
#endif
