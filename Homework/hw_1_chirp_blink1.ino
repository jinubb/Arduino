//14051012 도진우 Chirp Blink 과제

int time = 2000; //Blink 초기 주기 2초

void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
  //Blink 주기가 20ms 보다 클 경우
  if(time > 20){
    digitalWrite(13, HIGH);
  	delay(time/2);
  	digitalWrite(13, LOW);
  	delay(time/2);
    
    time = (time*9)/10; // blink 주기 10% 감소
  }
  //Blink 주기가 20ms보다 작거나 같을 경우
  else{
  	time = 2000; // Blink 주기 2초로 초기화
  }
}