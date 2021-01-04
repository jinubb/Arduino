//14051012 도진우
#include <LiquidCrystal.h>

LiquidCrystal lcd(2,4,8,9,10,11);

void setup()
{
  //16 col 2 line 생성
  lcd.begin(16,2);
  lcd.print("Current Temp :");
  pinMode(A0,INPUT);
}

void loop()
{
  //2번째 line 1번째 col 로 커서 변경
  lcd.setCursor(0,1);
  
  //센서값 아날로그 입력(0~1023)
  int sensor_val = analogRead(A0);
  
  //0~1023 값을 전압(0~5)으로 변경
  double voltage_val = double(sensor_val) * 5.0 / 1023.0;
  
  //전압(0~5)을 온도로 변경(-50~450)
  double tmp_val = voltage_val * 100.0 - 50.0;
  
  lcd.print(tmp_val);
  lcd.setCursor(5,1);
  lcd.print(" C");
  delay(100);
}