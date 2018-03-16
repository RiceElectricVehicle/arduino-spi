/*
    Drv.h - Library for communicating with a DRV8704
    Created by REV for SEM. March 15, 2018

    Usage:
    declare a drv object "drv sailboat(11, 12, 13, 10, 2);"
    use publilc methods:
    "drv.setBridge(0);"
    "drv.setBridge(1);"

    Dependencies:

    REV Logger Library. (should be included)

*/
#ifndef drv_h
#define drv_h

#include <Arduino.h>
#include <SPI.h>
#include "log/Logger.h"
#include "log/Logger.cpp"

class drv {
    public:
        
        drv(int out, int in, int clk, int select, int led);
        
        // pins
        int _MOSI;
        int _MISO;
        int _SCLK;
        int _SCS;
        int _LED;
        
        
        // register addresses
        const int CTRL = 0x0;
        const int TORQUE = 0x1;
        const int OFF = 0x2;
        const int BLANK = 0x3;
        const int DECAY = 0x4;
        const int DRIVE = 0x6;
        const int STATUS = 0x7;

        unsigned int currentRegisterValues[8];

        // Default reg values
        unsigned int initRegs[];

        // functions 
        
        /*
        opens SPI bus
        */
        void open();

        /*
        closes SPI bus
        */
        void close();
        
        /*
        reads from given address
        */
        unsigned int read(unsigned int address);

        /*
        writes value to address
        returns true if successful
        */
        boolean write(unsigned int address, unsigned int value);
        
        /*
        sets logging level for DRV logger object (see Logger.h)
        */
        void setLogging(char* level);
        
        /*
        reads all registers and stores in currentRegisterValues
        */
        void getCurrentRegisters();

        /*
        confirms that all Regs have desired values
        */
        void regDiagnostic(int desiredRegs[]);

    
};

#endif
