//
// Created by natha on 14/12/2020.
//

#include "LoggerFile.h"

std::ofstream LoggerFile::file_ = std::ofstream();

LoggerFile::LoggerFile(){}
LoggerFile::~LoggerFile(){}

bool LoggerFile::init(const std::string& nameFile){
    file_.open(nameFile);
    file_.seekp( std::ios::beg );

    if (!file_.good()) return false;

    auto start = std::chrono::system_clock::now();
    std::time_t now = std::chrono::system_clock::to_time_t(start);

    file_ << "  ===============================================\n"
          << "    Begin Output log "
          << std::ctime(&now)
          << "\n  ===============================================\n\n";
    file_.flush();

    return true;
}
bool LoggerFile::quit(){
    if (!file_.good()) return false;

    auto start = std::chrono::system_clock::now();
    std::time_t now = std::chrono::system_clock::to_time_t(start);

    file_ << "\n  ===============================================\n"
          << "    End   Output log "
          << std::ctime(&now)
          << "\n  ===============================================\n\n";
    file_.flush();
    file_.close();

    return true;
}

void LoggerFile::write( const std::string& msg ){
    file_ << msg << "\n";
    file_.flush();
}
