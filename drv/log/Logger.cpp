#include<Arduino.h>
#include"Logger.h"


Logger::Logger(char* tag, char* level) {
    tag = tag;
    lvl = level;
    Serial.begin(9600);
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
        Serial.print(tag);
        Serial.print(" - INFO: ");
        Serial.println(message);
    }
}

void Logger::loge(char* message) {
    if (lvl == "info" || lvl == "error") {
        Serial.print(tag);
        Serial.print(" - ERROR: ");
        Serial.println(message);
    }
}

void Logger::logg(char* message) {
    Serial.print(tag);
    Serial.print(" - GLOBAL: ")
    Serial.println(message);
}

