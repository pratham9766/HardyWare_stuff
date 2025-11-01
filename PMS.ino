/* ESP32 PMS - Serial Print Only
   Reads PMS data over UART and prints values to Serial
   SD logging is temporarily disabled
*/

#include <PMS.h>
#include <HardwareSerial.h>

// ---------------- Pin Config ----------------
#define PMS_RX 14
#define PMS_TX 27
// #define SD_CS 32  // SD card temporarily disabled

HardwareSerial pmsSerial(2);
PMS pms(pmsSerial);
PMS::DATA pmsData;

// ---------------- Timing ----------------
const unsigned long PMS_INTERVAL_MS = 2000; // 0.5 Hz

// ---------------- Helpers ----------------
// SD logging temporarily disabled
/*
void logToSD(unsigned long t, int pm1, int pm25, int pm10) {
  File f = SD.open("pms_log.csv", FILE_APPEND);
  if (f) {
    f.printf("%lu,%d,%d,%d\n", t, pm1, pm25, pm10);
    f.flush();
    f.close();
  } else {
    Serial.println("Error opening log file!");
  }
}
*/

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);
  delay(2000);

  // ---- SD card init temporarily disabled ----
  /*
  if (!SD.begin(SD_CS)) {
    Serial.println("SD card init failed!");
    while (1);
  }

  if (!SD.exists("pms_log.csv")) {
    File f = SD.open("pms_log.csv", FILE_WRITE);
    if (f) {
      f.println("Time(ms),PM1,PM2.5,PM10");
      f.flush();
      f.close();
      Serial.println("Created PMS log file with header.");
    }
  }
  */

  // ---- PMS init ----
  pmsSerial.begin(9600, SERIAL_8N1, PMS_RX, PMS_TX);
  pms.passiveMode(); // Read on request
}

// ---------------- Loop ----------------
void loop() {
  static unsigned long lastRead = 0;
  if (millis() - lastRead >= PMS_INTERVAL_MS) {
    lastRead = millis();

    if (pms.readUntil(pmsData, 1000)) {
      // Print formatted values to Serial
      Serial.println("======== PMS Reading ========");
      Serial.printf("Time: %lu ms\n", millis());
      Serial.printf("PM1:   %d ug/m3\n", pmsData.PM_AE_UG_1_0);
      Serial.printf("PM2.5: %d ug/m3\n", pmsData.PM_AE_UG_2_5);
      Serial.printf("PM10:  %d ug/m3\n", pmsData.PM_AE_UG_10_0);
      Serial.println("============================");

      // SD logging temporarily disabled
      // logToSD(millis(),
      //         pmsData.PM_AE_UG_1_0,
      //         pmsData.PM_AE_UG_2_5,
      //         pmsData.PM_AE_UG_10_0);
    } else {
      Serial.println("PMS read failed!");
    }
  }
}
