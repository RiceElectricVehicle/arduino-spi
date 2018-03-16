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

        void setLogging(char* level);

        // storage for polling all addresses
        unsigned int currentRegisterValues[8];

        // default register values
        unsigned int initRegs[];

        
       
        void initDiagnostic(int desiredRegs[]);

        // const int TORQUE;
        // const int OFF;
        // const int BLANK;
        // const int DECAY;
        // const int DRIVE;
        // const int STATUS;
        

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


        void open();

        void close();

        unsigned int spiReadReg(unsigned int adress);

        boolean spiWriteReg(unsigned int address, unsigned int value);

        void spiGetCurrentRegisterValues();

        boolean checkValsANDBitMask(unsigned int val1, unsigned int val2, unsigned int mask);

        boolean checkCTRL(unsigned int actual, unsigned int desired);

        boolean checkTORQUE(unsigned int actual, unsigned int desired);

        boolean checkOFF(unsigned int actual, unsigned int desired);

        boolean checkBLANK(unsigned int actual, unsigned int desired);
        
        boolean checkDECAY(unsigned int actual, unsigned int desired);

        boolean checkDRIVE(unsigned int actual, unsigned int desired);

        boolean checkSTATUS(unsigned int actual, unsigned int desired);

        boolean checkALL(int actualRegs[], int desiredRegs[]);
    
};

#endif
