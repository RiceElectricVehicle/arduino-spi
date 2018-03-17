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
        bool write(unsigned int address, unsigned int value);
        
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
        desiredRegs[]: array with 7 entries each with 12 bit values (one for each reg)
        */
        void regDiagnostic(int desiredRegs[]);


        // following funcs deal with CTRL register

        /*
        sets ENBL register
        value: 
            "on" - turns on h-bridge
            "off" ("on" default)
        returns true if successful
        */
        bool setHbridge(char* value);

        /*
        sets ISGAIN register
        value: 
            5 - sets ISENSE amplifier gain to 5 V/V
            10/20/40 (40 default)
        returns true if successful
        */
        bool setISGain(int value);

        /*
        sets DTIME register
        value:
            410 - sets dead time to 410 ns
            460/670/880 (410 default)
        returns true if successful
        */
        bool setDTime(int value);

        // following func deals with TORQUE register

        /*
        sets TORQUE register
        value: 0-255 - Sets full-scale output current for both H-bridges (255 default)
        returns true if successful
        */
        bool setTorque(unsigned int value);

        // following func deals with OFF register (only TOFF, PWMMODE shouldnt be modified)

        /*
        sets TOFF register
        value: 0-255 - Sets fixed off time, in increments of 525 ns
            0 - 525 ns
            48 - default
            255 - 133.8 us
        returns true if successful
        */
        bool setTOff(unsigned int value);

        // following func deals with BLANK register

        /*
        sets TBLANK register
        value: 0-255 - Sets current trip blanking time, in increments of 21 ns (128 default)
        returns true if successful
        */
        bool setTBlank(unsigned int value);

        // following funcs deal with DECAY register

        /*
        sets TDECAY register
        value: 0-255 - Sets mixed decay transition time, in increments of 525 ns (16 default)
        returns true if successful
        */
        bool setTDecay(unsigned int value);

        /*
        sets DECMODE register
        value: 
            "slow" - force slow decay at all times (default)
            "fast" - force fast decay at all times
            "mixed" - use mixed decay at all times
            "auto" - use auto mixed decay at all times
        returns true if successful
        */
        bool setDecMode(char* value);

    
};

#endif
