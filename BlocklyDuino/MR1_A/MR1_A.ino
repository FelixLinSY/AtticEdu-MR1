/*
 * Generated using BlocklyDuino:
 *
 * https://github.com/MediaTek-Labs/BlocklyDuino-for-LinkIt
 *
 * Date: Sat, 27 Jan 2018 06:54:07 GMT
 */



void setup()
{
  Serial.begin(115200);


  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}


void loop()
{
  Serial.println("開燈");
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(200);
  Serial.println("關燈");
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(200);
}
