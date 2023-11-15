#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

// #include "global.hpp"


// extern dataBase db;

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&now_c);

        std::lock_guard<std::mutex> lock(mutex);

        logFile << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] " << message << std::endl;
    }

private:
    Logger() {
        logFile.open("./logfile.log", std::ios::out);
    }

    ~Logger() {
        logFile.close();
    }

    Logger(const Logger&) = delete;           
    Logger& operator=(const Logger&) = delete; 

    std::ofstream logFile;
    std::mutex mutex;
};


#endif