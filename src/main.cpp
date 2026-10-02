#include <Arduino.h>

#include "audio/AudioDSP.h"

namespace {

using laudspeaker::AudioDSPManager;

AudioDSPManager dsp;

void printMenu() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("ESP32-S3 Laudspeaker Management System");
  Serial.println("========================================");
  Serial.println("1. Show EQ");
  Serial.println("2. Set EQ band");
  Serial.println("3. Show crossover");
  Serial.println("4. Set crossover");
  Serial.println("5. Show gain");
  Serial.println("6. Set gain");
  Serial.println("7. Process test signal");
  Serial.println("8. Reset to default profile");
  Serial.println("0. Show menu");
  Serial.println("========================================");
}

void waitForInput() {
  Serial.println("\nPress Enter to continue...");
  while (Serial.available() == 0) {
    delay(10);
  }
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void showEq() {
  Serial.println("\nCurrent EQ values (dB):");
  for (size_t i = 0; i < dsp.eqBands().size(); ++i) {
    Serial.printf("Band %d: %.2f dB\n", i + 1, dsp.eqBands()[i]);
  }
}

void showCrossover() {
  Serial.println("\nCurrent crossover settings:");
  Serial.printf("Low cutoff: %.1f Hz\n", dsp.crossover().lowCutHz);
  Serial.printf("Mid low: %.1f Hz\n", dsp.crossover().midLowHz);
  Serial.printf("Mid high: %.1f Hz\n", dsp.crossover().midHighHz);
  Serial.printf("High cutoff: %.1f Hz\n", dsp.crossover().highCutHz);
}

void showGain() {
  auto g = dsp.gains();
  Serial.println("\nCurrent gain settings:");
  Serial.printf("Master gain: %.2f dB\n", g.master);
  Serial.printf("Low gain: %.2f dB\n", g.low);
  Serial.printf("Mid gain: %.2f dB\n", g.mid);
  Serial.printf("High gain: %.2f dB\n", g.high);
}

void setEqBand() {
  Serial.println("Select band 1..10:");
  while (Serial.available() == 0) {
    delay(10);
  }
  int band = Serial.parseInt();
  if (band < 1 || band > 10) {
    Serial.println("Invalid band number.");
    return;
  }

  Serial.println("Enter dB value:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float db = Serial.parseFloat();
  dsp.setEqBand(band - 1, db);
  Serial.printf("Band %d set to %.2f dB\n", band, db);
}

void setCrossover() {
  Serial.println("Enter low cutoff Hz:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float low = Serial.parseFloat();

  Serial.println("Enter mid-low Hz:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float midLow = Serial.parseFloat();

  Serial.println("Enter mid-high Hz:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float midHigh = Serial.parseFloat();

  Serial.println("Enter high cutoff Hz:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float high = Serial.parseFloat();

  dsp.setCrossover(low, midLow, midHigh, high);
  Serial.println("Crossover updated.");
}

void setGain() {
  Serial.println("Enter master gain dB:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float master = Serial.parseFloat();

  Serial.println("Enter low gain dB:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float low = Serial.parseFloat();

  Serial.println("Enter mid gain dB:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float mid = Serial.parseFloat();

  Serial.println("Enter high gain dB:");
  while (Serial.available() == 0) {
    delay(10);
  }
  float high = Serial.parseFloat();

  dsp.setGain(master, low, mid, high);
  Serial.println("Gain values updated.");
}

void processTestSignal() {
  const float sampleRate = 48000.0f;
  Serial.println("Processing test signal...");

  for (int i = 0; i < 8; ++i) {
    float sample = 0.5f * std::sin((2.0f * PI * 220.0f * i) / sampleRate);
    float processed = dsp.processSample(sample);
    float low = dsp.processChannel(processed, 0);
    float mid = dsp.processChannel(processed, 1);
    float high = dsp.processChannel(processed, 2);

    Serial.printf("Sample %d -> low=%.4f mid=%.4f high=%.4f\n", i, low, mid, high);
    delay(10);
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  dsp.setDefaultProfile();
  printMenu();
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case '0':
        printMenu();
        break;
      case '1':
        showEq();
        break;
      case '2':
        setEqBand();
        break;
      case '3':
        showCrossover();
        break;
      case '4':
        setCrossover();
        break;
      case '5':
        showGain();
        break;
      case '6':
        setGain();
        break;
      case '7':
        processTestSignal();
        break;
      case '8':
        dsp.setDefaultProfile();
        Serial.println("Default profile restored.");
        break;
      case '\n':
      case '\r':
        break;
      default:
        Serial.println("Invalid command.");
        printMenu();
        break;
    }

    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  delay(20);
}

