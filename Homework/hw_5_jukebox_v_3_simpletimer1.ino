//14051012도진우 HW5 Jukebox V3 by SimpleTimer
#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H
 
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
 
typedef void (*timer_callback)(void);
 
class SimpleTimer {
 
public:
    // maximum number of timers
    const static int MAX_TIMERS = 10;
 
    // setTimer() constants
    const static int RUN_FOREVER = 0;
    const static int RUN_ONCE = 1;
 
    // constructor
    SimpleTimer();
 
    // this function must be called inside loop()
    void run();
 
    // call function f every d milliseconds
    int setInterval(long d, timer_callback f);
 
    // call function f once after d milliseconds
    int setTimeout(long d, timer_callback f);
 
    // call function f every d milliseconds for n times
    int setTimer(long d, timer_callback f, int n);
 
    // destroy the specified timer
    void deleteTimer(int numTimer);
 
    // restart the specified timer
    void restartTimer(int numTimer);
 
    // returns true if the specified timer is enabled
    boolean isEnabled(int numTimer);
 
    // enables the specified timer
    void enable(int numTimer);
 
    // disables the specified timer
    void disable(int numTimer);
 
    // enables the specified timer if it's currently disabled,
    // and vice-versa
    void toggle(int numTimer);
 
    // returns the number of used timers
    int getNumTimers();
 
    // returns the number of available timers
    int getNumAvailableTimers() { return MAX_TIMERS - numTimers; };
 
private:
    // deferred call constants
    const static int DEFCALL_DONTRUN = 0;       // don't call the callback function
    const static int DEFCALL_RUNONLY = 1;       // call the callback function but don't delete the timer
    const static int DEFCALL_RUNANDDEL = 2;      // call the callback function and delete the timer
 
    // find the first available slot
    int findFirstFreeSlot();
 
    // value returned by the millis() function
    // in the previous run() call
    unsigned long prev_millis[MAX_TIMERS];
 
    // pointers to the callback functions
    timer_callback callbacks[MAX_TIMERS];
 
    // delay values
    long delays[MAX_TIMERS];
 
    // number of runs to be executed for each timer
    int maxNumRuns[MAX_TIMERS];
 
    // number of executed runs for each timer
    int numRuns[MAX_TIMERS];
 
    // which timers are enabled
    boolean enabled[MAX_TIMERS];
 
    // deferred function call (sort of) - N.B.: this array is only used in run()
    int toBeCalled[MAX_TIMERS];
 
    // actual number of timers in use
    int numTimers;
};
 
