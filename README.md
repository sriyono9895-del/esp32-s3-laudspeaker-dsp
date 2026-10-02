#include <Arduino.h>
#include <cmath>

#include "audio/AudioDSP.h"
#include "ui/LcdMenu.h"

namespace {

using laudspeaker::AudioDSPManager;
using laudspeaker::LcdMenu;

AudioDSPManager dsp;
LcdMenu lcdMenu;

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
  Serial.println("8. Reset default profile");
  Serial.println("9. Show DSP summary");
  Serial.println("0. Show menu");
  Serial.println("========================================");
}

float readFloatValue() {
  while (Serial.available() == 0) {
    delay(10);
  }
  return Serial.parseFloat();
}

int readIntValue() {
  while (Serial.available() == 0) {
    delay(10);
  }
  return Serial.parseInt();
}

void updateDisplay() {
  lcdMenu.setEqValues(dsp.eqBands());
  lcdMenu.setGainValues(dsp.gains());
  lcdMenu.setCrossoverValues(dsp.crossover());
  lcdMenu.refresh();
}

void showEq() {
  Serial.println("\nCurrent EQ settings (dB):");
  const auto& freqs = dsp.eqCenterFrequenciesHz();
  for (size_t i = 0; i < dsp.eqBands().size(); ++i) {
    Serial.printf("Band %d [%5.0f Hz] : %.2f dB\n", static_cast<int>(i + 1), freqs[i], dsp.eqBands()[i]);
  }
  updateDisplay();
}

void showCrossover() {
  Serial.println("\nCurrent crossover settings:");
  Serial.printf("Low cutoff: %.1f Hz\n", dsp.crossover().lowCutHz);
  Serial.printf("Mid low: %.1f Hz\n", dsp.crossover().midLowHz);
  Serial.printf("Mid high: %.1f Hz\n", dsp.crossover().midHighHz);
  Serial.printf("High cutoff: %.1f Hz\n", dsp.crossover().highCutHz);
  updateDisplay();
}

void showGain() {
  const auto g = dsp.gains();
  Serial.println("\nCurrent gain settings:");
  Serial.printf("Master gain: %.2f dB\n", g.master);
  Serial.printf("Low gain: %.2f dB\n", g.low);
  Serial.printf("Mid gain: %.2f dB\n", g.mid);
  Serial.printf("High gain: %.2f dB\n", g.high);
  updateDisplay();
}

void setEqBand() {
  Serial.println("Select band 1..10:");
  const int band = readIntValue();
  if (band < 1 || band > 10) {
    Serial.println("Invalid band number.");
    return;
  }

  Serial.println("Enter dB value:");
  const float db = readFloatValue();
  dsp.setEqBand(band - 1, db);
  Serial.printf("Band %d set to %.2f dB\n", band, db);
  updateDisplay();
}

void setCrossover() {
  Serial.println("Enter low cutoff Hz:");
  const float low = readFloatValue();

  Serial.println("Enter mid-low Hz:");
  const float midLow = readFloatValue();

  Serial.println("Enter mid-high Hz:");
  const float midHigh = readFloatValue();

  Serial.println("Enter high cutoff Hz:");
  const float high = readFloatValue();

  dsp.setCrossover(low, midLow, midHigh, high);
  Serial.println("Crossover updated.");
  updateDisplay();
}

void setGain() {
  Serial.println("Enter master gain dB:");
  const float master = readFloatValue();

  Serial.println("Enter low gain dB:");
  const float low = readFloatValue();

  Serial.println("Enter mid gain dB:");
  const float mid = readFloatValue();

  Serial.println("Enter high gain dB:");
  const float high = readFloatValue();

  dsp.setGain(master, low, mid, high);
  Serial.println("Gain values updated.");
  updateDisplay();
}

void processTestSignal() {
  Serial.println("Processing test signal...");

  for (int i = 0; i < 8; ++i) {
    const float sample = 0.4f * std::sin((2.0f * static_cast<float>(M_PI) * 220.0f * i) / 48000.0f);
    const float processed = dsp.processSample(sample);
    const float low = dsp.processChannel(processed, 0);
    const float mid = dsp.processChannel(processed, 1);
    const float high = dsp.processChannel(processed, 2);

    Serial.printf("Sample %d -> low=%.4f mid=%.4f high=%.4f\n", i, low, mid, high);
    delay(20);
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  dsp.setDefaultProfile();
  lcdMenu.init();
  updateDisplay();
  printMenu();
}

void loop() {
  if (Serial.available() > 0) {
    const char command = Serial.read();

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
        updateDisplay();
        break;
      case '9':
        dsp.printSummary();
        break;
      case 'u':
        lcdMenu.next();
        break;
      case 'd':
        lcdMenu.previous();
        break;
      case 'e':
        lcdMenu.select();
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
