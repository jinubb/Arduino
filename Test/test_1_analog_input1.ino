void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop()
{
  int volt = analogRead(A0);
  Serial.println(String("ADC value = ")+volt);
  delay(1000);
}