int input;

void setup()
{
    Serial.begin(9600);
    pinMode(9, OUTPUT);
}
void loop()
{
    if(Serial.available()>0)
    {
      digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
        
    }else{
      digitalWrite(9, LOW);
    }
}

void serialEvent(){
  
  
  
}
