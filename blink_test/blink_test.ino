bool currentlyReading = false;
bool finishedReading = false;

//values 1-9 for sensitivity of each pin. PWM
int inVal1 = 0; // pin 9
int inVal2 = 0; // pin 10
int inVal3 = 0; // pin 11
int inVal4 = 0; // pin 3
int inVal5 = 0; // pin 5

int result = 0;
int result2 = 0;

int ledPin = 9;
int val = 0;   

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    result = map(4, 1, 9, 1, 255);
    result2 = map(9, 1, 9, 1, 255);
}

void loop () {
    
    if(Serial.available() > 0){
        val = Serial.read();
        if(val == '>'){
            currentlyReading = true;
            Serial.println("Reading engaged");
        }
        if(val == ':'){
            currentlyReading = false;
            Serial.println("Reading disengaged");
        }
        if(currentlyReading == true){
           
            if(val == '1'){
                //digitalWrite(ledPin, HIGH);
                
                 analogWrite(ledPin, result);
            }
            if(val == '2'){
               // digitalWrite(ledPin, LOW);
                
                 analogWrite(ledPin, result2);
             }
        }
      
    }
}

