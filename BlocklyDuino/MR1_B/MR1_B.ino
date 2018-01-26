/*
 * Generated using BlocklyDuino:
 *
 * https://github.com/MediaTek-Labs/BlocklyDuino-for-LinkIt
 *
 * Date: Fri, 26 Jan 2018 14:12:25 GMT
 */



int debounce_time;

int light;

void setup()
{
  Serial.begin(115200);

  Serial.println("使用內建按鈕操作車頭燈");
  debounce_time = 100;
  light = 0;
  pinMode(6, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}


void loop()
{
  if (digitalRead(6) == HIGH) {
    Serial.println("按鈕被按下");
    delay(debounce_time);
    if (digitalRead(6) == HIGH) {
      Serial.println("按鈕持續被按下超過抖動時間");
      if (light == 0) {
        light = 1;

      } else {
        light = 0;

      }

    }

  }
  if (light == 1) {
    Serial.println("開燈");
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);

  } else {
    Serial.println("關燈");
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);

  }
}