void setup(void){
  Serial.begin(9600);
}

void loop(void){
  if(Serial.available()){
    byte temp = Serial.read();
    Serial.println(temp);
    Serial.write(temp);
    Serial.println();
  }
}