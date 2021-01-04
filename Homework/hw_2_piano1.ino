//14051012 도진우 피아노 과제

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

int array[8] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};

void setup()
{
  pinMode(10, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino Piano by 14051012Dojinwoo");  
}

void loop()
{
  if(Serial.available()>0){
    byte data = Serial.read();
    data-=49; // 아스키코드를 배열값에 맞게 변환
    tone(10,array[data],900);
    delay(1000);
    noTone(10);
  }
}