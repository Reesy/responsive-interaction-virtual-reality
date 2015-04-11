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
      delay(1000);  
      digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);              // wait for a second
    }else{
        
    }
}
