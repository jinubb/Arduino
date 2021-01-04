//14051012도진우 HW3 Jukebox
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define T 200
#define PIN_PIEZO 10

void setup()
{
  pinMode(PIN_PIEZO, OUTPUT); //피에조 부저 핀 10번
  pinMode(4, INPUT_PULLUP); //스위치
  pinMode(12, OUTPUT); //LED
  Serial.begin(9600);
  Serial.println("Welcome to jinubb's Jukebox"); 
}

int Notes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};

String NoteStr[] = { "Do", "Re", "Mi", "Fa", "Sol", "La", "Si", "Do5" };

//음계 함수 (n은 1부터 도레미.. d는 1일경우 0.2초
void Play_note(int n, int d){
  //시리얼프린트에 음계 출력
  Serial.print(NoteStr[n] + " ");
  int duration = T * d;
  tone(PIN_PIEZO, Notes[n], duration);
  delay(duration*1.1);
}  

int song_note[] = {1,3,5,1,3,5,6,6,6,5,4,4,4,3,3,3,2,2,2,1};
int song_duration[] = {2,2,2,2,2,2,2,2,2,4,2,2,3,2,2,3,2,2,2,4};
int song_N = 20;

void Play_song(){
  for(int i=0;i<song_N;i++){
    Play_note(song_note[i]-1, song_duration[i]);
  }
}

void Play_blink(){
  Serial.print("1 ");
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);
  delay(500);
  
  Serial.print("2 ");
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);
  delay(500);
  
  Serial.print("3");
  digitalWrite(12, HIGH);
  delay(500);
  digitalWrite(12, LOW);
  delay(500);
}
int state = 1;
void loop()
{
  switch(state){
    case 1:{
      Serial.println("Press Button to Play music!");
	  while (digitalRead(4) != LOW); //버튼이 눌렸다면
      state++;
      break;
    }
    case 2:{
      Serial.println("Play music!!!");
      Play_song();
      state++;
      Serial.println();
      break;
    }
    case 3:{
      Serial.println("Press Button to Blink the LED!");
      while (digitalRead(4) != LOW); //버튼이 눌렸다면
      state++;
      break;
    }
    case 4:{
      Serial.print("Blinking LED ... ");
      Play_blink();
      state = 1;
      Serial.println();
      break;
    }
  }
}