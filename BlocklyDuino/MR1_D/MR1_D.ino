/*
 * Generated using BlocklyDuino:
 *
 * https://github.com/MediaTek-Labs/BlocklyDuino-for-LinkIt
 *
 * Date: Fri, 26 Jan 2018 15:03:11 GMT
 */

#include <Otto.h>

Otto __otto;

#define OTTO_PIN_ULTRASONIC_TRIGGER 8
#define OTTO_PIN_ULTRASONIC_ECHO 9

void setup()
{
  Serial.begin(115200);

  __otto.initUltrasonic(OTTO_PIN_ULTRASONIC_TRIGGER, OTTO_PIN_ULTRASONIC_ECHO);
  Serial.println("MR1行走避障範例");
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(17, OUTPUT);
}


void loop()
{
  if (__otto.getDistance() <= 30) {
    Serial.println("右轉");
    digitalWrite(10, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    digitalWrite(17, HIGH);
    delay(100);

  } else {
    Serial.println("前進");
    digitalWrite(10, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    digitalWrite(17, HIGH);
    delay(50);

  }
}