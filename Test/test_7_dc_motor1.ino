void setup()
{
  Serial.begin(9600);
  pinMode(12, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(A0, INPUT);
}
int ctime;
void loop()
{
  int temp = analogRead(A0);
  int temp_pwm = map(temp,0,1023,0,255);
  analogWrite(10, temp_pwm);
  
  ctime = millis();
  if(ctime %100 ==0)
  	Serial.println("pwm val = " +String(temp_pwm));
}