/***************************************************************************
 Revision: 1.0.0 - 2022/05/24 - initial release
 Layad Circuits Electronics Engineering Supplies and Services
 B314 Lopez Bldg., Session Rd. cor. Assumption Rd., Baguio City, Philippines
 www.layadcircuits.com
 general: info@layadcircuits.com
 sales: sales@layadcircuits.com
 +63-916-442-8565
 ***************************************************************************/


#define PIN_TRIG1 A4
#define PIN_ECHO1 A5
#define PIN_TRIG2 A1
#define PIN_ECHO2 A2
#define PIN_TRIG3 12
#define PIN_ECHO3 A3

#define AIN1 2 
#define BIN1 7
#define AIN2 4
#define BIN2 5
#define PWMA 3
#define PWMB 6
#define STBY 8
#define SENSOR_L A0
#define SENSOR_C A1
#define SENSOR_R A2

#define MAX_DISTANCE 100     // Maximum distance (in cm) to sense.
#define MOVEMENT_DIST_MAX 30 //max distance where the robot could move
#define MOVEMENT_DIST_MIN 6  //min distance where the robot could move

const byte  DIST_MID=0; // distance sensed is in between min and max
const byte  DIST_MIN=1; // distance is at the minimum
const byte  DIST_MAX=2; // distance is at the maximum

float duration;
float distance;
unsigned int dist[3];
unsigned int distL;
unsigned int distC;
unsigned int distR;


byte checkdist(unsigned int d)
{
  byte retval=  DIST_MID;

  if(d < MOVEMENT_DIST_MIN) retval = DIST_MIN;
  else if(d < MOVEMENT_DIST_MAX) retval = DIST_MID;
  else  retval = DIST_MAX;

  return retval;
}


void speedSetting(byte val)
{
  // set speed of motor A
  analogWrite(PWMA,val);
  // set speed of motor B
  analogWrite(PWMB,val);
}

void forward()
{
  // move right motor(s) forward
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  // move left motor(s) forward
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void backward()
{
  // move right motor(s) backward
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  // move right motor(s) backward
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void turnleft()
{
  // move right motor(s) forward
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  // move left motor(s) backward
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,HIGH);
}

void turnright()
{
  // move right motor(s) backward
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,HIGH);
  // move left motor(s) forward
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,LOW);
}

void motorstop()
{
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
  digitalWrite(BIN1,LOW);
  digitalWrite(BIN2,LOW);
}

void shortbreak()
{
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,HIGH);
  digitalWrite(BIN1,HIGH);
  digitalWrite(BIN2,HIGH);
}

void ultrasonicroutine(){
  static unsigned long t;
  if(millis() - t < 35) return;
  t=millis();
  
   digitalWrite(PIN_TRIG1, LOW);
   delayMicroseconds(2);
   digitalWrite(PIN_TRIG1, HIGH);
   delayMicroseconds(10);
   digitalWrite(PIN_TRIG1, LOW);
   duration = pulseIn(PIN_ECHO1, HIGH, 29000);
   distance = duration * 0.034/2;
   if(distance >= MAX_DISTANCE)    distance = MAX_DISTANCE;
   if(distance == 0 )  distance = MAX_DISTANCE;
   distL = distance;
  
   digitalWrite(PIN_TRIG2, LOW);
   delayMicroseconds(2);
   digitalWrite(PIN_TRIG2, HIGH);
   delayMicroseconds(10);
   digitalWrite(PIN_TRIG2, LOW);
   duration = pulseIn(PIN_ECHO2, HIGH, 29000);
   distance = duration * 0.034/2;
   if(distance >= MAX_DISTANCE)    distance = MAX_DISTANCE;
   if(distance == 0 )  distance = MAX_DISTANCE;
   distC= distance;    

   digitalWrite(PIN_TRIG3, LOW);
   delayMicroseconds(2);
   digitalWrite(PIN_TRIG3, HIGH);
   delayMicroseconds(10);
   digitalWrite(PIN_TRIG3, LOW);
   duration = pulseIn(PIN_ECHO3, HIGH, 29000);
   distance = duration * 0.034/2;
   if(distance >= MAX_DISTANCE)    distance = MAX_DISTANCE;
   if(distance == 0 )  distance = MAX_DISTANCE;
   distR = distance;        

   //display the distances for debugging
   Serial.print(distL);Serial.print(F("\t"));
   Serial.print(distC);Serial.print(F("\t"));
   Serial.print(distR);Serial.print(F("\t"));  
   Serial.println();  
} 

void setup() {
  Serial.begin(115200);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(STBY,OUTPUT);
  digitalWrite(STBY,HIGH);//enable driver
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(PIN_TRIG1,OUTPUT);
  pinMode(PIN_ECHO1,INPUT);
  pinMode(PIN_TRIG2,OUTPUT);
  pinMode(PIN_ECHO2,INPUT);
  pinMode(PIN_TRIG3,OUTPUT);
  pinMode(PIN_ECHO3,INPUT);

  // adjust the speed depending on how your robot reacts to the object
  // speed is from 0 to 255
  speedSetting(45);
  delay(1000); // add 1s delay
}

void loop() {
    ultrasonicroutine();

  // find the condition you need to keep the car in a ceratin movement
       if(checkdist(distL) ==  DIST_MAX     && checkdist(distC) ==  DIST_MAX      && checkdist(distR) ==  DIST_MID)   turnright();
  else if(checkdist(distL) ==  DIST_MAX     && checkdist(distC) ==  DIST_MID      && checkdist(distR) ==  DIST_MID)   turnright();
  else if(checkdist(distL) ==  DIST_MAX     && checkdist(distC) ==  DIST_MID      && checkdist(distR) ==  DIST_MIN)   turnright();      
  else if(checkdist(distL) ==  DIST_MID     && checkdist(distC) ==  DIST_MAX      && checkdist(distR) ==  DIST_MAX)   turnleft();
  else if(checkdist(distL) ==  DIST_MID     && checkdist(distC) ==  DIST_MID      && checkdist(distR) ==  DIST_MAX)   turnleft();
  else if(checkdist(distL) ==  DIST_MIN     && checkdist(distC) ==  DIST_MID      && checkdist(distR) ==  DIST_MAX)   turnleft();
  else if(checkdist(distL) ==  DIST_MAX     && checkdist(distC) ==  DIST_MID      && checkdist(distR) ==  DIST_MAX)   forward(); 
  else if(checkdist(distL) ==  DIST_MID     && checkdist(distC) ==  DIST_MID      && checkdist(distR) ==  DIST_MID)   
  {
         if(distC >= distL && distC >= distR) forward();
    else if(distC >= distL && distC <  distR) turnright();  
    else if(distC <  distL && distC >=  distR) turnleft();
    else if(distC <  distL && distC <   distR) forward();
    else if(distC == distL && distC == distR) forward();
    else motorstop();//all others not captures will cause the car to stop
  }
  else motorstop(); //all others not captures will cause the car to stop
}
