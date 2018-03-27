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
#include "drv.h"
#define DATAOUT 11 //MOSI
#define DATAIN 12 //MISO
#define SPICLK 13//sclk
#define SCS 10 //slave select
#define LED 2 // diagnostic LED

//**** Configure the Motor Driver's Settings ****//

 // initialize drv object
drv sailboat(DATAOUT, DATAIN, SPICLK, SCS, LED);

void setup(){
  Serial.begin(9600);
  // run diagnostic
  sailboat.setLogging("info");
  sailboat.regDiagnostic(sailboat.initRegs);
  sailboat.read(sailboat.CTRL);
  sailboat.setHbridge("on");
  sailboat.setISGain(10);
    
}

void loop(){
  //sailboat.writeTest(sailboat.TORQUE, 0x010);
  delay(250);
  
}


  
