#ifndef GLOBAL
#define GLOBAL
#include <string>
#include "../ConfigFile/ConfigFile.h"
#include <log4cxx/logger.h>

using namespace std;

class Global
{
private:
    static log4cxx::LoggerPtr logger;
	//static string configFile;
public:
	static ConfigFile config;
	static void loadConfig();
};
#endif
