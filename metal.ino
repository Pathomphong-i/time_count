#include <TM1637Display.h>    // Download at -> https://github.com/avishorp/TM1637
#include <Wire.h> //I/O

//Sensor metal
int monitoring_one;
int monitoring_two;
int metalDetection_one = 1;//pin A1
int metalDetection_two = 2; //pin A2

#define CLK 2    // TM1637 CLK
#define DIO 3    // TM1637 DIO


bool RUN = false;
int MIN = 0;
int SEC = 0;
int HALFSEC = 0;
int MilSEC = 0;

int ballStatus = 0;

unsigned long LAST_COUNT_TIME, GET_COUNT_TIME;

TM1637Display display(CLK, DIO);

void setup() {
  Serial.begin(57600);
  display.setBrightness(0x0f);
  display.clear();
  
}

void loop() {

  monitoring_one = analogRead(metalDetection_one);
  monitoring_two = analogRead(metalDetection_two);
  if(monitoring_one>=250 && monitoring_two>=250 && ballStatus==0)
  {
    delay(5000);
    RESET_PROGRAM();
  }

  else if(monitoring_one<250 && monitoring_two>=250 && ballStatus==0)
  {
    Serial.println("start");
    ballStatus = 1;
    LAST_COUNT_TIME = millis();
    RUN = true;
  }

  else if(monitoring_one>=250 && monitoring_two>=250 && ballStatus==1)
  {
    Serial.println("runing");
    ballStatus = 2;
  }
  else if(monitoring_one>=250 && monitoring_two<250 && ballStatus==2)
  {
    Serial.println("stop");
    ballStatus = 0;
    RUN = false;
  }
  
  
  // ถ้า RUN มีสถานะ true ทำงานฟังก์ชั่นนับ
  if(RUN == true){
    MAIN_COUNT();
  }

}


void MAIN_COUNT(){
  // ใช้คำสั่ง millis() แทน delay() เพื่อให้ไม่ไปขัดการทำงานอื่น ๆ
  // ใช้ 500ms เพื่อให้เครื่องหมาย : กระพริบ
  GET_COUNT_TIME = millis();
  if(GET_COUNT_TIME - LAST_COUNT_TIME >= 10){
    MilSEC++;
    
    if(MilSEC%50 == 0){
      HALFSEC++;
      
      if(HALFSEC%2 == 0){
        SEC++;
        
        if(SEC > 59){
          SEC = SEC - 60;
          MIN++;
        }
        if(MilSEC > 99){
          MilSEC = MilSEC-100;
        }
        display.showNumberDec(SEC, true, 2, 0);
        display.showNumberDec(MilSEC, true, 2, 2);
      }

      if(HALFSEC%2 == 1){
        display.showNumberDecEx(SEC, 0b01000000, true, 2, 0);
        display.showNumberDecEx(MilSEC, 0b01000000, true, 2, 2);
      }
      LAST_COUNT_TIME = millis();
    }
  }
}

void RESET_PROGRAM(){
  RUN = false;
  display.clear();
  HALFSEC = 0;
  SEC = 0;
  MIN = 0;
  MilSEC = 0;
}
