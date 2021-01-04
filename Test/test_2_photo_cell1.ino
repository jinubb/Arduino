void setup()
{
  pinMode(A0,INPUT);
  Serial.begin(9600);
}

void loop()
{
  Serial.println(String("Photocell value : ")+analogRead(A0));
  delay(1000); // Wait for 1000 millisecond(s)
}