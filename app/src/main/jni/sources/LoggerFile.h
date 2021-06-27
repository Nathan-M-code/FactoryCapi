//
// Created by natha on 14/12/2020.
//

#ifndef FACTORYCAPICLION_LOGGERFILE_H
#define FACTORYCAPICLION_LOGGERFILE_H

#include <string>
#include <fstream>
#include <ctime>
#include <chrono>

class LoggerFile
{
public:
    LoggerFile();
    ~LoggerFile();

    static bool init(const std::string& nameFile);
    static bool quit();

    static void write( const std::string& msg );

private:
    static std::ofstream file_;
};


#endif //FACTORYCAPICLION_LOGGERFILE_H
