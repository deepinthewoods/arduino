#define LED_BUILTIN 2

#define SCALAR 1
#define DIVISOR 2


int T[] = {12, 13};//, 16, 17, 18, 20, 21, 22, 23};
#define TRIGGERS_TOTAL 2
int touch[TRIGGERS_TOTAL], avTouch[TRIGGERS_TOTAL];
unsigned long  touchTime[TRIGGERS_TOTAL];
boolean isTouched[TRIGGERS_TOTAL];
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
 // pinMode(T3, INPUT_PULLUP);
 for (int i = 0; i < TRIGGERS_TOTAL; i++){
  pinMode(T[i], INPUT_PULLUP);
 }
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  unsigned long mill = millis();
  for (int i = 0; i < TRIGGERS_TOTAL; i++){
    touch[i] = touchRead(T[i]);//range 13-76 ish
    avTouch[i] =  (avTouch[i] * SCALAR + touch[i]) / (DIVISOR);
    if (avTouch[i] < 20
    ){
      if (!isTouched[i]){
        Serial.print('d');
        Serial.print((byte)i);
        //Serial.print(i);
        //Serial.print(mill);
        Serial.println();
      }
      isTouched[i] = true;
      touchTime[i] = millis();
      
      
    } else {
      if (mill - touchTime[i] > 100)
        isTouched[i] = false;
    }
  //Serial.print(touch[i]);
  //Serial.print(" ");
   // Serial.print(touch[i]);
  //Serial.print(" ");
    
  }
  //Serial.println();


 
 // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
 
  
}


