#include <SoftwareSerial.h>
#define BTSerial Serial1
//SoftwareSerial BTSerial(10,11); // RX | TX
void setup()
{
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);
  Serial.begin(38400);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);  // HC-06 current bound rate (default 9600)
}
void loop()
{
  // Keep reading from HC-06 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());
  // Keep reading from Arduino Serial Monitor and send to HC-06
  if (Serial.available())
    BTSerial.write(Serial.read());
}
