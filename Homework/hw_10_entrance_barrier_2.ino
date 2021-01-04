//14051012 도진우
#include <SoftwareSerial.h>

SoftwareSerial UART(8,9);
  
void setup()
{
  Serial.begin(9600);
  UART.begin(2400);
}

void loop()
{
  if(Serial.available()){
    char temp = Serial.read();
    Serial.println(String("Send message : ")+temp);
    UART.write(temp);
  }
}