#include <SoftwareSerial.h>
SoftwareSerial MySerial(A0, A1);
char inStr[6]="";
int i=0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if ( Serial.available() ) {
    char msg = Serial.read();
    if(msg == '$'){
      i = 0;
    }
    else{
      inStr[i++]=msg;
    }
    if(i==4){
      Serial.write(inStr);
    }
  }
}
