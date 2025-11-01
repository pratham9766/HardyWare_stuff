#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// I2C Pins for ESP32
#define I2C_SDA 22
#define I2C_SCL 23

// Create BMP280 object
Adafruit_BMP280 bmp;

// Sea level pressure (for altitude calculation)
#define SEALEVELPRESSURE_HPA (1013.25)

// Calibration variables
float altitudeOffset = 0.0;
bool offsetSet = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize BMP280
  if (!bmp.begin(0x76)) {   // try 0x77 if your module uses that
    Serial.println("Could not find BMP280 sensor!");
    while (1);
  }

  Serial.println("BMP280 initialized successfully.");
}

void loop() {
  float temp = bmp.readTemperature();            // °C
  float pressure = bmp.readPressure() / 100.0;   // hPa
  float rawAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA); // meters

  // Set offset on first valid reading
  if (!offsetSet) {
    altitudeOffset = rawAltitude;
    offsetSet = true;
    Serial.printf("Altitude offset set to: %.2f m\n", altitudeOffset);
  }

  // Apply calibration
  float calibratedAltitude = rawAltitude - altitudeOffset;

  Serial.println("===== BMP280 Reading =====");
  Serial.printf("Temperature: %.2f °C\n", temp);
  Serial.printf("Pressure: %.2f hPa\n", pressure);
  Serial.printf("Altitude (calibrated): %.2f m\n\n", calibratedAltitude);

  delay(2000);
}
