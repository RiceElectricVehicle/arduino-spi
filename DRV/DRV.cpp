#include <SPI.h>
#include <Arduino.h>
#include "drv.h"

// initialize logging object to error
Logger logger("error");

// register addresses (for internal functions)
const int CTRL = 0x0;
const int TORQUE = 0x1;
const int OFF = 0x2;
const int BLANK = 0x3;
const int DECAY = 0x4;
const int DRIVE = 0x6;
const int STATUS = 0x7;

// constructor
drv::drv(int out, int in, int clk, int select, int led) {

  // hello world
  logger.logg("DRV8704 driver loaded");

  // set up pins
  pinMode(out, OUTPUT);
  pinMode(in, INPUT);
  pinMode(clk, OUTPUT);
  pinMode(select, OUTPUT);
  pinMode(led, OUTPUT);

  // pins
  _MOSI = out;
  _MISO = in;
  _SCLK = clk;
  _SCS = select;
  _LED = led;

  // updated with currentRegisterValues
  unsigned int currentRegisterValues[8];

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
    
  
}

// private internal funcs
boolean checkValsANDBitMask (unsigned int val1, unsigned int val2, unsigned int mask) {
  /*
  compares two values after ANDing them with bitmask mask.
  */
  if(val1 & mask == val2 & mask) {
    return true;
  } else {
    return false;
  }
}

boolean checkCTRL(unsigned int actual, unsigned int desired) {
  /*
  Check the CTRL register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0)
  returns : true if actual matches desired 
  */

  // check DTIME and ISGAIN                         and ENBL 
  if (checkValsANDBitMask(actual, desired, 0xF00) &&  checkValsANDBitMask(actual, desired, 0x1)) {
    return true;
  } else {
    logger.loge("CRTL register not ok");
    return false;
  }
}

boolean checkTORQUE(unsigned int actual, unsigned int desired) {
  /*
  Check the TORQUE register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
  
  // check TORQUE 
  if (checkValsANDBitMask(actual, desired, 0xFF)) {
    return true;
  } else {
    logger.loge("TORQUE register not ok");
    return false;
  }
}

boolean checkOFF(unsigned int actual, unsigned int desired) {
  /*
  Checks the OFF register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
 
 // checks TOFF                                    and PWMMODE (should always be 1)
  if (checkValsANDBitMask(actual, desired, 0xFF) && checkValsANDBitMask(actual, 0x100, 0x100)) {
    return true;
  } else {
    logger.loge("OFF register not ok");
    return false;
  }
}

boolean checkBLANK(unsigned int actual, unsigned int desired) {
  /*
  Checks the BLANK register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
  if (checkValsANDBitMask(actual, desired, 0xFF)) {
    return true;
  } else {
    logger.loge("BLANK register not ok");
    return false;
  }
}

boolean checkDECAY(unsigned int actual, unsigned int desired) {
  /*
  Checks the DECAY register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */

  // check DECAY                                           and DECMOD
  if (checkValsANDBitMask(actual, desired, 0xFF) && checkValsANDBitMask(actual, desired, 0x700)) {
    return true;
  } else {
    logger.loge("DECAY register not ok");
    return false;
  }
}

boolean checkDRIVE(unsigned int actual, unsigned int desired) {
 /*
  Checks the DECAY register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits)
  returns : true if actual matches desired 
  */
  // no bitmask needed
  if (actual == desired) {
    return true;
  } else {
    logger.loge("DRIVE register not ok");
    return false;
  } 
}

boolean checkSTATUS(unsigned int actual, unsigned int desired) {
  /*
  Checks the STATUS register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits)
  returns : true if actual matches desired 
  */
  return checkValsANDBitMask(actual, desired, 0x3F);
}

boolean checkALL(int actualRegs[], int desiredRegs[]) {
  return (checkCTRL(actualRegs[CTRL], desiredRegs[CTRL]) 
          && checkTORQUE(actualRegs[TORQUE], desiredRegs[TORQUE])
          && checkOFF(actualRegs[OFF], desiredRegs[OFF])
          && checkBLANK(actualRegs[BLANK], desiredRegs[BLANK])
          && checkDECAY(actualRegs[DECAY], desiredRegs[DECAY])
          && checkDRIVE(actualRegs[DRIVE], desiredRegs[DECAY])
          && checkSTATUS(actualRegs[STATUS], desiredRegs[STATUS]));
          
}

// TODO : build functions to toggle settings via simple calls: drv.setDTIME(0/1/2/3). drv.setDECMODE("auto")
// public funcs

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

unsigned int drv::read(unsigned int address) {
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

boolean drv::write(unsigned int address, unsigned int value) {
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

  unsigned int confirmation = read(address); // read sent data

  close(); // close comms

  if (confirmation == value) {
    logger.logi("Successful write at: ");
    logger.logi(address);
    return true;
  } else {
    logger.loge("unsucessful write");
    return false;
  }

}

void drv::getCurrentRegisters (){
  /*
  Populate currentRegisterValues variable with the integers returned from
  spiReadReg at each memory register 0-7.
  */

  //populate currentRegisterValues array 
  for (int i = 0; i < 7; i++){
      currentRegisterValues[i] = read(i);
  }    
}

void drv::regDiagnostic(int desiredRegs[]) {
  /*
  If after drv powerup, registers are not default valued, _LED  goes high

  desiredRegs : can be any array of 7 12 bit numbers to check correct config of the registers
  */

  getCurrentRegisters();

  if (checkALL(currentRegisterValues, desiredRegs)) {
    digitalWrite(_LED, LOW);
  } else {
    logger.loge("default registers not loaded correctly");
    digitalWrite(_LED, HIGH);
  }
}


void drv::setLogging(char* level) {
  // sets logging level for the drv logger
  logger.setLevel(level);
}
