#include<Arduino.h>
#include"Logger.h"


Logger::Logger(char* level) {
    lvl = level;
}

void Logger::setLevel(char* level) {
    if (level == "info") {
        lvl = "info";
    } else if (level == "error") {
        lvl = "error";
    } else {
        lvl = "off";
    }
}

void Logger::logi(char* message) {
    if (lvl == "info") {
        Serial.print("INFO: ");
        Serial.println(message);
    }
}

void Logger::loge(char* message) {
    if (lvl == "info" || lvl == "error") {
        Serial.print("ERROR: ");
        Serial.println(message);
    }
}