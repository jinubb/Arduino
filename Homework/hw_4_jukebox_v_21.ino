//14051012도진우 HW4 Jukebox v2
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 554
#define NOTE_E5 587
#define NOTE_F5 622
#define T 200
#define PIN_BUTTON 2
#define PIN_PIEZO 10
#define PIN_LED 12

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_BUTTON, INPUT_PULLUP); //버튼 핀 2번
  pinMode(PIN_PIEZO, OUTPUT); //피에조 부저 핀 10번
  pinMode(PIN_LED, OUTPUT); //LED 핀 12번
  attachInterrupt(0,sw_isr,FALLING); // 인터럽트 추가
  Serial.println("Welcome to jinwoo's Jukebox Version_2"); 
}

int Notes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5};
String NoteStr[] = { "Do", "Re", "Mi", "Fa", "Sol", "La", "Si", "Do5", "Re5", "Mi5", "Fa5" };

//음계 재생 함수 (n은 1부터 도레미.. d는 1일경우 0.2초)
void Play_note(int n, int d){
  //시리얼프린트에 음계 출력
  Serial.print(NoteStr[n] + " ");
  int duration = T * d;
  tone(PIN_PIEZO, Notes[n], duration);
  jinu_delay(duration*1.1,millis());
}  

int song_note[] = {3, 8, 3, 7, 3, 6, 5, 4, 5, 1, 2, 4, 6, 8, 7, 5, 3, 2, 3, 3, 8, 3, 7, 3, 6, 5, 4, 5, 3, 2, 4, 6, 8, 9, 10, 7, 5, 6};
int song_duration[] = {2,4,2,4,2,4,2,2,5,2,2,2,2,2,2,2,2,2,7,2,4,2,4,2,4,2,2,5,2,2,2,2,2,2,2,2,2,7};
int song_N = 38;

volatile int interrupt_val = 0; // 인터럽트 변수
volatile int state = 1;

//음악 재생 함수
void Play_song(){
  for(int i=0;i<song_N;i++){
    //인터럽트 변수가 SET될 경우 종료
    if(interrupt_val == 1){
      break;
    }
    else{
      //음계 재생 함수
      Play_note(song_note[i]-1, song_duration[i]);
    }
  }
  //인터럽트가 들어오지 않을 경우 Auto Increase
  if(interrupt_val ==0){
  	state++;
  }
}

//LED Blink 함수(매개변수 : Blink 횟수, Blink 주기)
void Play_blink(int times,int cycle){
  for(int i=1;i<=times;i++){
    int previous_time = millis();
    //인터럽트 변수가 SET될 경우 종료
    if(interrupt_val == 1){
      break;
    }
    //LED Blink
    else{
      Serial.print(i);
      Serial.print("  ");
      digitalWrite(PIN_LED, HIGH);
      jinu_delay(cycle/2,millis());
      digitalWrite(PIN_LED, LOW);
      jinu_delay(cycle/2,millis());
    }
  }
}

void loop()
{
  switch(state){
    case 1:{
      Serial.println("Press Button to Play music!");
	  while (digitalRead(PIN_BUTTON) != LOW); //버튼이 눌렸다면
      break;
    }
    case 2:{
      Serial.println("Play music!!!");
      Play_song(); //음악 재생 함수
      Serial.println();
      break;
    }
    case 3:{
      Serial.println("Press Button to Blink the LED!");
      while (digitalRead(PIN_BUTTON) != LOW); //버튼이 눌렸다면
      break;
    }
    case 4:{
      Serial.print("Blinking LED ... ");
      Play_blink(10,1000); //10회 blink , 1초 주기
      state = 1;
      Serial.println();
      break;
    }
  }
  interrupt_val = 0;
}

void sw_isr(){
  state++;
  interrupt_val = 1; // 인터럽트 변수 SET
}

//millis()를 이용한 사용사정의 delay함수
void jinu_delay(int time,unsigned long current_time){
  while (true){
    unsigned long temp = millis();
    if( temp > current_time + time )
      break;
  }
}
