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
#include "libraries/drv/drv.h"
#include "libraries/drv/drv.cpp"
#define MOSI 11 
#define MISO 12 
#define CLK 13
#define SCS 8 
#define LED 2 // diagnostic LED
#define SLEEP 7

//**** Configure the Motor Driver's Settings ****//

 // initialize drv object
drv sailboat(MOSI, MISO, CLK, SCS, LED);

void setup(){
  Serial.begin(9600);

  pinMode(SCS, OUTPUT); pinMode(MOSI, OUTPUT); pinMode(MISO, OUTPUT); pinMode(CLK, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(SCS, LOW); 
  pinMode(SLEEP, OUTPUT);
  digitalWrite(SLEEP, HIGH);
  
  // run diagnostic
  sailboat.setLogging("info");
  // sailboat.regDiagnostic(sailboat.initRegs);
  // sailboat.read(sailboat.CTRL);
  // sailboat.setHbridge("on");
  // sailboat.setISGain(10); 
  delay(1000);
  sailboat.setHbridge("off");
  //sailboat.setISGain(10);

    
}

void loop(){
  sailboat.read(0x00);
  //delay(250);
  
}


  
