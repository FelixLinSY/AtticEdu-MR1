/*
 * Generated using BlocklyDuino:
 *
 * https://github.com/MediaTek-Labs/BlocklyDuino-for-LinkIt
 *
 * Date: Fri, 26 Jan 2018 13:49:22 GMT
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
  delay(1000);
  Serial.println("關燈");
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(1000);
}