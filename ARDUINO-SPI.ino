 /*
 * SPI interface for DRV8704 Dual H-Bridge Controller
 * 
 * 
 * The circuit 
 * - SCLK: to SCK(ICSP-3) or digital pin 13
 * - SDATI: MOSI(ICSP-4) or digital pin 11
 * - SCS: 10 (slave select pin)
 * - SDATO: MISO(ICSP-1) or digital pin 12
 * 
 * by REV Motor Controller Team 
 */

#include <SPI.h>
#include <Arduino.h>
#define DATAOUT 11 //MOSI
#define DATAIN 12 //MISO
#define SPICLK 13//sclk
#define SCS 10 //slave select
#define LED 2 // diagnostic LED

//DRV8704's register addresses
const int CTRL = 0x0;
const int TORQUE = 0x1;
const int OFF = 0x2;
const int BLANK = 0x3;
const int DECAY = 0x4;
const int DRIVE = 0x6;
const int STATUS = 0x7;

// default register values for DRV (reserved bits are zeroed)
const unsigned int initRegs[8] = {
  0x301, // B001100000001  CTRL
  0x0FF, // B000011111111  TORQUE
  0x130, // B000100110000  OFF
  0x080, // B000010000000  BLANK
  0x010, // B000000010000  DECAY
  0x000, // B000000000000  RESERVED register (unused)
  0xFA5, // B111110100101  DRIVE
  0x000, // B000000000000  STATUS
};

//store values at each register, to compare to desired values
unsigned int currentRegisterValues[8];

void spiWriteReg(unsigned int address, unsigned int value){ 
  /*
  Write to register over SPI using Arduino SPI library.

  Args: address -> int 0xX where X <= 7, value > int to be written (as binary) to register. Only 
  lower 12 bits used. 

  Example:  spiWriteReg(0x6, 0x0FF0);

  */

  digitalWrite(SCS, HIGH); // select drv

  // build and send 2-byte packet 
  address = address << 12;
  address &= ~0x8000; // set MSB to write (0)
  value |= address;
  SPI.transfer16(value);

  digitalWrite(SCS, LOW); // release drv
} 

unsigned int spiReadReg(unsigned int address){
  /*
  Read from a register over SPI using Arduino SPI library.

  Args: address -> int 0xX where X <= 7
  Return: integer representing register value. 

  Example:  data = spiReadReg(0x6);
  */
  
  unsigned int data;    
  unsigned int value;

  digitalWrite(SCS, HIGH); // select drv
  address = (address << 12); 
  address |= 0x8000; // set MSB to read (1)
  
  value = SPI.transfer16(address);  // read from address 
  
  digitalWrite(SCS, LOW); // release drv
  return value;
} 
   

void spiGetCurrentRegisterValues (){
  /*
  Populate currentRegisterValues variable with the integers returned from
  spiReadReg at each memory register 0-7.
  */

  //populate currentRegisterValues array 
  for (int i = 0; i < 7; i++){
      currentRegisterValues[i] = spiReadReg(i);
  }    
}

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
 return checkValsANDBitMask(actual, desired, 0xF00) &&  checkValsANDBitMask(actual, desired, 0x1);
}
   
boolean checkTORQUE(unsigned int actual, unsigned int desired) {
  /*
  Check the TORQUE register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
  
  // check TORQUE 
  return checkValsANDBitMask(actual, desired, 0x7F);

}

boolean checkOFF(unsigned int actual, unsigned int desired) {
  /*
  Checks the OFF register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
 
 // checks TOFF                                    and PWMMODE (should always be 1)
  return checkValsANDBitMask(actual, desired, 0x7F) && checkValsANDBitMask(actual, 0x100, 0x100);
}

boolean checkBLANK(unsigned int actual, unsigned int desired) {
  /*
  Checks the BLANK register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */
  return checkValsANDBitMask(actual, desired, 0x7F);
}

boolean checkDECAY(unsigned int actual, unsigned int desired) {
  /*
  Checks the DECAY register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  returns : true if actual matches desired 
  */

  // check DECAY                                           and DECMOD
  return checkValsANDBitMask(actual, desired, 0x7F) && checkValsANDBitMask(actual, desired, 0x700);
}

boolean checkDRIVE(unsigned int actual, unsigned int desired) {
 /*
  Checks the DECAY register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits)
  returns : true if actual matches desired 
  */
  // no bitmask needed
  return actual == desired;
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

void initDiagnostic(int actualRegs[], int desiredRegs[]) {
 /*
 If after DRV powerup, registers are not default valued, LED on pin 2 goes high
 */
if (checkALL(actualRegs, desiredRegs)) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }
}

//**** Configure the Motor Driver's Settings ****//

void setup(){

  //Open serial monitor
  Serial.begin(9600);

  //set input and output pins
  pinMode(SCS, OUTPUT); // active HIGH
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLK, OUTPUT);
  pinMode(LED, OUTPUT);

  //Set up SPI transaction
  SPI.beginTransaction(SPISettings(140000, MSBFIRST, SPI_MODE0));
  
  // update registers
  spiGetCurrentRegisterValues();

  // run diagnostic
  initDiagnostic(currentRegisterValues, initRegs);
    
}

void loop(){
  
}


  