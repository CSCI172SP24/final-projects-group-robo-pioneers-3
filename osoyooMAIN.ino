#include <IRremote.h>

// Pin Definitions
#define LEFT_SENSOR_PIN A4
#define RIGHT_SENSOR_PIN A0
#define speedPinR 9
#define speedPinL 6
#define RECV_PIN 10
#define ECHO_PIN 3
#define TRIG_PIN 2
#define RightDirectPin1  12
#define RightDirectPin2  11
#define LeftDirectPin1  7
#define LeftDirectPin2  8
#define BUZZ_PIN 13
#define RightObstacleSensor 5
#define LeftObstacleSensor 4

// IR Remote Button Codes
#define btn1 0xFFA25D
#define btn2 0xFF629D
#define btn3 0xFFE21D
#define btn4 0xFF22DD
#define btn5 0xFF02FD
#define btn6 0xFFC23D
#define btn7 0xFFE01F
#define btn8 0xFFA857
#define btn9 0xFF906F
#define btnASTERISCKS 0xFF6897
#define btn0 0xFF9867
#define btnHASHTAG 0xFFB04F
#define btnUP 0xFF18E7
#define btnLEFT 0xFF10EF
#define btnOK 0xFF38C7
#define btnRIGHT 0xFF5AA5
#define btnDOWN 0xFF4AB5

#define MODE_IR_DRIVE 0
#define MODE_OBJ_FOLLOW 1
#define MODE_OBJ_AVOID 2
#define MODE_LINE_FOLLOW 3
#define MODE_SPECIAL 4

#define SENSOR_THRESHOLD 500

int mode = MODE_IR_DRIVE;
int t = 250;
int motorSpeed = 150; // Motor speed

IRrecv irrecv(RECV_PIN);
decode_results results;

// Enum for driving states
enum DN { 
  GO_ADVANCE,
  GO_LEFT,
  GO_RIGHT,
  GO_BACK,
  STOP_STOP,
  OBJ_FOLLOW,
  LINE_FOLLOW,
  DEF
} Drive_Num = DEF;

bool stopFlag = true;
bool JogFlag = false;
uint16_t JogTimeCnt = 0;
uint32_t JogTime = 0;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

  pinMode(RightDirectPin1, OUTPUT);
  pinMode(RightDirectPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);
  pinMode(LeftDirectPin1, OUTPUT);
  pinMode(LeftDirectPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);
  stop_Stop();
  
  pinMode(RightObstacleSensor, INPUT);
  pinMode(LeftObstacleSensor, INPUT);
  
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, HIGH);
  buzz_OFF();

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(RECV_PIN, INPUT);
  digitalWrite(RECV_PIN, HIGH);

  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
}
// NOTE: IR_Tick() determines the mode, then the switch-case figures out which mode's code to execute. Mode is an integer.
void loop() {
  IR_Tick();
  switch (mode) {
    case MODE_IR_DRIVE:
      Drive_Tick();
      break;
    case MODE_OBJ_FOLLOW:
      doUltraFollowTick();
      break;
    case MODE_OBJ_AVOID:
      doAvoidTick();
      break;
    case MODE_LINE_FOLLOW:
      lineFollow();
      break;
    case MODE_SPECIAL:
      alarm();
      break;
    default:
      break;
  }
    
}

// Motor control functions
void go_Back() {
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2, LOW);
  digitalWrite(LeftDirectPin1, HIGH);
  digitalWrite(LeftDirectPin2, LOW);
  analogWrite(speedPinL, motorSpeed);
  analogWrite(speedPinR, motorSpeed);
}

void go_Left() {
  digitalWrite(RightDirectPin1, HIGH);
  digitalWrite(RightDirectPin2, LOW);
  digitalWrite(LeftDirectPin1, LOW);
  digitalWrite(LeftDirectPin2, HIGH);
  analogWrite(speedPinL, motorSpeed);
  analogWrite(speedPinR, motorSpeed);
}

void go_Right() {
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2, HIGH);
  digitalWrite(LeftDirectPin1, HIGH);
  digitalWrite(LeftDirectPin2, LOW);
  analogWrite(speedPinL, motorSpeed);
  analogWrite(speedPinR, motorSpeed);
}

void go_Advance() {
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2, HIGH);
  digitalWrite(LeftDirectPin1, LOW);
  digitalWrite(LeftDirectPin2, HIGH);
  analogWrite(speedPinL, motorSpeed);
  analogWrite(speedPinR, motorSpeed);
}

void stop_Stop() {
  digitalWrite(RightDirectPin1, LOW);
  digitalWrite(RightDirectPin2, LOW);
  digitalWrite(LeftDirectPin1, LOW);
  digitalWrite(LeftDirectPin2, LOW);
}

