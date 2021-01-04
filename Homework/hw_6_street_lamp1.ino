//14051012 도진우
void setup()
{
  pinMode(3,OUTPUT);
  pinMode(A0,INPUT);
  Serial.begin(9600);
}
int change_num(int number){
  int temp = 255;
  return temp - number;
}
void loop()
{
  int DAC_val = analogRead(A0);
  int pwm_val = DAC_val -1; // pwm 값(0~255) 맞춰서 저장
  pwm_val = change_num(pwm_val); // 값 뒤집기
  Serial.println(String("Lamp control = ")+pwm_val);
  analogWrite(3,pwm_val);
  delay(1000);
}