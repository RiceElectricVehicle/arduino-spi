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
#define SLEEP 4
#define FAULT 7

//**** Configure the Motor Driver's Settings ****//

 // initialize drv object
drv sailboat(MOSI, MISO, CLK, SCS);

void setup(){
  Serial.begin(9600);

  pinMode(SCS, OUTPUT); pinMode(MOSI, OUTPUT); pinMode(MISO, OUTPUT); pinMode(CLK, OUTPUT);
  pinMode(FAULT, INPUT);
  pinMode(SLEEP, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(SLEEP, HIGH);
  digitalWrite(SCS, LOW); 
  // run diagnostic 
  sailboat.setLogging("info");
  // sailboat.regDiagnostic(sailboat.initRegs);
  // sailboat.read(sailboat.CTRL);
  // sailboat.setHbridge("on");
  // sailboat.setISGain(10); 
  delay(50);
  sailboat.setTorque(0x70);
  delay(50);
  sailboat.setHbridge("off");
  sailboat.setTorque(0x70);

    
}

void loop(){
  Serial.println(sailboat.getTorque());
  //delay(250);
  
}


  
