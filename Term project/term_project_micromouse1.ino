//14051012 도진우 Micromouse for Arduino
#include <LiquidCrystal.h>
//스위치
#define PIN_SWITCH 2
//초음파센서
#define PIN_SENSOR_LEFT 3
#define PIN_SENSOR_RIGHT 4
#define PIN_SENSOR_FRONT 7
//피에조
#define PIN_PIEZO 6
//모터드라이버
#define PIN_IN1 5
#define PIN_IN2 9
#define PIN_EN_LEFT 8
#define PIN_IN3 10
#define PIN_IN4 11
#define PIN_EN_RIGHT 12

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5); //lcd 매핑
volatile bool state = false; //동작상태 변수, false:정지 true:직진
String str_lcd = "Hello jinubb!"; //lcd 메시지 변수
unsigned long c_time; //현재시간 저장
int start_flag = 0; //lcd 시작메시지 출력

void isr(){
  state = !state; //상태 변경
}

//초음파센서 거리계산 함수
long readTravelTime(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  
  long distance = 0.01723 * pulseIn(pin, HIGH);
  return distance;
}

//정지
void stop(){
  digitalWrite(PIN_EN_LEFT, LOW);
  digitalWrite(PIN_EN_RIGHT, LOW);
  analogWrite(PIN_IN1, 0);
  analogWrite(PIN_IN3, 0);  
}

//직진
void go(int left_speed, int right_speed){
  digitalWrite(PIN_EN_LEFT, HIGH);
  digitalWrite(PIN_EN_RIGHT, HIGH);
  analogWrite(PIN_IN1, left_speed);
  analogWrite(PIN_IN3, right_speed);
}

void setup()
{
  //Serial setup
  Serial.begin(9600);
  
  //Switch setup
  pinMode(PIN_SWITCH, INPUT_PULLUP);
  attachInterrupt(0,isr,FALLING);
  
  //Piezo setup
  pinMode(PIN_PIEZO, OUTPUT);
  
  //Left DC Motor setup
  pinMode(PIN_EN_LEFT, OUTPUT);
  digitalWrite(PIN_EN_LEFT, LOW);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  digitalWrite(PIN_IN2, LOW);
    
  //Right DC Motor setup
  pinMode(PIN_EN_RIGHT, OUTPUT);
  digitalWrite(PIN_EN_RIGHT, LOW);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  digitalWrite(PIN_IN4, LOW);
  
  //LCD setup
  lcd.begin(16,2); //16 col 2 line 생성
  lcd.print("Micromouse");
}

void loop()
{
  if(Serial.available() == true){
    char temp = Serial.read();
    Serial.println(String("Arrived new message : ")+temp);
    if(temp == 'b'){
      state = true;
    }
    else if(temp == 'r'){
      state = false;
    }
    else{
      Serial.println("Error : Wrong message");
    }
  }
  switch(state){
    //정지
    case false:{
      if(start_flag == 1){
        str_lcd = "Stop!          ";
      }
      stop();
      noTone(PIN_PIEZO);
      break;
    }
    
    //직진
    case true:{
      start_flag = 1;
      c_time = millis();
      long distance_front = readTravelTime(PIN_SENSOR_FRONT);
      long distance_left = readTravelTime(PIN_SENSOR_LEFT);
      long distance_right = readTravelTime(PIN_SENSOR_RIGHT);
      //전방 벽과의 거리가 1m 이내일 경우
      if(distance_front <= 100){
        //우측 벽과의 거리가 1m 이상일 경우
        if(distance_right > 100){ 
          noTone(PIN_PIEZO);
          //우회전
          str_lcd = "Turn right     ";
          go(10,0);
          while(millis() - c_time < 1000 && state == true);
        }
        //좌측 벽과의 거리가 1m 이상일 경우
        else if(distance_left > 100){
          noTone(PIN_PIEZO);
          //좌회전
          str_lcd = "Turn left      ";
          go(0,10);
          while(millis() - c_time < 1000 && state == true);
        }
        else{
          //정지
          str_lcd = "Can't go       ";
          //부저음 출력
          tone(PIN_PIEZO, 500);
          go(0,0);
        }
                  
      }
      //좌측 벽과의 거리가 50cm 이내일 경우
      else if(distance_left <= 50){
        noTone(PIN_PIEZO);
        //1초동안 좌측 바퀴 속도 10%증가
        str_lcd = "Left speedup   ";
        go(11,10);
        while(c_time - millis() < 1000 && state == true);
      }
      //우측 벽과의 거리가 50cm 이내일 경우
      else if(distance_right <= 50){
        noTone(PIN_PIEZO);
        //1초동안 우측 바퀴 속도 10%증가
        str_lcd = "Right speedup  ";
        go(10,11);
        while(c_time - millis() < 1000 && state == true);
      }
      else{
        noTone(PIN_PIEZO);
        str_lcd = "Go straight!   ";
        go(10,10);
      }
      break;
    }
  }
  //LCD출력
  lcd.setCursor(0,1);
  lcd.print(str_lcd);
}