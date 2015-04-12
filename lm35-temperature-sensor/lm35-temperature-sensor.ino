#define SERIAL_BAUDRATE 9600

int outputPin = 0;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
}

void loop() {
  int rawvoltage = analogRead(outputPin);
  float millivolts= (rawvoltage/1024.0) * 5000;
  float celsius= millivolts/10;
  Serial.println(celsius);
  delay(5000);
}
