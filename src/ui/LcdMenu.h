#pragma once

#include <array>
#include <cstdint>

#include <TFT_eSPI.h>

#include "audio/AudioDSP.h"

namespace laudspeaker {

class LcdMenu {
public:
  enum class Page : uint8_t {
    Main = 0,
    Eq = 1,
    Crossover = 2,
    Gain = 3
  };

  LcdMenu();
  ~LcdMenu() = default;

  void init();
  void setEqValues(const std::array<float, 10>& eq);
  void setGainValues(const GainSettings& gain);
  void setCrossoverValues(const CrossoverSettings& xover);
  void refresh();
  void nextItem();
  void previousItem();
  void selectItem();
  Page currentPage() const;
  int getCurrentIndex() const;

private:
  TFT_eSPI tft_;
  Page activePage_ = Page::Main;
  int selectedIndex_ = 0;
  std::array<float, 10> eqValues_{};
  GainSettings gain_{};
  CrossoverSettings crossover_{};

  void drawMainMenu();
  void drawEqMenu();
  void drawCrossoverMenu();
  void drawGainMenu();
  void drawFooter();
  void drawHeader(const char* title);
};

}  // namespace laudspeaker
