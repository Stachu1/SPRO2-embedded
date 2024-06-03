#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "fft.h"

#define BTN_PIN 9
#define MIC_PIN A2
#define HR_PIN A1
#define TEMP_PIN A0
#define MIC_SAMPLES 16
#define MIC_SAMPLE_RATE 2e3
#define SD_CS_PIN 10


void sd_init(File *file);
void measure(File *file);


void setup() {
  Serial.begin(115200);
  pinMode(MIC_PIN, INPUT);
  pinMode(HR_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  Serial.println("\n\nInitialising...\n");

  File file;
  sd_init(&file);

  while(digitalRead(BTN_PIN));
  while(!digitalRead(BTN_PIN));
  delay(200);
  measure(&file);
}

void loop() {}

void sd_init(File *file) {
  Serial.print("SD card - ");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("\033[91mError!\033[0m");
    while (1);
  }
  Serial.println("\033[92mOK\033[0m");

  Serial.print("Formatting - ");
  SD.remove("data.txt");
  *file = SD.open("data.txt", FILE_WRITE);
  if (!*file) {
    Serial.println("\033[91mError!\033[0m");
    while (1);
  }
  Serial.println("\033[92mOK\033[0m");
}

void measure(File *file) {
  Serial.print("Measuring - ");
  uint16_t data[MIC_SAMPLES];
  complex_t x[MIC_SAMPLES];

  uint16_t i = 0;
  uint32_t t_start = millis();
  uint32_t t_update = 0;
  while(digitalRead(BTN_PIN)) {
    // Sample microphone
    for (int i = 0; i < MIC_SAMPLES; i++) {
      data[i] = analogRead(MIC_PIN);
      delayMicroseconds(1e6 / MIC_SAMPLE_RATE - 118);
    }

    // Convert to complex
    for (int i = 0; i < MIC_SAMPLES; i++) {
      x[i] = cvalue((float)data[i], 0.0f);
    }

    // FFT
    fft(x, MIC_SAMPLES);

    // Format and save the data
    String data = "";
    data += String(millis() - t_start) + ";" + String(analogRead(HR_PIN)) + ";" + String(analogRead(TEMP_PIN)) + ";";
    
    for(int i = 0; i < MIC_SAMPLES/2+1; i++) {
      data += String(cabs(x[i])) + ",";
    }
    file->println(data);

    i++;
  }
  file->close(); 
  Serial.print("\033[92mOK\033[0m\n\n[");
  uint16_t t_s = (millis() - t_start) / 1000;
  uint16_t t_m = t_s / 60;
  uint16_t t_h = t_h / 60;
  Serial.print(String(t_h) + "h " + String(t_m%60) + "m " + String(t_s%60) + "s of data saved to the SD card]\nAvg cycle time: ");
  Serial.println(((float)millis() - t_start) / i);
}


// TESTSING:
// MIC_SAMPLES 64
// MIC_SAMPLE_RATE 2000
// TPS 112.96ms

// MIC_SAMPLES 32
// MIC_SAMPLE_RATE 1000
// TPS 67.14ms

// MIC_SAMPLES 16
// MIC_SAMPLE_RATE 1000
// TPS 31.21ms