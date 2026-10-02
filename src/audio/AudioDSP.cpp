#include "AudioDSP.h"

namespace laudspeaker {

namespace {
constexpr float kPi = 3.14159265358979323846f;
}

AudioDSPManager::AudioDSPManager() {
  setDefaultProfile();
}

void AudioDSPManager::setDefaultProfile() {
  eq_.fill(0.0f);
  eq_[1] = 1.5f;
  eq_[2] = 2.5f;
  eq_[4] = 3.0f;
  eq_[5] = 1.0f;
  eq_[6] = -1.5f;
  eq_[7] = 1.5f;
  eq_[8] = 2.0f;

  gain_.master = 0.0f;
  gain_.low = 0.0f;
  gain_.mid = 0.0f;
  gain_.high = 0.0f;

  crossover_.lowCutHz = 150.0f;
  crossover_.midLowHz = 900.0f;
  crossover_.midHighHz = 3500.0f;
  crossover_.highCutHz = 12000.0f;

  lowState_ = 0.0f;
  midLowState_ = 0.0f;
  midHighState_ = 0.0f;
  highState_ = 0.0f;
}

void AudioDSPManager::setEqBand(int index, float dB) {
  if (index < 0 || index >= static_cast<int>(eq_.size())) {
    return;
  }
  eq_[index] = dB;
}

void AudioDSPManager::setGain(float masterDb, float lowDb, float midDb, float highDb) {
  gain_.master = masterDb;
  gain_.low = lowDb;
  gain_.mid = midDb;
  gain_.high = highDb;
}

void AudioDSPManager::setCrossover(float lowCutHz, float midLowHz, float midHighHz, float highCutHz) {
  crossover_.lowCutHz = lowCutHz;
  crossover_.midLowHz = midLowHz;
  crossover_.midHighHz = midHighHz;
  crossover_.highCutHz = highCutHz;
}

std::array<float, AudioDSPManager::kEqBands>& AudioDSPManager::eqBands() {
  return eq_;
}

const std::array<float, AudioDSPManager::kEqBands>& AudioDSPManager::eqBands() const {
  return eq_;
}

const std::array<float, AudioDSPManager::kEqBands>& AudioDSPManager::eqCenterFrequenciesHz() const {
  return kEqCenterFrequenciesHz;
}

GainSettings& AudioDSPManager::gains() {
  return gain_;
}

const GainSettings& AudioDSPManager::gains() const {
  return gain_;
}

CrossoverSettings& AudioDSPManager::crossover() {
  return crossover_;
}

const CrossoverSettings& AudioDSPManager::crossover() const {
  return crossover_;
}

float AudioDSPManager::dBToLinear(float dB) {
  return std::pow(10.0f, dB / 20.0f);
}

float AudioDSPManager::lowPass(float input, float cutoffHz, float& state) {
  if (cutoffHz <= 0.0f) {
    return input;
  }

  const float rc = 1.0f / (2.0f * kPi * cutoffHz);
  const float dt = 1.0f / kSampleRateHz;
  const float alpha = dt / (rc + dt);
  const float output = alpha * input + (1.0f - alpha) * state;
  state = output;
  return output;
}

float AudioDSPManager::highPass(float input, float cutoffHz, float& state) {
  if (cutoffHz <= 0.0f) {
    return input;
  }

  const float low = lowPass(input, cutoffHz, state);
  return input - low;
}

float AudioDSPManager::bandPass(float input, float lowCutHz, float highCutHz, float& lowState, float& highState) {
  const float lowPassOutput = lowPass(input, highCutHz, lowState);
  const float highPassOutput = highPass(input, lowCutHz, highState);
  return lowPassOutput - highPassOutput;
}

float AudioDSPManager::processSample(float input) {
  float sample = input * dBToLinear(gain_.master);
  for (size_t i = 0; i < eq_.size(); ++i) {
    sample = sample * dBToLinear(eq_[i]);
  }
  return sample;
}

float AudioDSPManager::processChannel(float input, int channelIndex) {
  switch (channelIndex) {
    case 0:
      return lowPass(input, crossover_.lowCutHz, lowState_) * dBToLinear(gain_.low);
    case 1:
      return bandPass(input, crossover_.midLowHz, crossover_.midHighHz, midLowState_, midHighState_) *
             dBToLinear(gain_.mid);
    case 2:
      return highPass(input, crossover_.midHighHz, highState_) * dBToLinear(gain_.high);
    default:
      return input;
  }
}

void AudioDSPManager::printSummary() {
#if defined(ARDUINO)
  Serial.println("\nDSP summary:");
  Serial.printf("Master gain: %.2f dB\n", gain_.master);
  Serial.printf("Low gain: %.2f dB\n", gain_.low);
  Serial.printf("Mid gain: %.2f dB\n", gain_.mid);
  Serial.printf("High gain: %.2f dB\n", gain_.high);
  Serial.printf("Low cutoff: %.1f Hz\n", crossover_.lowCutHz);
  Serial.printf("Mid-low: %.1f Hz\n", crossover_.midLowHz);
  Serial.printf("Mid-high: %.1f Hz\n", crossover_.midHighHz);
  Serial.printf("High cutoff: %.1f Hz\n", crossover_.highCutHz);
#else
  std::cout << "\nDSP summary:" << std::endl;
  std::cout << "Master gain: " << gain_.master << " dB" << std::endl;
  std::cout << "Low gain: " << gain_.low << " dB" << std::endl;
  std::cout << "Mid gain: " << gain_.mid << " dB" << std::endl;
  std::cout << "High gain: " << gain_.high << " dB" << std::endl;
  std::cout << "Low cutoff: " << crossover_.lowCutHz << " Hz" << std::endl;
  std::cout << "Mid-low: " << crossover_.midLowHz << " Hz" << std::endl;
  std::cout << "Mid-high: " << crossover_.midHighHz << " Hz" << std::endl;
  std::cout << "High cutoff: " << crossover_.highCutHz << " Hz" << std::endl;
#endif
}

}  // namespace laudspeaker
