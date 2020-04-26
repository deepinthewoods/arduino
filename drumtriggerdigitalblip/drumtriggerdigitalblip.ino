#define LED_BUILTIN 2

#define SCALAR 1
#define DIVISOR 2


int T[] = { 12, 13, 14, 27, 33, 32}//{20, 18, 17, 16, 12, 13}
, Sw[] = {25, 26, 21};//
#define TRIGGERS_TOTAL 6
int touch[TRIGGERS_TOTAL];
int touchedLength[TRIGGERS_TOTAL];
unsigned long  touchTime[TRIGGERS_TOTAL];
boolean isTouched[TRIGGERS_TOTAL];
int triggerQ[1000], triggerCount;
#define OUTPUT_PIN 1
#define INPUT_PIN 3
bool tt;
int prevSwitchState;
long switchStateTime, flashTime;
bool hasSentSwitchState;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
 // pinMode(T3, INPUT_PULLUP);
 for (int i = 0; i < TRIGGERS_TOTAL; i++){
  pinMode(T[i], INPUT_PULLUP);
 }
 for (int i = 0; i < 4; i++)
    pinMode(Sw[i], INPUT_PULLUP);
  //Serial.begin(115200);
  
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

    
    
    if (touchedLength[i] > 1
    ){
      if (!isTouched[i]){
        triggerQ[triggerCount++] = i;
        //Serial.print(touchedLength[i]);
        //Serial.println("firsttouch");
      }
      isTouched[i] = true;
      touchTime[i] = mill;
      
    } else {
      if (mill - touchTime[i] > 50){
        isTouched[i] = false;
      }
    }
  }

  int switchState = 0;
  for (int i = 0; i < 4; i++){
    int pin = digitalRead(Sw[i]);
    switchState = switchState << 1;
    switchState += pin;
  }
  if (switchState != prevSwitchState){
    prevSwitchState = switchState;
    switchStateTime = mill;
    hasSentSwitchState = false;
    //Serial.println(switchState);
  } else {
    if (!hasSentSwitchState && mill - switchStateTime > 50){
      hasSentSwitchState = true;
      triggerQ[triggerCount++] = switchState + 9;//comes out as +10
      //Serial.println(switchState);
    }
  }
  
  
  for (int i = 0; i < triggerCount; i++){
    //Serial.println("write sig");
    
    //Serial.print(triggerQ[i]);
    //Serial.println(" triggered");
    //dacWrite(OUTPUT_PIN, triggerQ[i]*4-1);
    digitalWrite(OUTPUT_PIN, HIGH);
    delayMicroseconds(45 * (triggerQ[i] + 1));
    //delay((i+1));
    digitalWrite(OUTPUT_PIN, LOW);
    delayMicroseconds(100);
    
    digitalWrite(LED_BUILTIN, HIGH);
    flashTime = mill;
    
  }
  //dacWrite(OUTPUT_PIN, 0);
  triggerCount = 0;
  //Serial.println(touch[0]);
  if (mill - flashTime > 200)
    digitalWrite(LED_BUILTIN, LOW);
  //Serial.println(switchState);
}


