#define LED_BUILTIN 2

#define SCALAR 1
#define DIVISOR 2


int T[] = {12, 27};//, 16, 17, 18, 20, 21, 22, 23};//12
#define TRIGGERS_TOTAL 1
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
    touch[i] = analogRead(T[i]);//touch: range 13-76 ish

    //Serial.println(touch[i]);
    //if (touch[i] == 0) Serial.println(touch[i]);
    
    if (touch[i]  < 3){
      //Serial.print(touch[i]);
      //Serial.println("touch ");
      touchedLength[i]++;
    } else{
     // Serial.println("not touch ");
      //Serial.println(touchedLength[i]);
      touchedLength[i] = 0;
    }

    //
    
    if (touchedLength[i] > 30
    ){
      if (!isTouched[i]){
        //Serial.print('First Touch');
        //Serial.print((byte)i);
        //Serial.print(i);
        //Serial.print(mill);
        //Serial.println();
        triggerQ[triggerCount++] = i;
        Serial.print(touchedLength[i]);
        Serial.println("firsttouch");
      }
      isTouched[i] = true;
      touchTime[i] = millis();
      //Serial.println(touchedLength[i]);
      
    } else {
      if (mill - touchTime[i] > 100){
        isTouched[i] = false;
        
      }
    }
    
  //Serial.print(touch[i]);
  //Serial.print(" ");
   // Serial.print(touch[i]);
  //Serial.print(" ");
    
  }
  
  
  for (int i = 0; i < triggerCount; i++){
    //Serial.println("write sig");
    Serial.print(mill);
    Serial.print("triggers: ");
    Serial.println(triggerCount);
    dacWrite(OUTPUT_PIN, 100);
    delay(1000);
    
  }
  dacWrite(OUTPUT_PIN, 0);
  triggerCount = 0;
  Serial.println(touch[0]);
  //tt = (touchedLength[0] > 40);
  //S//erial.println(" ");

  /*for (;;){
    dacWrite(OUTPUT_PIN, 255);
    delay(100);
    dacWrite(OUTPUT_PIN, 0);
    delay(4000);
  }*/
  //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
 
  
}


