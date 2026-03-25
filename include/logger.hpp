#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


class Logger {

    private:
        std::ofstream myfile;
        static inline int iterator = 1;


    public:
        Logger(std::string filename = "log.txt");
        void log(const std::string& message, const std::string& type, const std::string& file = __FILE__, const std::string& function = __func__);

        int iterate();


        ~Logger();
};