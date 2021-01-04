const int pin_ssd[] = {12, 11, 10, 9, 8, 7, 6, 5};

void setup()
{
  Serial.begin(2400);
  for(int i=0;i<8;i++){
    pinMode(pin_ssd[i],OUTPUT);
  }
}

const byte ssd[] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe4, 0xfe, 0xe6};
void show_digit(int n){
  int ssd_signal = ssd[n];
  for(int i=7;i>=0;i--){
    digitalWrite(pin_ssd[i], ssd_signal&0x01);
    ssd_signal = ssd_signal >> 1;
  }
}

void loop()
{
  if(Serial.available()){
    char temp = Serial.read();
    Serial.println(String("RX : ")+temp);
    if(temp >= '0' && temp <= '9'){
      show_digit(temp-48);
    }
    else{
      Serial.println("Not digit");
    }
  }
}