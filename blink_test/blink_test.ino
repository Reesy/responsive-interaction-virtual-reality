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
int ledPin2 = 6;
int ledPin3 = 3;
int val = 0;   
int pins[3] = {9, 6, 3};
int vals[3];


int current = 0;
void setup() {
    
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    
   
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
        if(val == ','){
            current++;
            if(current == 3){
              current = 0;
            }
        }
        if(currentlyReading == true){
           
            if(val == '1'){
                vals[current] = map(1, 1, 9, 1, 255);
            }
            if(val == '2'){
                vals[current] = map(2, 1, 9, 1, 255);
            }
            if(val == '3'){
                vals[current] = map(3, 1, 9, 1, 255);
            }
            if(val == '4'){
                vals[current] = map(4, 1, 9, 1, 255);
            }
            if(val == '5'){
                vals[current] = map(5, 1, 9, 1, 255);
            }
            if(val == '6'){
                vals[current] = map(6, 1, 9, 1, 255);
            }
            if(val == '7'){
                vals[current] = map(7, 1, 9, 1, 255);
            }
            if(val == '8'){
                vals[current] = map(8, 1, 9, 1, 255);
            }
            if(val == '9'){
                vals[current] = map(9, 1, 9, 1, 255);
            }
            
             analogWrite(ledPin, vals[0]);  
             analogWrite(ledPin2, vals[1]);  
             analogWrite(ledPin3, vals[2]);  
      }
      
    }
}

