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

/***
GP2Y1010AU0F setting
***/
int measurePin = 1; //Connect dust sensor to Arduino A0 pin
int ledPower = 8;   //Connect 3 led driver pins of dust sensor to Arduino D2
  
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
  
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {
  Serial.begin(9600);
  bt.begin(9600); // 블루투스를 사용하기 위해 초기화

  // DHT
  dht.begin();
  
  // GP2Y1010AU0
  pinMode(ledPower, OUTPUT);
}

void loop() {
  String output = "";
  // Wait a few seconds between measurements.

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

  output += "{ \"location\": 2, \"humidity\": "; 
  output += h;
  output += ", ";
  output += "\"temperature\": "; 
  output += t;
  output += ", ";
 
  // Sound Sensor
  int sound = analogRead(A0) - 40;
  output += "\"sound\": "; 
  output += sound;
  output += ", ";
  
  // Vibration Sensor
  int vibration = digitalRead(4);
  output += "\"vibration\": "; 
  output += vibration;


  // GP2Y1010AU0
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  
  voMeasured = analogRead(measurePin); // read the dust value
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5 / 1024.0);  
  
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1; // unit: mg/m3
  dustDensity = dustDensity * 1000; // unit: ug/m3
  dustDensity = dustDensity >= 0 ? dustDensity : 0;
  
  output += ", ";
  output += "\"dust\": "; 
  output += dustDensity;
  
  output += "}";
  
  Serial.println(output);
  delay(1000);
  
  // HC-05
  bt.write("{ \"location\": 2, \"humidity\": ");
  bt.write(dtostrf(h, 0, 2, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"temperature\": ");
  bt.write(dtostrf(t, 0, 2, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"sound\": ");
  bt.write(dtostrf(sound, 0, 0, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"vibration\": ");
  bt.write(dtostrf(vibration, 0, 0, dtostrfbuffer));
  bt.write(", ");
  bt.write("\"dust\": ");
  bt.write(dtostrf(dustDensity, 0, 2, dtostrfbuffer));
  bt.write("}\n");
}

