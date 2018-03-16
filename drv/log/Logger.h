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
     /*
     initializes Serial communications (baud rate: 9600)
     */
     Logger(char* level);

     /*
     stores logging level
     */
     char* lvl;

     /*
     sets logging:
     "off" - nothing except globals
     "error" - only errors
     "info" - all messages
     */
     void setLevel(char* level);

     /*
     info log message
     */
     void logi(char* message);

     /*
     error log message
     */
     void loge(char* message);

     /*
     global log message
     */
     void logg(char* message);

};

#endif