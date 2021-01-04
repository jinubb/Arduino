//14051012도진우 HW5 Jukebox V3 by TimerOne
#ifndef TIMERONE_h
#define TIMERONE_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION 65536    // Timer1 is 16 bit

class TimerOne
{
  public:
  
    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;
	char oldSREG;					// To hold Status Register while ints disabled

    // methods
    void initialize(long microseconds=1000000);
    void start();
    void stop();
    void restart();
	void resume();
	unsigned long read();
    void pwm(char pin, int duty, long microseconds=-1);
    void disablePwm(char pin);
    void attachInterrupt(void (*isr)(), long microseconds=-1);
    void detachInterrupt();
    void setPeriod(long microseconds);
    void setPwmDuty(char pin, int duty);
    void (*isrCallback)();
};

extern TimerOne Timer1;
#endif
#ifndef TIMERONE_cpp
#define TIMERONE_cpp


TimerOne Timer1;              // preinstatiate

ISR(TIMER1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Timer1.isrCallback();
}


void TimerOne::initialize(long microseconds)
{
  TCCR1A = 0;                 // clear control register A 
  TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}


void TimerOne::setPeriod(long microseconds)		// AR modified for atomic access
{
  
  long cycles = (F_CPU / 2000000) * microseconds;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
  
  oldSREG = SREG;				
  cli();							// Disable interrupts for 16 bit register access
  ICR1 = pwmPeriod = cycles;                                          // ICR1 is TOP in p & f correct pwm mode
  SREG = oldSREG;
  
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                          // reset clock select register, and starts the clock
}

void TimerOne::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  
  dutyCycle *= duty;
  dutyCycle >>= 10;
  
  oldSREG = SREG;
  cli();
  if(pin == 1 || pin == 9)       OCR1A = dutyCycle;
  else if(pin == 2 || pin == 10) OCR1B = dutyCycle;
  SREG = oldSREG;
}

void TimerOne::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  if(pin == 1 || pin == 9) {
    DDRB |= _BV(PORTB1);                                   // sets data direction register for pwm output pin
    TCCR1A |= _BV(COM1A1);                                 // activates the output pin
  }
  else if(pin == 2 || pin == 10) {
    DDRB |= _BV(PORTB2);
    TCCR1A |= _BV(COM1B1);
  }
  setPwmDuty(pin, duty);
  resume();			// Lex - make sure the clock is running.  We don't want to restart the count, in case we are starting the second WGM
					// and the first one is in the middle of a cycle
}

void TimerOne::disablePwm(char pin)
{
  if(pin == 1 || pin == 9)       TCCR1A &= ~_BV(COM1A1);   // clear the bit that enables pwm on PB1
  else if(pin == 2 || pin == 10) TCCR1A &= ~_BV(COM1B1);   // clear the bit that enables pwm on PB2
}

void TimerOne::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK1 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
	// might be running with interrupts disabled (eg inside an ISR), so don't touch the global state
//  sei();
  resume();												
}

void TimerOne::detachInterrupt()
{
  TIMSK1 &= ~_BV(TOIE1);                                   // clears the timer overflow interrupt enable bit 
															// timer continues to count without calling the isr
}

void TimerOne::resume()				// AR suggested
{ 
  TCCR1B |= clockSelectBits;
}

void TimerOne::restart()		// Depricated - Public interface to start at zero - Lex 10/9/2011
{
	start();				
}

void TimerOne::start()	// AR addition, renamed by Lex to reflect it's actual role
{
  unsigned int tcnt1;
  
  TIMSK1 &= ~_BV(TOIE1);        // AR added 
  GTCCR |= _BV(PSRSYNC);   		// AR added - reset prescaler (NB: shared with all 16 bit timers);

  oldSREG = SREG;				// AR - save status register
  cli();						// AR - Disable interrupts
  TCNT1 = 0;                	
  SREG = oldSREG;          		// AR - Restore status register
	resume();
  do {	// Nothing -- wait until timer moved on from zero - otherwise get a phantom interrupt
	oldSREG = SREG;
	cli();
	tcnt1 = TCNT1;
	SREG = oldSREG;
  } while (tcnt1==0); 
 
//  TIFR1 = 0xff;              		// AR - Clear interrupt flags
//  TIMSK1 = _BV(TOIE1);              // sets the timer overflow interrupt enable bit
}

void TimerOne::stop()
{
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));          // clears all clock selects bits
}

unsigned long TimerOne::read()		//returns the value of the timer in microseconds
{									//rember! phase and freq correct mode counts up to then down again
  	unsigned long tmp;				// AR amended to hold more than 65536 (could be nearly double this)
  	unsigned int tcnt1;				// AR added

	oldSREG= SREG;
  	cli();							
  	tmp=TCNT1;    					
	SREG = oldSREG;

	char scale=0;
	switch (clockSelectBits)
	{
	case 1:// no prescalse
		scale=0;
		break;
	case 2:// x8 prescale
		scale=3;
		break;
	case 3:// x64
		scale=6;
		break;
	case 4:// x256
		scale=8;
		break;
	case 5:// x1024
		scale=10;
		break;
	}
	
	do {	// Nothing -- max delay here is ~1023 cycles.  AR modified
		oldSREG = SREG;
		cli();
		tcnt1 = TCNT1;
		SREG = oldSREG;
	} while (tcnt1==tmp); //if the timer has not ticked yet

	//if we are counting down add the top value to how far we have counted down
	tmp = (  (tcnt1>tmp) ? (tmp) : (long)(ICR1-tcnt1)+(long)ICR1  );		// AR amended to add casts and reuse previous TCNT1
	return ((tmp*1000L)/(F_CPU /1000L))<<scale;
}

#endif
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

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_BUTTON, INPUT_PULLUP); //버튼 핀 2번
  pinMode(PIN_PIEZO, OUTPUT); //피에조 부저 핀 10번
  pinMode(PIN_LED, OUTPUT); //LED 핀 12번
  attachInterrupt(0,sw_isr,FALLING); // 인터럽트 추가
  Timer1.initialize(500000); // timer 주기 0.5초로 설정
  Serial.println("Welcome to jinwoo's Jukebox Version_3 by TimerOne"); 
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
      Timer1.attachInterrupt(Play_blink); //타이머 추가
      while (interrupt_val != 1 && num < 11); // 버튼이 눌리거나 10초 blink한 경우
      digitalWrite(PIN_LED, LOW); // 인터럽트로 강제종료시 LED 점등
      Timer1.detachInterrupt(); // 타이머 삭제
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
