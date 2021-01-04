//14051012 도진우
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

void setup()
{
  //16 col 2 line 생성
  lcd.begin(16,2);
  lcd.print("Distance : ");
}

int cm = 0;

void loop()
{
  cm = 0.01723 * readTravelTime(12,12);
  //2번째 line 1번째 col 로 커서 변경
  lcd.setCursor(0,1);
  lcd.print(cm);
  lcd.print("   ");
  //cm이 한자리수면 pos = 2 두자리수면 3 세자리수이상이면 4
  int pos = (cm < 10) ? 2 : (cm<100) ? 3 : 4;
  lcd.setCursor(pos,1);
  lcd.print("cm    ");
  delay(100);
}