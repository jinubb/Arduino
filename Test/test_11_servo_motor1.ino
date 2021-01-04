#include <Servo.h>
#define PIN_SERVO 11
Servo servo;
void setup()
{
  servo.attach(PIN_SERVO);
  pinMode(A0, INPUT);
}

void loop()
{
  int temp = analogRead(A0);
  int angle = map(temp, 0, 1023 ,0, 180);
  servo.write(angle);
}