#define PIN_INT 2

volatile unsigned long c_time = 0;
volatile int enc = 0;
unsigned long ref_time = 0;

void isr_encoder(){
  c_time = millis();
  enc++;
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_INT, INPUT_PULLUP);
  attachInterrupt(0, isr_encoder, FALLING);
}

void loop()
{
  if(enc){
    int diff = c_time - ref_time;
    Serial.println(String("rpm = ") + 60*1000/diff);
    ref_time = c_time;
    enc = 0;
  }
}