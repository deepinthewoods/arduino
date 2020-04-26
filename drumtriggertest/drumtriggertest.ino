#define LED_BUILTIN 2

#define SCALAR 1
#define DIVISOR 2


int T[] = {12, 27, 14, 33, 32, 13, 9, 10};//12
#define TRIGGERS_TOTAL 8
int touch[TRIGGERS_TOTAL];
int touchedLength[TRIGGERS_TOTAL];
unsigned long  touchTime[TRIGGERS_TOTAL];
boolean isTouched[TRIGGERS_TOTAL];
int triggerQ[1000], triggerCount;
#define OUTPUT_PIN 25
bool tt;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
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
    touch[i] = digitalRead(T[i]);//touch: range 13-76 ish

    //Serial.println(touch[i]);
    //if (touch[i] == 0) Serial.println(touch[i]);
    
    if (touch[i]  == 0){
      touchedLength[i]++;
    } else{
      touchedLength[i] = 0;
    }

    
    
    if (touchedLength[i] > 3
    ){
      if (!isTouched[i]){
        triggerQ[triggerCount++] = i;
        //Serial.print(touchedLength[i]);
        //Serial.println("firsttouch");
      }
      isTouched[i] = true;
      touchTime[i] = millis();
      
    } else {
      if (mill - touchTime[i] > 100){
        isTouched[i] = false;
      }
    }
  }
  
  for (int i = 0; i < triggerCount; i++){
    //Serial.println("write sig");
    Serial.print(triggerQ[i]);
    Serial.println("triggered");
    dacWrite(OUTPUT_PIN, triggerQ[i]*4-1);
    delayMicroseconds(3000);
  }
  dacWrite(OUTPUT_PIN, 0);
  triggerCount = 0;
  //Serial.println(touch[0]);
  
}


