#include "LedControl.h"
#include "binary.h"
#include "HX711.h"
#include "HX711.h"

#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(9, 8, 13, 12, 11, 10); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

//LED MATRIX
int DIN = 3;
int LOAD = 2;
int CLK = 4;

//WEIGHT SENSOR
int DT  = 6;
int SK = 7;

HX711 scale;
LedControl lc=LedControl(DIN,CLK,LOAD,1);

float CALIB_F= 108;

byte hf[8]= { 0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C };
byte sf[8]= { 0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C };
byte nf[8]= { 0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C };

void setup() {
  scale.set_scale(CALIB_F);
  scale.begin(DT, SK);
  pinMode(5,OUTPUT);
  lc.shutdown(0,false);       
  lc.setIntensity(0,5);  
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
  lcd.print("Hello Shubham");
  Serial.begin(9600);
  scale.tare();
  digitalWrite(5,LOW);
}

void loop() {
  //for(long i = -10000;i<=10000;i+=10){
    //scale.set_scale(i);
    //Serial.println("Calib = "+ String(i)+"VALUE = " + scale.get_units());
  //}
  int p = 0;
  for (int i = 0;i<5;i++)
    p+= scale.get_units();
  p/=10;
  if (p<0) p=0;
  lcd.clear();
  lcd.print(String(p) + " grams");

  if(p==0){
  setLEDMatrix(0);
  digitalWrite(5,LOW);
  }
  else if(p<80){
  setLEDMatrix(2);
  digitalWrite(5,LOW);
  }
  else{
  setLEDMatrix(1);
  digitalWrite(5,HIGH);
  }
  
}

void setLEDMatrix(int a){
  int i = 0;
  for(i=0;i<8;i++)
  {
    if(a==0)
      lc.setRow(0,i,nf[i]);
    if(a==1)
      lc.setRow(0,i,sf[i]);
    if(a==2)
      lc.setRow(0,i,hf[i]);
    
  }
}
