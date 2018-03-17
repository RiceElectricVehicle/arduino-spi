#include<Arduino.h>
#include"Logger.h"


Logger::Logger(char* tagg, char* level) {
    tag = tagg;
    lvl = level;
}

void Logger::setLevel(char* level) {
    if (level == "info") {
        lvl = "info";
    } else if (level == "error") {
        lvl = "error";
    } else if (level == "global") {
        lvl = "global";
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
    if (lvl == "global" || lvl == "error" || lvl == "info") {
        Serial.print(tag);
        Serial.print(" - GLOBAL: ");
        Serial.println(message);
    }
}

bool Logger::logSet(char* reg, char* subreg, char* setting, bool success) {
    
    if (success && lvl == "info")  {
        Serial.print(tag);
        Serial.print(" - INFO: ");
        Serial.print(reg);
        Serial.print(" register subreg ");
        Serial.print(setting);
        Serial.println(" write success");
    } else if(!success && (lvl == "error" || lvl == "info")) {
        Serial.print(tag);
        Serial.print(" - ");
        Serial.print(reg);
        Serial.print(" register subreg ERROR: ");
        Serial.print(setting);
        Serial.println(" write fail");
    }

    return success;
}

bool Logger::logSet(char* reg, char* subreg, int setting, bool success){
     
     if (success && lvl == "info")  {
        Serial.print(tag);
        Serial.print(" - INFO: ");
        Serial.print(reg);
        Serial.print(" register subreg ");
        Serial.print(setting);
        Serial.println(" write success");
    } else if(!success && (lvl == "error" || lvl == "info")) {
        Serial.print(tag);
        Serial.print(" - ERROR: ");
        Serial.print(reg);
        Serial.print(" register subreg ");
        Serial.print(setting);
        Serial.println(" write fail");
    }

    return success;
}

bool Logger::logSet(char* reg, char* subreg, float setting, bool success){
     
     if (success && lvl == "info")  {
        Serial.print(tag);
        Serial.print(" - INFO: ");
        Serial.print(reg);
        Serial.print(" register subreg ");
        Serial.print(setting);
        Serial.println(" write success");
    } else if(!success && (lvl == "error" || lvl == "info")) {
        Serial.print(tag);
        Serial.print(" - ERROR: ");
        Serial.print(reg);
        Serial.print(" register subreg ");
        Serial.print(setting);
        Serial.println(" write fail");
    }

    return success;
}

bool Logger::logSet(char* reg, char* subreg, unsigned int setting, bool success){
     
     if (success && lvl == "info")  {
        Serial.print(tag);
        Serial.print(" - INFO: ");
        Serial.print(reg);
        Serial.print(" register subreg ");
        Serial.print(setting);
        Serial.println(" write success");
    } else if(!success && (lvl == "error" || lvl == "info")) {
        Serial.print(tag);
        Serial.print(" - ERROR: ");
        Serial.print(reg);
        Serial.print(" register subreg ");
        Serial.print(setting);
        Serial.println(" write fail");
    }

    return success;
}
