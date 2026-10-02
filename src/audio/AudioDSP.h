#pragma once

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <cmath>
#include <array>
#include <iostream>
#endif

namespace laudspeaker {

struct GainSettings {
  float master = 0.0f;
  float low = 0.0f;
  float mid = 0.0f;
  float high = 0.0f;
};

struct CrossoverSettings {
  float lowCutHz = 150.0f;
  float midLowHz = 900.0f;
  float midHighHz = 3500.0f;
  float highCutHz = 12000.0f;
};

class AudioDSPManager {
public:
  static constexpr size_t kEqBands = 10;

  AudioDSPManager();

  void setDefaultProfile();
  void setEqBand(int index, float dB);
  void setGain(float masterDb, float lowDb, float midDb, float highDb);
  void setCrossover(float lowCutHz, float midLowHz, float midHighHz, float highCutHz);

  std::array<float, kEqBands>& eqBands();
  const std::array<float, kEqBands>& eqBands() const;
  GainSettings& gains();
  const GainSettings& gains() const;
  CrossoverSettings& crossover();
  const CrossoverSettings& crossover() const;

  float processSample(float input);
  float processChannel(float input, int channelIndex);

  void printSummary();

private:
  static float dBToLinear(float dB);
  static float applyEqBand(float sample, float gainDb);

  float onePoleLowPass(float input, float cutoffHz, float sampleRate, float& state);
  float onePoleHighPass(float input, float cutoffHz, float sampleRate, float& state);

  std::array<float, kEqBands> eq_{0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  GainSettings gain_{0.0f, 0.0f, 0.0f, 0.0f};
  CrossoverSettings crossover_{150.0f, 900.0f, 3500.0f, 12000.0f};

  float lowState_{0.0f};
  float midState_{0.0f};
  float highState_{0.0f};
};

}  // namespace laudspeaker