void IR_Tick() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    Serial.println("-------------------");

    switch (results.value) {
      case btnUP:
        mode = MODE_IR_DRIVE;
        Drive_Num = GO_ADVANCE;
        break;
      case btnDOWN:
        mode = MODE_IR_DRIVE;
        Drive_Num = GO_BACK;
        break;
      case btnLEFT:
        mode = MODE_IR_DRIVE;
        Drive_Num = GO_LEFT;
        break;
      case btnRIGHT:
        mode = MODE_IR_DRIVE;
        Drive_Num = GO_RIGHT;
        break;
      case btnOK:
        mode = MODE_IR_DRIVE;
        Drive_Num = STOP_STOP;
        break;
      case btn1:
        mode = MODE_OBJ_FOLLOW;
        break;
      case btn2:
        mode = MODE_LINE_FOLLOW;
        break;
      case btn3:
        mode = MODE_OBJ_AVOID;
        break;
      case btn0:
        mode = MODE_SPECIAL;
        break;
      default:
        break;
    }
    irrecv.resume(); // Receive the next value
  }
}

void Drive_Tick() {
  
  
  switch (Drive_Num) {
    case GO_ADVANCE:
      go_Advance();
      JogFlag = true;
      JogTimeCnt = 1;
      JogTime = millis();
      break;
    case GO_LEFT:
      go_Left();
      JogFlag = true;
      JogTimeCnt = 1;
      JogTime = millis();
      break;
    case GO_RIGHT:
      go_Right();
      JogFlag = true;
      JogTimeCnt = 1;
      JogTime = millis();
      break;
    case GO_BACK:
      go_Back();
      JogFlag = true;
      JogTimeCnt = 1;
      JogTime = millis();
      break;
    case STOP_STOP:
      mode = MODE_IR_DRIVE; 
      stop_Stop();
      JogTime = 0;
      break;
    case LINE_FOLLOW:
      lineFollow();
      break;
    default:
      break;
  }
  Drive_Num = DEF;

  if (millis() - JogTime >= 200) {
    JogTime = millis();
    if (JogFlag == true) {
      stopFlag = false;
      if (JogTimeCnt <= 0) {
        JogFlag = false;
        stopFlag = true;
      }
      JogTimeCnt--;
    }
    if (stopFlag == true) {
      JogTimeCnt = 0;
      stop_Stop();
    }
  }
}

void lineFollow() {
  int leftSensorValue = analogRead(LEFT_SENSOR_PIN);
  int rightSensorValue = analogRead(RIGHT_SENSOR_PIN);

  if (leftSensorValue > SENSOR_THRESHOLD && rightSensorValue > SENSOR_THRESHOLD) {
    // Move forward
    Serial.println("fwd");
    go_Advance();
  } else if (leftSensorValue < SENSOR_THRESHOLD && rightSensorValue > SENSOR_THRESHOLD) {
    // Turn right
    Serial.println("lft");
    go_Left();
  } else if (leftSensorValue > SENSOR_THRESHOLD && rightSensorValue < SENSOR_THRESHOLD) {
    // Turn left
    Serial.println("rgt");
    go_Right();
  } else {
    // Stop
    Serial.println("stp"); // TODO: there are cases where all sensors are on, in which case it should inch forward.
    stop_Stop();
  }
}
// NOTE: Get distance from ultrasonic sensor to nearest object in front of the cat's "eyes".
float getUltraDistance() {
  float duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) * 0.0344;
  return distance;
}

// NOTE: Figure out wether to drive forward or not. Values subject to change for distances.
void doUltraFollowTick() {
  float currentDistance = getUltraDistance();
  Serial.println(currentDistance);
  if (currentDistance > 20.00 && currentDistance < 75.00) {
    Serial.println("went forward");
    go_Advance();
    JogFlag = true;
    JogTimeCnt = 1;
    JogTime = millis();
  } else {
    stop_Stop();
  }
}
void buzz_ON()   //open buzzer
{
  
  for(int i=0;i<100;i++) {
   digitalWrite(BUZZ_PIN,LOW);
   delay(2);//wait for 1ms
   digitalWrite(BUZZ_PIN,HIGH);
   delay(2);//wait for 1ms
  }
}
void buzz_OFF()  //close buzzer
{
  digitalWrite(BUZZ_PIN, HIGH);
  
}
// TODO: add buzzer pin to setup(), add buzzer button, and maybe add it to a couple other functions?
void alarm(){
   buzz_ON();
 
   buzz_OFF();
   mode = MODE_IR_DRIVE;
}
// NOTE: Determine whether the back IR sensors readings need movement, and which direction. Then continue.
// NOTE: Determine whether the back IR sensors readings need movement, and which direction. Then continue.
void doAvoidTick(){
  int IRvalueLeft = digitalRead(RightObstacleSensor);
  int IRvalueRight = digitalRead(LeftObstacleSensor);
  if (IRvalueLeft==LOW && IRvalueRight==LOW) {
    //both sensors detected. go forward.
    go_Advance();
  }
  else if (IRvalueLeft==HIGH && IRvalueRight==HIGH) {
    stop_Stop();  //no obstacle, stop
  }
  else if (IRvalueLeft==LOW && IRvalueRight==HIGH) {
    //only left sensor detected an obstacle
    go_Left();  //Turn left
  }
  else if (IRvalueLeft==HIGH && IRvalueRight==LOW) { 
    //only right sensor detected an obstacle
    go_Right();  //Turn right
  }
}

// TODO: move JogFlag and Time assignments to the end of their repspective movement codes to not rely on re-using it.

