//14051012 도진우
#include <Servo.h>
#define PIN_SERVO 11
#define PIN_PHOTO A0

Servo servo;
int day = 0; //낮밤 플래그변수 1:낮 2:밤

void setup()
{
  Serial.begin(2400);
  servo.attach(PIN_SERVO);
  pinMode(PIN_PHOTO,INPUT);
}

void loop()
{
  int photo_val = analogRead(PIN_PHOTO);
  if(photo_val > 500){ // 낮
    if(day != 1){
      Serial.println("It's new day.");
      Serial.println("Door open");
      day = 1;
    }
    servo.write(0); // 차단봉 열림    
    Serial.read(); // 낮에 들어온 메시지 삭제
  }
  else{ //밤
    if(day != 2){
      Serial.println("It's dark");
      Serial.println("Door close");
      day = 2;
    }
    servo.write(90); //차단봉 닫힘
    
    //메시지가 들어올 경우
    if(Serial.available()){
      char temp = Serial.read();
      Serial.println(String("Arrived new message : ")+temp);
      if(temp == 'o'){
        servo.write(0); // 차단봉 열림
        Serial.println("Door open");
        long p_time = millis();
        while(photo_val <= 500){ // 낮이되면 탈출
          
          //문이 열려있는 중에 메시지가 들어올 경우
          if(Serial.available()){
            char temp2 = Serial.read();
            Serial.println(String("Arrived new message : ")+temp2);
            if(temp2 == 'o'){
              Serial.println("Door open");
              p_time = millis(); // 3초 카운트 초기화
            }
            else if(temp2 == 'c'){ 
              Serial.println("Door close");
              break; 
            }
            else{ 
              Serial.println("Error : Wrong message");
            }
          }
          
          //3초 카운트
          long c_time = millis();
          if(c_time - p_time >= 3000){
            break;
          }
          
          //낮 감지
          photo_val = analogRead(PIN_PHOTO);
        }
      }
      else if(temp == 'c'){
        Serial.println("Error : Already closed");
      }
      else{
        Serial.println("Error : Wrong message");
      }
    }
  }
}