#define LED 12
#define CDS A0

int value;

void led_onoff(int value){
  if(value >r= 300)
    digitalWrite(LED, 1);
  else
    digitalWrite(LED, 0);
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(CDS, INPUT);
  digitalWrite(LED, 0);
}

void loop() {
  value = analogRead(CDS);
  led_onoff(value);
}
