#include <SPI.h>
#include <Arduino.h>
#include "drv.h"


Logger logger("error");

drv::drv(int out, int in, int clk, int select, int led) {


    pinMode(out, OUTPUT);
    pinMode(in, INPUT);
    pinMode(clk, OUTPUT);
    pinMode(select, OUTPUT);
    pinMode(led, OUTPUT);

    _MOSI = out;
    _MISO = in;
    _SCLK = clk;
    _SCS = select;
    _LED = led;

    unsigned int initRegs[] = {
         0x301, // B001100000001  CTRL
         0x0FF, // B000011111111  TORQUE
         0x130, // B000100110000  OFF
         0x080, // B000010000000  BLANK
         0x010, // B000000010000  DECAY
         0x000, // B000000000000  RESERVED register (unused)
         0xFA5, // B111110100101  DRIVE
         0x000, // B000000000000  STATUS
    };    
    
    unsigned int currentRegisterValues[8];

}

// short hand to initialize spi comms
void drv::open() {
    SPI.beginTransaction(SPISettings(140000, MSBFIRST, SPI_MODE0));
    digitalWrite(_SCS, HIGH);
}

// short hand to end spi comms
void drv::close() {
    digitalWrite(_SCS, LOW);
    SPI.endTransaction();
}

unsigned int drv::spiReadReg(unsigned int address) {
    /*
     Read from a register over SPI using Arduino SPI library.

     Args: address -> int 0xX where X <= 7
     Return: integer representing register value. 

     Example:  data = spiReadReg(0x6);
    */ 
    unsigned int value;
    open(); // init comms
    address = address << 12; // allocate zeros for data
    address |= 0x8000; // set MSB to read (1)

    value = SPI.transfer16(address); // transfer read request, recieve data
    
    close(); // close comms
    return value;
}

boolean drv::spiWriteReg(unsigned int address, unsigned int value) {
 /*
  Write to register over SPI using Arduino SPI library.

  address : int 0xX where X <= 7, 
  value : int to be written (as binary) to register. (12 bits)
  returns : true if write successful, false otherwise
  Example:  spiWriteReg(0x6, 0x0FF0);

  */
  unsigned int packet;

  open(); // open comms
  address = address << 12; // build packet skelleton
  address &= ~0x8000; // set MSB to write (0)
  packet = address | value;

  SPI.transfer16(packet);

  unsigned int confirmation = spiReadReg(address); // read sent data

  close(); // close comms

  return confirmation == value;

}

void drv::spiGetCurrentRegisterValues (){
  /*
  Populate currentRegisterValues variable with the integers returned from
  spiReadReg at each memory register 0-7.
  */

  //populate currentRegisterValues array 
  for (int i = 0; i < 7; i++){
      currentRegisterValues[i] = spiReadReg(i);
  }    
}

boolean drv::checkValsANDBitMask (unsigned int val1, unsigned int val2, unsigned int mask) {
  /*
  compares two values after ANDing them with bitmask mask.
  */
  if(val1 & mask == val2 & mask) {
    return true;
  } else {
    return false;
  }
}

boolean drv::checkCTRL(unsigned int actual, unsigned int desired) {
  /*
  Check the CTRL register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0)
  returns : true if actual matches desired 
  */
  
  // check DTIME and ISGAIN                         and ENBL 
 return checkValsANDBitMask(actual, desired, 0xF00) &&  checkValsANDBitMask(actual, desired, 0x1);
}

boolean drv::checkTORQUE(unsigned int actual, unsigned int desired) {
  /*
  Check the TORQUE register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
  
  // check TORQUE 
  return checkValsANDBitMask(actual, desired, 0xFF);

}

boolean drv::checkOFF(unsigned int actual, unsigned int desired) {
  /*
  Checks the OFF register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
 
 // checks TOFF                                    and PWMMODE (should always be 1)
  return checkValsANDBitMask(actual, desired, 0xFF) && checkValsANDBitMask(actual, 0x100, 0x100);
}

boolean drv::checkBLANK(unsigned int actual, unsigned int desired) {
  /*
  Checks the BLANK register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
  return checkValsANDBitMask(actual, desired, 0xFF);
}

boolean drv::checkDECAY(unsigned int actual, unsigned int desired) {
  /*
  Checks the DECAY register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */

  // check DECAY                                           and DECMOD
  return checkValsANDBitMask(actual, desired, 0xFF) && checkValsANDBitMask(actual, desired, 0x700);
}

boolean drv::checkDRIVE(unsigned int actual, unsigned int desired) {
 /*
  Checks the DECAY register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits)
  returns : true if actual matches desired 
  */
  // no bitmask needed
  return actual == desired;
}

boolean drv::checkSTATUS(unsigned int actual, unsigned int desired) {
  /*
  Checks the STATUS register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits)
  returns : true if actual matches desired 
  */
  return checkValsANDBitMask(actual, desired, 0x3F);
}

boolean drv::checkALL(int actualRegs[], int desiredRegs[]) {
  return (checkCTRL(actualRegs[CTRL], desiredRegs[CTRL]) 
          && checkTORQUE(actualRegs[TORQUE], desiredRegs[TORQUE])
          && checkOFF(actualRegs[OFF], desiredRegs[OFF])
          && checkBLANK(actualRegs[BLANK], desiredRegs[BLANK])
          && checkDECAY(actualRegs[DECAY], desiredRegs[DECAY])
          && checkDRIVE(actualRegs[DRIVE], desiredRegs[DECAY])
          && checkSTATUS(actualRegs[STATUS], desiredRegs[STATUS]));
          
}

// public functions

void drv::initDiagnostic(int desiredRegs[]) {
  /*
  If after drv powerup, registers are not default valued, _LED  goes high

  desiredRegs : can be any array of 7 12 bit numbers to check correct config of the registers
  */

  spiGetCurrentRegisterValues();
  // Serial.println(1);
  // Serial.println(currentRegisterValues);
  // Serial.println(checkALL(currentRegisterValues, initRegs));

if (checkALL(currentRegisterValues, desiredRegs)) {
    digitalWrite(_LED, LOW);
  } else {
    logger.loge("default registers not loaded correctly");
    digitalWrite(_LED, HIGH);
  }
}

void drv::setLogging(char* level) {
  logger.setLevel(level);
}
  


// TODO : build functions to toggle settings via simple calls: drv.setDTIME(0/1/2/3). drv.setDECMODE("auto")