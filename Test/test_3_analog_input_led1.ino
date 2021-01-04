void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
}

void loop()
{
  int DAC_val = analogRead(A0);
  int input_LED = DAC_val >> 2;
  analogWrite(13, input_LED);
  Serial.println(String("LED bright(0~255) : ")+input_LED);
  delay(1000); // Wait for 1000 millisecond(s)
}