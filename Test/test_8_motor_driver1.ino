//14051012도진우
#define PIN_EN 11
#define PIN_IN1 10
#define PIN_IN2 9

void setup()
{
  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_EN, HIGH);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
}

void loop()
{
  // IN2를 LOW로 지정하면 정방향 IN1에 적은 숫자일수록 느림 높은 숫자일수록 빠름
  digitalWrite(PIN_IN2, LOW); 
  
  for(int i=0;i<256;i++){
    analogWrite(PIN_IN1, i);
    delay(30);
  }
  analogWrite(PIN_IN1, 0);
  delay(1000);
  
  
  // IN2를 HIGH로 지정하면 정방향 IN1에 높은 숫자일수록 느림 낮은 숫자일수록 빠름
  digitalWrite(PIN_IN2, HIGH); 
  
  for(int i=255;i>=0;i--){
    analogWrite(PIN_IN1, i);
    delay(30);
  }
  analogWrite(PIN_IN1, 255);
  delay(1000);
}