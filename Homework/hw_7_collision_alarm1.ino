//14051012 도진우 HW #7 Collision Alarm
#define PIN_BUZZER 5
#include <LiquidCrystal.h>

LiquidCrystal lcd(2,4,8,9,10,11);

long readTravelTime(int trig, int echo){
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trig, LOW);
  
  pinMode(echo, INPUT);
  return pulseIn(echo, HIGH);
}

void Play_buzzer(int distance){
  //물체까지의 거리가 100cm보다 작거나 같을 때
  if(distance <= 100){
    //부저 울림
    tone(PIN_BUZZER, 400);
  }
  //물체까지의 거리가 100cm보다 클 때
  else{   
    //물체까지의 거리(101~300)에 따라서 주기 0.1초~ 1초로 지정
    int period = map(distance,101,300,100,1000);
    tone(PIN_BUZZER,400,period/2);
    delay(period);
  }
}

void setup()
{
  pinMode(PIN_BUZZER,OUTPUT);
  lcd.begin(16,2); //16 col 2 line 생성
  lcd.print("Object Detector");
}

void loop()
{
  // 물체까지의 거리 계산
  int cm = 0.01723 * readTravelTime(12,12);
  
  String print_str;
  
  if(cm > 300){
    print_str = "No Object";
    noTone(PIN_BUZZER); //100cm 이내 물체가 사라질 시 부저음 제거
  }
  else{
    print_str = String(cm) + " cm      ";
    Play_buzzer(cm);
  }
  
  //2번째 line 1번째 col 로 커서 변경
  lcd.setCursor(0,1);
  lcd.print(print_str);

  delay(100);
}