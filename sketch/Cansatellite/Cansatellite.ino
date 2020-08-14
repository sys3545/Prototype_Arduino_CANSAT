#include <stdio.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#define LED 14

SoftwareSerial BTSerial(15, 16);   //bluetooth module Tx:Digital 15 Rx:Digital 16
Servo servo;

char inStr[6]="";
int i=0;
int mode=0;

void setup() {
  pinMode(LED, OUTPUT);
  servo.attach(9);
  Serial.begin(9600);
  BTSerial.begin(9600);
  fflush(stdin);
  servo.write(0);
  Serial.println("ATcommand");  //ATcommand Start
}

void loop() {
  if(BTSerial.available()){
    char msg = BTSerial.read();
    if(msg=='$'){
      i=0;
    }
    else{
      inStr[i++]=msg;
    }

    if(i==5){
      if(strcmp(inStr,"tuon")==0){
        mode = 1;
        digitalWrite(LED, mode);
      }
      else if(strcmp(inStr,"tuof")==0){
        mode = 0;
        digitalWrite(LED, mode);
      }
      else if(strcmp(inStr,"seon")==0){
        servo.write(60);
      }
      else{
        servo.write(0);
      }

      i=0;
    }
  }
  //BTSerial.write(mode);
}
