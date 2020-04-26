#define LED_BUILTIN 2

#define T0 12
#define T1 14
#define T2 33
#define T3 32
#define SCALAR 1
#define DIVISOR 2
#define TRIGGERS_TOTAL 4

int[] T = {12, 14, 33, 32};
int[TRIGGERS_TOTAL] touch, avTouch;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
 // pinMode(T3, INPUT_PULLUP);
  Serial.begin(38400);
}

// the loop function runs over and over again forever
void loop() {

  for (int i = 0; i < TRIGGERS_TOTAL; i++){
    touch[i] = touchRead(T[i]);
    avTouch[i] =  (avTouch[i] * SCALAR + touch[i]) / (DIVISOR);
  Serial.print(touch[i]);
  Serial.print(" ");
    Serial.print(avTouch[i]);
  Serial.print(" ");

  }
  Serial.println();

/*  touch0 =  touchRead(T0);
  touch1 =  touchRead(T1);
  touch2 =   touchRead(T2);
  touch3 =  touchRead(T3);*/

  /*avTouch0 =  (avTouch0 * SCALAR + touch0) / (DIVISOR);
  avTouch1 =  (avTouch1 * SCALAR + touch1) / (DIVISOR);
  avTouch2 =  (avTouch2 * SCALAR + touch2) / (DIVISOR);
  avTouch3 =  (avTouch3 * SCALAR + touch3) / (DIVISOR);*/
  
  /*Serial.print(touch0);
  Serial.print(" ");
  Serial.print(touch1);
  Serial.print(" ");  
  Serial.print(touch2);
  Serial.print(" ");
  Serial.println(touch3);*/

  /*Serial.print(avTouch0);
  Serial.print(" ");
  Serial.print(avTouch1);
  Serial.print(" ");  
  Serial.print(avTouch2);
  Serial.print(" ");
  Serial.println(avTouch3);*/
 
 // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
 
  
}
