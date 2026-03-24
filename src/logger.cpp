#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "logger.h"


void Logger::log(const std::string& message, const std::string& type){

    time_t timestamp;

    std::cout << time(&timestamp);
    std::cout << std::endl;

    if (type == "DEBUG"){

    }
    else if (type == "INFO"){

    }
    else if (type == "WARNING"){

    }
    else { // type == "ERROR"


    }


}

Logger::~Logger(){
    std::cout << "-" << std::endl;
}



