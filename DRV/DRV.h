/*
    Drv.h - Library for communicating with a DRV8704
    Created by REV for SEM. March 15, 2018
*/
#ifndef DRV_h
#define DRV_h

#include <SPI.h>
#include <Arduino.h>


class DRV {
    public:

        DRV(int out, int in, int clk, int select, int led);
       
        void initDiagnostic(int actualRegs[], int desiredRegs[]);
   
    private:

    // pins
    const int _MOSI;
    const int _MISO;
    const int _SCLK;
    const int _SCS;
    const int _LED;
    const int CTRL;
    
    // register addresses
    const int TORQUE;
    const int OFF;
    const int BLANK;
    const int DECAY;
    const int DRIVE;
    const int STATUS;

    // storage for polling all addresses
    unsigned int currentRegisterValues[8];

    // default register values
    const unsigned int initRegs[];

    open();

    close();

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
