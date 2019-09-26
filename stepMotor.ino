#include <Stepper.h>

//2048->360 deg, 1024->180 deg
const int steps = 2048;

//밀리초 기준 (UL은 unsigned int형의 숫자라는 선언.)
unsigned long times = (2 * 3600 * 1000UL);

/* IN1->pin8
 * IN2->pin9
 * IN3->pin10
 * IN4->pin11
 */
Stepper myStepper(steps, 11, 9, 10, 8);

void setup() {
  /*
   * step() 함수를 실행할 때 스피드 설정
   */
  myStepper.setSpeed(15);  //15 RPM
}

void loop() {
  // 반시계방향 360도 * 2회
  myStepper.step(steps * 2);
  delay(times);

  // 시계방향 360도 * 1회
  myStepper.step(-steps);
  delay(times);
}
