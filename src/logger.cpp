#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "logger.hpp"

#define FILELOG(file, timestamp, message, function) myfile << "[FILE] " << file << " - [FUNCTION] " << function << " - [TIME] " << ctime(&timestamp) << " - [MESSAGE] " << message << std::endl << std::endl;


Logger::Logger(const std::string filename){
    myfile.open(filename, std::ios::out);
    if (!myfile.is_open()){
        log("Error opening file ", "ERROR", __FILE__, __func__);
    }
}

int Logger::iterate(){
    return iterator++;
}


void Logger::log(const std::string& message, const std::string& type, const std::string& file, const std::string& function){    
    time_t timestamp;
    if (type == "ERROR"){
        myfile << iterate() << ". [" << "ERROR" << "] - ";
        FILELOG(file,timestamp, message, function);
    }
    else if (type == "DEBUG"){
        myfile << iterate() << ". [" << "DEBUG" << "] - ";
        FILELOG(file, timestamp, message, function);
    }
    else {
        myfile << iterate() << "INVALID TYPE" << std::endl;
    }

}



Logger::~Logger(){
    myfile.close();
    std::cout << "-" << std::endl;
}



