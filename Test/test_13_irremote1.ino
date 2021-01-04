#include<IRremote.h>
#define PIN_IR 12
#define PIN_LED 13

IRrecv irrecv(PIN_IR);
decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_IR, INPUT);
  irrecv.enableIRIn();
}

void loop()
{
  if(irrecv.decode(&results)){
    unsigned int val = results.value;
    Serial.println("Receive Signal");
    switch(val){
      case 2295:
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
        break;
      default:
        Serial.println(String("Wrong val :"+String(val)));
        break;
      }
    irrecv.resume();
  }
}