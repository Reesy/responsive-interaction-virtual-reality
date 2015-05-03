bool currentlyReading = false;

int result = 0;
int result2 = 0;

int ledPin1 = 11; //sets pin for vibration motor, used for thumb
int ledPin2 = 10; //sets pin for vibration motor, used for finger1
int ledPin3 = 9;  //sets pin for vibration motor, used for finger2
int ledPin4 = 6;  //sets pin for vibration motor, used for finger3
int ledPin5 = 3;  //sets pin for vibration motor, used for finger4
int val = 0;   
int vals[5];


int current = 0;
void setup() {
    
    Serial.begin(9600);
    pinMode(ledPin1, OUTPUT);
    
   
}

void loop () {
    
    if(Serial.available() > 0){
        val = Serial.read();
        if(val == '>'){
            currentlyReading = true;
        }
        if(val == ','){
             current++;
        }
        if(val == ':'){
             current = 0;
        }
        if(currentlyReading == true){
            if(val == '0'){
                vals[current] = 0;
            }
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
            
             analogWrite(ledPin1, vals[0]);  
             analogWrite(ledPin2, vals[1]);  
             analogWrite(ledPin3, vals[2]);  
             analogWrite(ledPin4, vals[3]);
             analogWrite(ledPin5, vals[4]);
      }
      
    }
}

