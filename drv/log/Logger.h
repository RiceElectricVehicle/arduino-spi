/*
    Logger.h - simple logging library for REV projects

    Created by REV for SEM. March 16, 2018

    Outputs log messages to serial port

    Usage:
    initialize a Logger object:
    Logger logger(char* level);
    logger.setLevel(error/info/off);
    logger.logi("info message");
    logger.loge("error message");

*/

#ifndef Logger_h
#define Logger_h

#include <Arduino.h>

class Logger {

    public: 
    Logger(char* level);

    char* lvl;

    void logi(char* message);

    void loge(char* message);

    void setLevel(char* level);


};

#endif