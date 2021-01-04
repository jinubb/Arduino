//14051012도진우
#define PIN_EN 11
#define PIN_IN1 10
#define PIN_IN2 9
int ctime;
void setup()
{
  Serial.begin(9600);
  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_EN, HIGH);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(A0, INPUT);
  
  digitalWrite(PIN_IN2, LOW);
  analogWrite(PIN_IN1, 200);
}

void loop()
{
  /*
  int temp = analogRead(A0);
  int temp_pwm = map(temp, 0, 1023, -255, 255);
  //정방향 회전
  if(temp_pwm > 0){
    digitalWrite(PIN_IN2, LOW);
  }
  //역방향 회전
  else{
    digitalWrite(PIN_IN2, HIGH);
  }
  analogWrite(PIN_IN1, temp_pwm);
  */
  
  ctime = millis();
  if(ctime %1000 ==0)
    digitalWrite(PIN_EN, !digitalRead(PIN_EN));
}