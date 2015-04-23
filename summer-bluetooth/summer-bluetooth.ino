/***
DHT setting
***/
#include <DHT.h>

#define DHTPIN 7     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);

/***
HC-05 Bluetooth setting
***/
#include <SoftwareSerial.h>
SoftwareSerial bt(3, 2); // tx, rx
//HC05 btSerial = HC05(2, 5, 0, 1); // cmd, state, rx, tx
static char dtostrfbuffer[15];

void setup() {
  Serial.begin(9600);
  bt.begin(9600); // 블루투스를 사용하기 위해 초기화

  // DHT
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(3000);

  // DHT
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity() + 10;
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("{ \"humidity\": "); 
  Serial.print(h);
  Serial.print(", ");
  Serial.print("\"temperature\": "); 
  Serial.print(t);
  Serial.print(", ");
  
  
  // Sound Sensor
  int sound = analogRead(A0);
  Serial.print("\"sound\": "); 
  Serial.print(sound);
  Serial.print(", ");
  
  // Vibration Sensor
  int vibration = digitalRead(4);
  Serial.print("\"vibration\": "); 
  Serial.print(vibration);
  Serial.print(", ");
  Serial.print("\"location\": 2");
  
  Serial.println("}");

  // HC-05
  bt.write("{ \"humidity\": ");
  bt.write(dtostrf(h, 0, 2, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"temperature\": ");
  bt.write(dtostrf(t, 0, 2, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"sound\": ");
  bt.write(dtostrf(sound, 0, 2, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"vibration\": ");
  bt.write(dtostrf(vibration, 0, 2, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"location\": 2");
  bt.write("}\n");
}