#endif
// Select time function:
//static inline unsigned long elapsed() { return micros(); }
static inline unsigned long elapsed() { return millis(); }
 
 
SimpleTimer::SimpleTimer() {
    unsigned long current_millis = elapsed();
 
    for (int i = 0; i < MAX_TIMERS; i++) {
        enabled[i] = false;
        callbacks[i] = 0;                   // if the callback pointer is zero, the slot is free, i.e. doesn't "contain" any timer
        prev_millis[i] = current_millis;
        numRuns[i] = 0;
    }
 
    numTimers = 0;
}
 
 
void SimpleTimer::run() {
    int i;
    unsigned long current_millis;
 
    // get current time
    current_millis = elapsed();
 
    for (i = 0; i < MAX_TIMERS; i++) {
 
        toBeCalled[i] = DEFCALL_DONTRUN;
 
        // no callback == no timer, i.e. jump over empty slots
        if (callbacks[i]) {
 
            // is it time to process this timer ?
            // see https://arduino.cc/forum/index.php/topic,124048.msg932592.html#msg932592
 
            if (current_millis - prev_millis[i] >= delays[i]) {
 
                // update time
                //prev_millis[i] = current_millis;
                prev_millis[i] += delays[i];
 
                // check if the timer callback has to be executed
                if (enabled[i]) {
 
                    // "run forever" timers must always be executed
                    if (maxNumRuns[i] == RUN_FOREVER) {
                        toBeCalled[i] = DEFCALL_RUNONLY;
                    }
                    // other timers get executed the specified number of times
                    else if (numRuns[i] < maxNumRuns[i]) {
                        toBeCalled[i] = DEFCALL_RUNONLY;
                        numRuns[i]++;
 
                        // after the last run, delete the timer
                        if (numRuns[i] >= maxNumRuns[i]) {
                            toBeCalled[i] = DEFCALL_RUNANDDEL;
                        }
                    }
                }
            }
        }
    }
 
    for (i = 0; i < MAX_TIMERS; i++) {
        switch(toBeCalled[i]) {
            case DEFCALL_DONTRUN:
                break;
 
            case DEFCALL_RUNONLY:
                (*callbacks[i])();
                break;
 
            case DEFCALL_RUNANDDEL:
                (*callbacks[i])();
                deleteTimer(i);
                break;
        }
    }
}
 
 
// find the first available slot
// return -1 if none found
int SimpleTimer::findFirstFreeSlot() {
    int i;
 
    // all slots are used
    if (numTimers >= MAX_TIMERS) {
        return -1;
    }
 
    // return the first slot with no callback (i.e. free)
    for (i = 0; i < MAX_TIMERS; i++) {
        if (callbacks[i] == 0) {
            return i;
        }
    }
 
    // no free slots found
    return -1;
}
 
 
int SimpleTimer::setTimer(long d, timer_callback f, int n) {
    int freeTimer;
 
    freeTimer = findFirstFreeSlot();
    if (freeTimer < 0) {
        return -1;
    }
 
    if (f == NULL) {
        return -1;
    }
 
    delays[freeTimer] = d;
    callbacks[freeTimer] = f;
    maxNumRuns[freeTimer] = n;
    enabled[freeTimer] = true;
    prev_millis[freeTimer] = elapsed();
 
    numTimers++;
 
    return freeTimer;
}
 
 
int SimpleTimer::setInterval(long d, timer_callback f) {
    return setTimer(d, f, RUN_FOREVER);
}
 
 
int SimpleTimer::setTimeout(long d, timer_callback f) {
    return setTimer(d, f, RUN_ONCE);
}
 
 
void SimpleTimer::deleteTimer(int timerId) {
    if (timerId >= MAX_TIMERS) {
        return;
    }
 
    // nothing to delete if no timers are in use
    if (numTimers == 0) {
        return;
    }
 
    // don't decrease the number of timers if the
    // specified slot is already empty
    if (callbacks[timerId] != NULL) {
        callbacks[timerId] = 0;
        enabled[timerId] = false;
        toBeCalled[timerId] = DEFCALL_DONTRUN;
        delays[timerId] = 0;
        numRuns[timerId] = 0;
 
        // update number of timers
        numTimers--;
    }
}
 
 
// function contributed by code@rowansimms.com
void SimpleTimer::restartTimer(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }
 
    prev_millis[numTimer] = elapsed();
}
 
 
boolean SimpleTimer::isEnabled(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }
 
    return enabled[numTimer];
}
 
 
void SimpleTimer::enable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }
 
    enabled[numTimer] = true;
}
 
 
void SimpleTimer::disable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }
 
    enabled[numTimer] = false;
}
 
 
void SimpleTimer::toggle(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }
 
    enabled[numTimer] = !enabled[numTimer];
}
 
 
int SimpleTimer::getNumTimers() {
    return numTimers;
}

//code start
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

SimpleTimer timer0;
int temp;

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_BUTTON, INPUT_PULLUP); //버튼 핀 2번
  pinMode(PIN_PIEZO, OUTPUT); //피에조 부저 핀 10번
  pinMode(PIN_LED, OUTPUT); //LED 핀 12번
  attachInterrupt(0,sw_isr,FALLING); // 인터럽트 추가
  Serial.println("Welcome to jinwoo's Jukebox Version_3 by SimpleTimer"); 
}

int Notes[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5};
String NoteStr[] = { "Do", "Re", "Mi", "Fa", "Sol", "La", "Si", "Do5", "Re5", "Mi5", "Fa5" };

//음계 재생 함수 (n은 1부터 도레미.. d는 1일경우 0.2초)
void Play_note(int n, int d){
  //시리얼프린트에 음계 출력
  Serial.print(NoteStr[n] + " ");
  int duration = T * d;
  tone(PIN_PIEZO, Notes[n], duration);
  delay(duration*1.1);
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

int num = 0; // blink 횟수 저장 변수
int blink_temp = 0; // LED 상태 저장 변수
//LED Blink 함수
void Play_blink(){
  blink_temp = digitalRead(PIN_LED);
  if(blink_temp == LOW){
    Serial.print(++num);
    Serial.print(" ");
  }
  else if(num == 10){ // 10번째 블링크 주기를 채우기 위해 설정
    num++;
  }
  digitalWrite(PIN_LED, !blink_temp);
}


void loop()
{
  switch(state){
    case 1:{
      Serial.println("Press Button to Play music!");
	  while (interrupt_val != 1); //버튼이 눌렸다면
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
      while (interrupt_val != 1); //버튼이 눌렸다면
      break;
    }
    case 4:{
      Serial.print("Blinking LED ... ");
      temp = timer0.setTimer(500,Play_blink,20); // 0.5초마다 Play_blink함수 실행
      while (interrupt_val != 1 && num < 11){ // 버튼이 눌리거나 10초 blink한 경우
        timer0.run();
      }
      digitalWrite(PIN_LED, LOW); // 인터럽트로 강제종료시 LED 점등
      timer0.deleteTimer(temp); // 타이머 삭제
      num = 0; // blink 횟수 저장 변수 초기화
      blink_temp = 0; // LED 상태 저장 변수 초기화
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
