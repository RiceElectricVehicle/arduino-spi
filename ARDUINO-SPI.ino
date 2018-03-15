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

//DRV8704's register addresses
enum REGS : unsigned int {CTRL, TORQUE, OFF, BLANK, DECAY, RSVRD, DRIVE, STATUS};

//store values at each register, to compare to desired values
unsigned int currentRegisterValues[8];

//**** Configure the Motor Driver's Settings ****//
void setup(){

  //Open serial monitor
  Serial.begin(9600);

  //set input and output pins
  pinMode(SCS, OUTPUT); // active HIGH
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLK, OUTPUT);

  //Set up SPI transaction
  SPI.beginTransaction(SPISettings(140000, MSBFIRST, SPI_MODE0));

}

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
      currentRegisterValues[i] = spiRead(i);
  }    
}


boolean checkCTRL(unsigned int actual, unsigned int desired) {
  /*
  Check the CTRL register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0)
  */
  
  // check DTIME and ISGAIN and ENBL 
  if ((actual & 0xF00 == desired & 0xF00) && (actual & 1 == desired & 1)) {
    return true;
  } else {
    return false;
  }    


}
   
boolean checkTORQUE(unsigned int actual, unsigned int desired) {
  /*
  Check the TORQUE register against a desired value

  actual : the value of the the register (12 bits)
  desired : the desired value of the register (12 bits, reserved sections should be 0s)
  */
  
  // check TORQUE part of TORQUE
  if (actual & 0x7F == desired & 0x7F) {
    return true;
  } else {
    return false;
  }

}

void loop(){
  
}


  