#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define sensor A0
#define DHT11PIN 2
#define DHTTYPE DHT11

DHT dht(DHT11PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasLevel = analogRead(sensor);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // --- Air Quality Logic ---
  String airStatus = "";
  if (gasLevel < 250) {
    airStatus = "Good";
  } else if (gasLevel >= 250 && gasLevel < 500) {
    airStatus = "Average";
  } else {
    airStatus = "Bad";
  }

  // --- Display Logic ---
  display.clearDisplay();
  display.setTextSize(1);
  
  // Row 1: Temperature
  display.setCursor(0, 10);
  display.print("Temp: "); display.print(t); display.print(" C");

  // Row 2: Humidity
  display.setCursor(0, 25);
  display.print("Humi: "); display.print(h); display.print(" %");

  // Row 3: Gas Level
  display.setCursor(0, 40);
  display.print("Gas : "); display.print(gasLevel);

  // Row 4: Air Quality Status (The part you requested)
  display.setCursor(0, 55);
  display.print("Air : "); 
  display.setTextSize(1);
  display.print(airStatus);

  display.display();
  delay(2000); 
}