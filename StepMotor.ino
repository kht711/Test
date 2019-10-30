#include <Stepper.h>
#include <Arduino.h>
#include <TM1637Display.h>

//2048->360 deg, 1024->180 deg
const int steps = 2048;

/* IN1->pin8
 * IN2->pin9
 * IN3->pin10
 * IN4->pin11
 */
Stepper myStepper(steps, 11, 9, 10, 8);

//-------------------------------------------------

//TM1637 모듈에 있는 핀 CLK, DIO를 디지털 핀 3, 2로 설정
#define CLK 3
#define DIO 2

//TM1637 관련 변수들
unsigned long s;
int dotFlag = 0;
uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };

//타이머 변수. 시, 분, 초 관리.
int hour;
int minute;
int second;

//설정하고 싶은 시간 설정 (2시간 설정)
#define HOUR 0
#define MINUTE 1
#define SECOND 0

TM1637Display display(CLK, DIO);

void TimerInit(){
  hour = HOUR;
  minute = MINUTE;
  second = SECOND;
  data[0] = display.encodeDigit(hour / 10);
  data[1] = 0x80 + display.encodeDigit(hour % 10);
  data[2] = display.encodeDigit(minute / 10);
  data[3] = display.encodeDigit(minute % 10);
  display.setBrightness(0x0f);
  display.setSegments(data);
  s = millis();
}

void TimerDisplay(){
  //디스플레이 초기화
  TimerInit();
  
  //무한 반복
  while(1){
    //hour가 0, 그리고 minute가 0이 되는 조건이면
    //타이머 끄면서 함수 빠져나오기
    if (hour == 0 && minute == 0){
      display.setBrightness(0x00);
      return;
    }
    
    //콜론 ON, OFF
    if (millis() - s > 500){
      s = millis();
      //콜론 없애기
      if (dotFlag == 0){
        dotFlag = 1;
        data[1] = display.encodeDigit(hour % 10);
      }
      //콜론 표시하기
      else{
        dotFlag = 0;
        data[1] = 0x80 + display.encodeDigit(hour % 10);
        second++;
      }
    }
    
    //60초 경과
    if (second == 60){
      s = millis();
      second = 0;
      if (minute == 0){
        minute = 59;
        hour--;
      }else{
        minute--;
      }
      data[0] = display.encodeDigit(hour / 10);
      data[1] = 0x80 + display.encodeDigit(hour % 10);
      data[2] = display.encodeDigit(minute / 10);
      data[3] = display.encodeDigit(minute % 10);
    }
    display.setSegments(data);
  }
}

//-------------------------------------------------

void setup() {
  delay(15000UL);
  //step() 함수를 실행할 때 스피드 설정
  myStepper.setSpeed(15);  //15 RPM
}

void loop() {
  // 반시계방향
  myStepper.step(steps * 9);
  myStepper.step(steps * 8);

  TimerDisplay();

  // 시계방향
  myStepper.step(-steps * 9);
  myStepper.step(-steps * 8);
  
  TimerDisplay();
}
