//14051012 도진우
#include <LiquidCrystal.h>

LiquidCrystal lcd(2,4,8,9,10,11);

void setup()
{
  //16 col 2 line 생성
  lcd.begin(16,2);
  lcd.print("Hello jinubb!");
}

void loop()
{
  //2번째 line 1번째 col 로 커서 변경
  lcd.setCursor(0,1);
  
  lcd.print("Lap time : ");
  lcd.print(millis()/1000);
  delay(100);
}