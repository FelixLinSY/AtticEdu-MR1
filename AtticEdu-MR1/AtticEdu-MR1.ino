
#include <LRemote.h>
#include <Ultrasonic.h>

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

#define BLE_MR1_NAME            "AtticEdu-MR1"

#define SAFE_DISTANCE           30    /* cm */

/* Linkit Remote control object */
LRemoteLabel      LR_us_text;
LRemoteLabel      LR_us_value;
LRemoteSwitch     LR_us_enable;
LRemoteSlider     LR_safe_dist;
LRemoteButton     LR_frontward;
LRemoteButton     LR_backward;
LRemoteButton     LR_right;
LRemoteButton     LR_left;
LRemoteSwitch     LR_auto_drive;
LRemoteSwitch     LR_light_f;
LRemoteSwitch     LR_light_r;

Ultrasonic  ultrasonic(PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO);
int         auto_drive = 0;
int         avoid = 0;
int         distance, last_distance = 999;
int         safe_distance = SAFE_DISTANCE;

void setup() {
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
  
  /* stop all motors */
  //  rear_stop();
  //  front_stop();

  /* setup luinkit remote control */
  LRemote.setName(BLE_MR1_NAME);
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(3, 5);

  /* Add ultrasonic label */
  LR_us_text.setText("distance: ");
  LR_us_text.setPos(0, 0);
  LR_us_text.setSize(1, 1);
  LR_us_text.setColor(RC_GREY);
  LRemote.addControl(LR_us_text);
  
  LR_us_value.setText(" ");
  LR_us_value.setPos(1, 0);
  LR_us_value.setSize(1, 1);
  LR_us_value.setColor(RC_GREY);
  LRemote.addControl(LR_us_value);

  /* Add ultrasonic enable switch */
  LR_us_enable.setText("US Enable");
  LR_us_enable.setPos(2, 0);
  LR_us_enable.setSize(1, 1);
  LR_us_enable.setColor(RC_BLUE);
  LRemote.addControl(LR_us_enable);
  
  /* add slide bar for safe distance */
  LR_safe_dist.setText("safe distance");
  LR_safe_dist.setPos(0, 1);
  LR_safe_dist.setSize(3, 1);
  LR_safe_dist.setColor(RC_ORANGE);
  LR_safe_dist.setValueRange(10, 50, SAFE_DISTANCE);
  LRemote.addControl(LR_safe_dist);
  
  /* add push button: forward */
  LR_frontward.setText("forward");
  LR_frontward.setPos(1, 2);
  LR_frontward.setSize(1, 1);
  LR_frontward.setColor(RC_GREEN);
  LRemote.addControl(LR_frontward);

  /* add push button: backward */
  LR_backward.setText("backward");
  LR_backward.setPos(1, 4);
  LR_backward.setSize(1, 1);
  LR_backward.setColor(RC_GREEN);
  LRemote.addControl(LR_backward);

  /* add push button: right */
  LR_right.setText("right");
  LR_right.setPos(2, 3);
  LR_right.setSize(1, 1);
  LR_right.setColor(RC_GREEN);
  LRemote.addControl(LR_right);

  /* add push button: left */
  LR_left.setText("left");
  LR_left.setPos(0, 3);
  LR_left.setSize(1, 1);
  LR_left.setColor(RC_GREEN);
  LRemote.addControl(LR_left);
  
  /* Add auto drive switch */
  LR_auto_drive.setText("Auto Drive");
  LR_auto_drive.setPos(1, 3);
  LR_auto_drive.setSize(1, 1);
  LR_auto_drive.setColor(RC_BLUE);
  LRemote.addControl(LR_auto_drive);

  /* Add light switch */
  LR_light_f.setText("Front Light");
  LR_light_f.setPos(0, 2);
  LR_light_f.setSize(1, 1);
  LR_light_f.setColor(RC_BLUE);
  LRemote.addControl(LR_light_f);
  
  LR_light_r.setText("Rear Light");
  LR_light_r.setPos(2, 2);
  LR_light_r.setSize(1, 1);
  LR_light_r.setColor(RC_BLUE);
  LRemote.addControl(LR_light_r);

  /* start BLE advertisement  */
  LRemote.begin();
  Serial.println("begin() returned");
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
  /**
     check if we are connect by some
     BLE central device, e.g. an mobile app
  */
  if (!LRemote.connected()) {
    Serial.println("waiting for connection");
    delay(500);
  } else {
    delay(10);
  }

  /** 
   * Process the incoming BLE write request 
   * and translate them to control events
   */
  LRemote.process();

  /* read ultrasonic */
  distance = ultrasonic.distanceRead();
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  
  if (distance != last_distance) {
    last_distance = distance;
    LR_us_value.updateText(String(distance, 10));
  }

  if (LR_safe_dist.isValueChanged()) {
    safe_distance = LR_safe_dist.getValue();
  }
  
  if (LR_us_enable.isValueChanged()) {
    if (LR_us_enable.getValue() == 1) {
      Serial.println("enable ultrasonic avoid");
      avoid = 1;
    } else {
      Serial.println("disable ultrasonic avoid");
      avoid = 0;
    }
  }
  
  if (LR_auto_drive.isValueChanged()) {
    if (LR_auto_drive.getValue() == 1) {
      Serial.println("enable auto drive");
      auto_drive = 1;
    } else {
      Serial.println("disable auto drive");
      auto_drive = 0;
      freeze();
    }
  }

  if (LR_light_f.isValueChanged()) {
    if (LR_light_f.getValue() == 1) {
      Serial.println("turn the front light on");
      front_light(1);
    } else {
      Serial.println("turn the front light off");
      front_light(0);
    }
  }

  if (LR_light_r.isValueChanged()) {
    if (LR_light_r.getValue() == 1) {
      Serial.println("turn the rear light on");
      rear_light(1);
    } else {
      Serial.println("turn the rear light off");
      rear_light(0);
    }
  }

  if (auto_drive) {
    if (distance <= safe_distance) {
      turn_left();
    } else {
      forward();
    }
    delay(50);

  } else {
    if (LR_frontward.isValueChanged()) {
      if (LR_frontward.getValue() == 1) {
        Serial.println("forward is pressed");
        if ((avoid) && (distance <= safe_distance)) {
          /* don't do anything */
        } else {
          forward();
        }
      } else {
        Serial.println("forward is released");
        freeze();
      }
    }

    if (LR_backward.isValueChanged()) {
      if (LR_backward.getValue() == 1) {
        Serial.println("backward is pressed");
        backward();
      } else {
        Serial.println("backward is released");
        freeze();
      }
    }

    if (LR_right.isValueChanged()) {
      if (LR_right.getValue() == 1) {
        Serial.println("right is pressed");
        turn_right();
      } else {
        Serial.println("right is released");
        freeze();
      }
    }

    if (LR_left.isValueChanged()) {
      if (LR_left.getValue() == 1) {
        Serial.println("left is pressed");
        turn_left();
      } else {
        Serial.println("left is released");
        freeze();
      }
    }

  }

}
