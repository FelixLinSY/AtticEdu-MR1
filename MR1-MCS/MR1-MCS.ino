#include <LWiFi.h>
#include <WiFiClient.h>
#include <Ultrasonic.h>
#include "MCS.h"

#define PIN_ULTRASONIC_TRIG     8
#define PIN_ULTRASONIC_ECHO     9
#define PIN_RIGHT_WHEEL_A       10
#define PIN_RIGHT_WHEEL_B       12
#define PIN_LEFT_WHEEL_A        13
#define PIN_LEFT_WHEEL_B        17
#define PIN_LIGHT_FRONT_L       2
#define PIN_LIGHT_FRONT_R       3
#define PIN_LIGHT_REAR_L        4
#define PIN_LIGHT_REAR_R        5

// Assign AP ssid / password here
#define _SSID                   "Your_WiFi_SSID"
#define _KEY                    "Your_WiFi_Password"

// Assign device id / key of your test device
MCSDevice mcs("Your_MCS_ID", "Your_MCS_KEY");

// Assign channel id
// The test device should have a channel id "control_gamepad".
// the first channel should be "Controller" - "GamePad"
MCSControllerGamePad gamepad("cmd");

void setup() {
  // setup Serial output at 115200
  Serial.begin(115200);
  
  /* initialize pin mode */
  pinMode(PIN_RIGHT_WHEEL_A, OUTPUT);
  pinMode(PIN_RIGHT_WHEEL_B, OUTPUT);
  pinMode(PIN_LEFT_WHEEL_A, OUTPUT);
  pinMode(PIN_LEFT_WHEEL_B, OUTPUT);
  
  pinMode(PIN_LIGHT_FRONT_L, OUTPUT);
  pinMode(PIN_LIGHT_FRONT_R, OUTPUT);
  pinMode(PIN_LIGHT_REAR_L, OUTPUT);
  pinMode(PIN_LIGHT_REAR_R, OUTPUT);
  
  // turn off the light
  front_light(0);
  rear_light(0);

  freeze();
  
  // setup Wifi connection
  while (WL_CONNECTED != WiFi.status()) {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  // setup MCS connection
  mcs.addChannel(gamepad);

  while (!mcs.connected()) {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  while (!gamepad.valid()) {
    Serial.println("initialize gamepad default value...");
    gamepad.value();
    // Note: At this moment we can "read" the values
    // of the gamepad - but the value is meaningless.
    //
    // The MCS server returns that "last button pressed"
    // in this cause - even if the user is not pressing any button
    // at this moment.
    //
    // We read the values here simply to make the following
    // process() -> if(gamepad.updated()) check working.
  }

}

void freeze() {
  digitalWrite(PIN_RIGHT_WHEEL_A, LOW);
  digitalWrite(PIN_RIGHT_WHEEL_B, LOW);
  digitalWrite(PIN_LEFT_WHEEL_A, LOW);
  digitalWrite(PIN_LEFT_WHEEL_B, LOW);
}

void forward() {
  digitalWrite(PIN_RIGHT_WHEEL_A, LOW);
  digitalWrite(PIN_RIGHT_WHEEL_B, HIGH);
  digitalWrite(PIN_LEFT_WHEEL_A, LOW);
  digitalWrite(PIN_LEFT_WHEEL_B, HIGH);
}

void backward() {
  digitalWrite(PIN_RIGHT_WHEEL_A, HIGH);
  digitalWrite(PIN_RIGHT_WHEEL_B, LOW);
  digitalWrite(PIN_LEFT_WHEEL_A, HIGH);
  digitalWrite(PIN_LEFT_WHEEL_B, LOW);
}

void turn_right() {
  digitalWrite(PIN_RIGHT_WHEEL_A, HIGH);
  digitalWrite(PIN_RIGHT_WHEEL_B, LOW);
  digitalWrite(PIN_LEFT_WHEEL_A, LOW);
  digitalWrite(PIN_LEFT_WHEEL_B, HIGH);
}

void turn_left() {
  digitalWrite(PIN_RIGHT_WHEEL_A, LOW);
  digitalWrite(PIN_RIGHT_WHEEL_B, HIGH);
  digitalWrite(PIN_LEFT_WHEEL_A, HIGH);
  digitalWrite(PIN_LEFT_WHEEL_B, LOW);
}

void front_light(int on_off) {
  if (on_off) {
    digitalWrite(PIN_LIGHT_FRONT_L, HIGH);
    digitalWrite(PIN_LIGHT_FRONT_R, HIGH);
  } else {
    digitalWrite(PIN_LIGHT_FRONT_L, LOW);
    digitalWrite(PIN_LIGHT_FRONT_R, LOW);
  }
}

void rear_light(int on_off) {
  if (on_off) {
    digitalWrite(PIN_LIGHT_REAR_L, HIGH);
    digitalWrite(PIN_LIGHT_REAR_R, HIGH);
  } else {
    digitalWrite(PIN_LIGHT_REAR_L, LOW);
    digitalWrite(PIN_LIGHT_REAR_R, LOW);
  }
}

void loop() {
  // Note that each process consumes 1 command from MCS server.
  // The 100 millisecond timeout assumes that the server
  // won't send command rapidly.
  mcs.process(100);

  // updated flag will be cleared in process(), user must check it after process() call.
  if (gamepad.updated()) {
    Serial.print("Gamepad event arrived, new value = ");
    Serial.println(gamepad.value());
    if (gamepad.value().event == BTN_PRESSED) {
      switch (gamepad.value().button) {
        case BTN_LEFT:
            turn_left();
          break;
        case BTN_RIGHT:
            turn_right();
          break;
        case BTN_A:
            forward();
          break;
        case BTN_B:
            backward();
          break;
        
      }
    } else if (gamepad.value().event == BTN_RELEASED) {
       freeze();
    }
    
  }

  // check if need to re-connect
  while (!mcs.connected()) {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if (mcs.connected()) {
      Serial.println("MCS connected !!");
    }
  }
}
