int outputPin = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int rawvoltage = analogRead(outputPin);
  float millivolts= (rawvoltage/1024.0) * 5000;
  float celsius= millivolts/10;
  Serial.print(celsius);
  Serial.print(" degrees Celsius, ");
  
  Serial.print((celsius * 9)/5 + 32);
  Serial.println(" degrees Fahrenheit");
  delay(1000);
}
