#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin definitions
#define SOIL1 A0
#define SOIL2 A1
#define SOIL3 A2
#define DHT_PIN 2
#define BUTTON_PIN 3

// DHT sensor type
#define DHT_TYPE DHT22

// Initialize LCD and DHT
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address if needed
DHT dht(DHT_PIN, DHT_TYPE);

// Variables
int buttonState = 0;
int lastButtonState = 0;
int displayMode = 0;
int Soil1, Soil2, Soil3;

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize DHT sensor
  dht.begin();

  // Set pin modes
  pinMode(BUTTON_PIN, INPUT);
  
  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Press button to");
  lcd.setCursor(0, 1);
  lcd.print("start!");

  for(int Pin=8;Pin<=10;Pin++) {
    pinMode(Pin, OUTPUT);
  }
}

void loop() {
  // Read button state
  buttonState = digitalRead(BUTTON_PIN);

  // Read soil values
  Soil1 = analogRead(SOIL1);
  Soil2 = analogRead(SOIL2);
  Soil3 = analogRead(SOIL3);

  Soil1 = map(Soil1, 0, 1023, 0, 100);
  Soil2 = map(Soil2, 0, 1023, 0, 100);
  Soil3 = map(Soil3, 0, 1023, 0, 100);

  // Detect button press (simple debounce logic)
  if (buttonState == HIGH && lastButtonState == LOW) {
    displayMode = (displayMode + 1) % 4; // Cycle through modes
    delay(200); // Debounce delay
  }
  lastButtonState = buttonState;

  // Clear LCD
  lcd.clear();

  // Display based on mode
  switch (displayMode) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Soil1: ");
      lcd.print(Soil1);
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Soil2:");
      lcd.print(Soil2);
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Soil3:");
      lcd.print(Soil3);
      break;

    case 3:
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.print(temperature);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Hum:");
      lcd.print(humidity);
      lcd.print("%");
      break;
  }


  if(Soil1 < 50) {
    digitalWrite(8, HIGH);
  } else if(Soil1 > 80) {
    digitalWrite(8, LOW);
  }

  if(Soil2 < 50) {
    digitalWrite(9, HIGH);
  } else if(Soil2 > 80) {
    digitalWrite(9, LOW);
  }

  if(Soil3 < 50) {
    digitalWrite(10, HIGH);
  } else if(Soil3 > 80) {
    digitalWrite(10, LOW);
  }
  // delay(100);
}
