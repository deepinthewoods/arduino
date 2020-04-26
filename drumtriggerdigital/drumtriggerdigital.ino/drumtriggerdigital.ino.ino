#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino


#define LED_BUILTIN 2

#define SCALAR 1
#define DIVISOR 2


int T[] = { 12, 13, 14, 27, 33, 32}//{20, 18, 17, 16, 12, 13}
, Sw[] = {34, 35};//, 21};//
#define TRIGGERS_TOTAL 6
int touch[TRIGGERS_TOTAL];
int touchedLength[TRIGGERS_TOTAL];
unsigned long  touchTime[TRIGGERS_TOTAL];
boolean isTouched[TRIGGERS_TOTAL];
byte triggerQ[1000], triggerCount;
#define OUTPUT_PIN 23
#define INPUT_PIN 22
bool tt;
int prevSwitchState;
long switchStateTime, flashTime;
bool hasSentSwitchState;
HardwareSerial Serial1(0);
#define DRUM_COMMAND 0
BluetoothSerial ESP_BT;

#define INPUT_SIZE 600
byte serialinput[INPUT_SIZE + 1];
byte serialsize;

byte serialinputB[INPUT_SIZE + 1];
byte serialsizeB;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
 // pinMode(OUTPUT_PIN, OUTPUT);
 // pinMode(T3, INPUT_PULLUP);
 for (int i = 0; i < TRIGGERS_TOTAL; i++){
  pinMode(T[i], INPUT_PULLUP);
 }
 for (int i = 0; i < sizeof(Sw); i++)
    pinMode(Sw[i], INPUT_PULLUP);
  //Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, INPUT_PIN, OUTPUT_PIN);
  ESP_BT.begin("trekninja control"); 
  triggerQ[triggerCount++] = 4 + 9;//comes out as +10
  triggerQ[triggerCount++] = 6 + 9;//comes out as +10
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
      if (mill - touchTime[i] > 5){
        isTouched[i] = false;
      }
    }
  }

  int switchState = 0;
  for (int i = 0; i < sizeof(Sw); i++){
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
    Serial1.print("msg");
    Serial1.write(2);
    Serial1.write(DRUM_COMMAND);
    
    Serial1.write(triggerQ[i] );
    
    
    //Serial1.print(String(triggerQ[i]));
    
    digitalWrite(LED_BUILTIN, HIGH);
    flashTime = millis();
    
  }
  //Serial1.flush();
  //if (Serial1.available()){
   // Serial.println("seial " + Serial1.read());
  //}
  
  //dacWrite(OUTPUT_PIN, 0);
  triggerCount = 0;
  //Serial.println(touch[0]);
  if (mill - flashTime > 200)
    digitalWrite(LED_BUILTIN, LOW);
  //Serial.println(switchState);

  


  if (ESP_BT.available()){
    byte incomingChar = ESP_BT.read();
    if (incomingChar != '\n'){
      serialinput[serialsize++] = incomingChar;
    }
    else{
     
     serialinput[serialsize] = 0;//endline
     
     Serial1.write(serialinput, serialsize);
     serialsize = 0;
    }
  }
  if (Serial1.available()){
    byte incomingChar = Serial1.read();
    if (incomingChar != '\n'){
      serialinputB[serialsizeB++] = incomingChar;
    }
    else{
     
     serialinputB[serialsizeB] = 0;//endline
     
     ESP_BT.write(serialinput, serialsizeB);
     serialsizeB = 0;
    }
  }
  
}
