#pragma once

#include <array>
#include <cmath>

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
  static constexpr float kSampleRateHz = 48000.0f;
  static constexpr std::array<float, kEqBands> kEqCenterFrequenciesHz = {
      31.0f, 63.0f, 125.0f, 250.0f, 500.0f,
      1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f
  };

  AudioDSPManager();

  void setDefaultProfile();
  void setEqBand(int index, float dB);
  void setGain(float masterDb, float lowDb, float midDb, float highDb);
  void setCrossover(float lowCutHz, float midLowHz, float midHighHz, float highCutHz);

  std::array<float, kEqBands>& eqBands();
  const std::array<float, kEqBands>& eqBands() const;
  const std::array<float, kEqBands>& eqCenterFrequenciesHz() const;
  GainSettings& gains();
  const GainSettings& gains() const;
  CrossoverSettings& crossover();
  const CrossoverSettings& crossover() const;

  float processSample(float input);
  float processChannel(float input, int channelIndex);

  void printSummary();

private:
  static float dBToLinear(float dB);

  float lowPass(float input, float cutoffHz, float& state);
  float highPass(float input, float cutoffHz, float& state);
  float bandPass(float input, float lowCutHz, float highCutHz, float& lowState, float& highState);

  std::array<float, kEqBands> eq_{0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  GainSettings gain_{0.0f, 0.0f, 0.0f, 0.0f};
  CrossoverSettings crossover_{150.0f, 900.0f, 3500.0f, 12000.0f};

  float lowState_{0.0f};
  float midLowState_{0.0f};
  float midHighState_{0.0f};
  float highState_{0.0f};
};

}  // namespace laudspeaker
