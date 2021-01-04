//14051012 도진우
#define LEDRED_PIN 13
#define LEDBLUE_PIN 10
#define BUTTON_PIN 2

volatile int sw_pressed;
int ledred_state = LOW;
int ledblue_state = LOW;
unsigned long time_previous, time_current;

void setup()
{
  pinMode(BUTTON_PIN , INPUT_PULLUP); // pin 2
  pinMode(LEDRED_PIN , OUTPUT); // pin 13
  pinMode(LEDBLUE_PIN , OUTPUT); // pin 10
  attachInterrupt(0,sw_isr,FALLING); // 인터럽트 추가
  time_previous = millis(); //현재시간 저장
}

void loop()
{
  time_current = millis();
  if(time_current - time_previous >= 1000){
    digitalWrite(LEDRED_PIN, ledred_state);
  	ledred_state = !ledred_state;
    time_previous = millis();
  }
  
  if(sw_pressed == 1){
    sw_pressed = 0;
    ledblue_state = !ledblue_state;
    digitalWrite(LEDBLUE_PIN, ledblue_state);
  }
}

void sw_isr(){
  sw_pressed = 1;
}