
int ledPin = 9;
int val = 0;   
char response[20];
void setup() {
 Serial.begin(9600);
 pinMode(ledPin, OUTPUT);
}

void loop () {
    
    if(Serial.available() > 0){
        val = Serial.read();
        if(val == 'a'){
            digitalWrite(ledPin, HIGH);
          
         //  Serial.write(Serial.read());
        }
        if(val == 's'){
            digitalWrite(ledPin, LOW);
        }
    }
}

