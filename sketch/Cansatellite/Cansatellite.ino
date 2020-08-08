#include <stdio.h>
#include <string.h>
#include <SoftwareSerial.h>
#define LED 8

SoftwareSerial BTSerial(2, 3);   //bluetooth module Tx:Digital 2 Rx:Digital 3

char inStr[6]="";
int i=0;

void myFlush(){
  //  버퍼에 있는 데이터를 다 지운다.
  while(Serial.available()>0)
    Serial.read();
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
  fflush(stdin);
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
      if(strcmp(inStr,"tuon")==0)
        digitalWrite(LED, 1);
      else
        digitalWrite(LED, 0);

      i=0;
    }
  }
}
