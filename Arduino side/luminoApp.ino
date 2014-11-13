#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

//Matrix LED part
//alpha
float   alpha=150;
boolean growAlpha=true;
int     pass = 0;
uint16_t colors= matrix.Color(0, 0, 0);
const uint16_t colors1[] = {
  matrix.Color(255, 0, 0), matrix.Color(255, 233, 84), matrix.Color(255, 140, 0), matrix.Color(255, 238, 80), matrix.Color(220, 20, 60), matrix.Color(255, 215, 0), matrix.Color(255, 99, 71), matrix.Color(218, 165, 32), matrix.Color(255, 80, 80), matrix.Color(255, 165, 0), matrix.Color(250, 128, 114), matrix.Color(238, 201, 0)};
const uint16_t colors2[] = {
  matrix.Color(255, 127,0), matrix.Color(255, 255,0), matrix.Color(184, 134,11), matrix.Color(255, 215, 0), matrix.Color(250, 128, 114), matrix.Color(173, 255, 47), matrix.Color(139, 69, 19), matrix.Color(205, 129, 98), matrix.Color(255, 140,0), matrix.Color(255, 99,71), matrix.Color(255, 255, 0), matrix.Color(124, 252, 0)};
const uint16_t colors3[] = {
  matrix.Color(247, 143, 216), matrix.Color(0, 255, 255), matrix.Color(255, 187, 255), matrix.Color(216, 191, 216), matrix.Color(134, 255, 97), matrix.Color(255, 228, 225), matrix.Color(135, 206, 250), matrix.Color(255, 20, 147), matrix.Color(127, 255, 212), matrix.Color(255, 228, 196), matrix.Color(102, 205, 170), matrix.Color(221, 160, 221)};
const uint16_t colors4[] = {
  matrix.Color(0, 0, 255), matrix.Color(186, 181, 255), matrix.Color(128, 0, 128), matrix.Color(0, 206, 209), matrix.Color(255, 192, 203), matrix.Color(47, 127, 208),matrix.Color(131, 0, 219), matrix.Color(135, 206, 250), matrix.Color(135, 206, 250), matrix.Color(78, 78, 201), matrix.Color(221, 160, 221), matrix.Color(196, 1, 151)};

//stepper motor part
int delaylength=50;

//wire
int wire1 = 4;
int wire2 = 5;
int wire3 = 10;
char result='0';

void setup(){
//Matrix LED part
  setupLED();
//stepper motor part
  setupMotor();
//wire part
  setupWire();
}

void setupLED(){
  matrix.begin();
  matrix.setBrightness(255);
}

void setupMotor(){
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
}

void setupWire(){
  pinMode(wire1, INPUT);
  pinMode(wire2, INPUT);
  pinMode(wire3, INPUT);
}

void loop(){
//Matrix LED part
  loopLED();
//wire part
  loopWire();
//programs part
  loopProgram();
}

void loopLED(){
  int lim;
  int freq;
  switch (result){
    case '1':
      //light
      freq=10;
      lim=255;
      break;
    case '2':
      //moderate
      freq=15;
      lim=235;
      break;
    case '3':
      //busy
      freq=20;
      lim=220;
      break;
    case '4':
      //sportive
      freq=25;
      lim=205;
      break;
    default:
      freq=0;
      lim=255;
      break;
  }
  alpha=changeAlpha(alpha, freq, growAlpha);
  growAlpha=changeGrow(alpha, lim, 15, growAlpha); 
  matrix.setBrightness(alpha);
}

void loopWire(){
    int result1 = digitalRead(wire1);
    int result2 = digitalRead(wire2);
    int result3 = digitalRead(wire3);
    if(result1==1 and result2==1 and result3==1){
      result='1';
    }
    else if(result1==1 and result2==1 and result3==0){
      result='2';
    }
    else if(result1==1 and result2==0 and result3==1){
      result='3';
    }
    else if(result1==0 and result2==1 and result3==1){
      result='4';
    }
    else if(result1==0 and result2==0 and result3==0){
      result='0';
    }
}

void loopProgram(){
  switch(result){
    case '1':
      //light
      program(delaylength); //if value< 25, it stops
      break;
    case '2':
      //moderate
      program(delaylength*2);
      break;
    case '3':
      //busy
      program(delaylength*3);
      break;
    case '4':
      //sportive
      program(delaylength*4);
      break;
    default:
      breakSequence();
      break;  
  }
}

void program(int frequence){
  sequence();
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(12, HIGH);
  analogWrite(3, 1023);
  delay(frequence);
  
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW);
  analogWrite(11, 1023);
  delay(frequence);
  
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(12, LOW);
  analogWrite(3, 1023);
  delay(frequence);
    
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, HIGH);
  analogWrite(11, 1023);
  delay(frequence);
}

void sequence(){
  switch(result){
    case '1':
      colors= colors1[pass];
      break;
    case '2':
      colors=colors2[pass];
      break;
    case '3':
      colors=colors3[pass];
      break;
    case '4':
      colors=colors4[pass];
      break;
    default:
      break; 
  }
  matrix.fillScreen(colors);
  matrix.show();
}

void breakSequence(){
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.show();
}

boolean changeGrow(float value, int limitUp, int limitDown, boolean grow){
   if(value >= limitUp){
    grow=false;
   }
   else if(value<=limitDown){
    grow=true;
    pass++;
   }
   if(pass > 11) 
      pass = 0;   
   return grow;
}

float changeAlpha(float value, int frequence, boolean grow){
    if(grow)
      value+= frequence;
    else 
      value-= frequence;
    return value;
}
