//14051012도진우
#define PIN_LED 12
#define PIN_EN 11
#define PIN_IN1 10
#define PIN_IN2 9
#define PIN_SONIC 2
#define PIN_TMP A0

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_TMP, INPUT);
  digitalWrite(PIN_EN, HIGH); //enable
  digitalWrite(PIN_IN2, LOW); //정방향 회전
}

//온도센서 함수
double sensor_tmp(int pin){
  //센서값 아날로그 입력(0~1023)
  double sensor_val = double(analogRead(pin));
  
  //0~1023 값을 전압(0~5)으로 변경
  double voltage_val = sensor_val * 5.0 / 1023.0;
  
  //전압(0~5)을 온도로 변경(-50~450)
  double tmp_val = voltage_val * 100.0 - 50.0;
  
  //온도 반환
  return tmp_val;
}

//초음파센서 함수
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

void play_blink(int period){
  digitalWrite(PIN_LED, HIGH);
  delay(period/2);
  digitalWrite(PIN_LED, LOW);
  delay(period/2);
}

void loop()
{
  // 물체까지의 거리 계산
  int cm = 0.01723 * readTravelTime(PIN_SONIC,PIN_SONIC);
  // 50센치 이내로 접근했을경우
  if(cm <= 50){
    analogWrite(PIN_IN1, 0);
    play_blink(1000);
  }
  else{
    double c_tmp = sensor_tmp(PIN_TMP);
  	if(c_tmp < 20){
      analogWrite(PIN_IN1, 0);
  	}
  	else if(c_tmp < 30){
      int pwm_val = map(c_tmp,20,29,0,255);
      analogWrite(PIN_IN1, pwm_val);
  	}
  	else{
      analogWrite(PIN_IN1, 255);
  	}
  }
}