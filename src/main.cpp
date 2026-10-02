#include "LcdMenu.h"

namespace laudspeaker {

LcdMenu::LcdMenu() = default;

void LcdMenu::init() {
  tft_.init();
  tft_.setRotation(1);
  tft_.fillScreen(TFT_BLACK);
  tft_.setTextColor(TFT_WHITE, TFT_BLACK);
  tft_.setTextSize(2);
  refresh();
}

void LcdMenu::setEqValues(const std::array<float, 10>& eq) {
  eqValues_ = eq;
}

void LcdMenu::setGainValues(const GainSettings& gain) {
  gain_ = gain;
}

void LcdMenu::setCrossoverValues(const CrossoverSettings& xover) {
  crossover_ = xover;
}

void LcdMenu::refresh() {
  tft_.fillScreen(TFT_BLACK);
  switch (activePage_) {
    case Page::Main:
      drawMainMenu();
      break;
    case Page::Eq:
      drawEqMenu();
      break;
    case Page::Crossover:
      drawCrossoverMenu();
      break;
    case Page::Gain:
      drawGainMenu();
      break;
  }
  drawFooter();
}

void LcdMenu::next() {
  switch (activePage_) {
    case Page::Main:
      selectedIndex_ = (selectedIndex_ + 1) % 4;
      break;
    case Page::Eq:
      selectedIndex_ = (selectedIndex_ + 1) % 10;
      break;
    case Page::Crossover:
      selectedIndex_ = (selectedIndex_ + 1) % 4;
      break;
    case Page::Gain:
      selectedIndex_ = (selectedIndex_ + 1) % 4;
      break;
  }
  refresh();
}

void LcdMenu::previous() {
  switch (activePage_) {
    case Page::Main:
      selectedIndex_ = (selectedIndex_ + 3) % 4;
      break;
    case Page::Eq:
      selectedIndex_ = (selectedIndex_ + 9) % 10;
      break;
    case Page::Crossover:
      selectedIndex_ = (selectedIndex_ + 3) % 4;
      break;
    case Page::Gain:
      selectedIndex_ = (selectedIndex_ + 3) % 4;
      break;
  }
  refresh();
}

void LcdMenu::select() {
  switch (activePage_) {
    case Page::Main:
      if (selectedIndex_ == 0) activePage_ = Page::Eq;
      else if (selectedIndex_ == 1) activePage_ = Page::Crossover;
      else if (selectedIndex_ == 2) activePage_ = Page::Gain;
      else activePage_ = Page::Main;
      selectedIndex_ = 0;
      refresh();
      break;
    case Page::Eq:
      activePage_ = Page::Main;
      selectedIndex_ = 0;
      refresh();
      break;
    case Page::Crossover:
      activePage_ = Page::Main;
      selectedIndex_ = 1;
      refresh();
      break;
    case Page::Gain:
      activePage_ = Page::Main;
      selectedIndex_ = 2;
      refresh();
      break;
  }
}

LcdMenu::Page LcdMenu::currentPage() const {
  return activePage_;
}

void LcdMenu::drawHeader(const char* title) {
  tft_.fillRect(0, 0, 320, 30, TFT_NAVY);
  tft_.setTextColor(TFT_WHITE, TFT_NAVY);
  tft_.setTextSize(2);
  tft_.drawString(title, 10, 8);
}

void LcdMenu::drawFooter() {
  tft_.fillRect(0, 220, 320, 20, TFT_DARKGREY);
  tft_.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft_.setTextSize(1);
  const char* footer = (activePage_ == Page::Main) ? "UP/DOWN: Nav  SELECT: Enter" : "SELECT: Back";
  tft_.drawString(footer, 10, 224);
}

void LcdMenu::drawMainMenu() {
  drawHeader("Main Menu");
  const char* items[] = {"10-Band EQ", "3-way XOVER", "Gain", "Exit"};
  const int yStart = 50;
  const int boxWidth = 120;
  const int boxHeight = 50;

  for (int i = 0; i < 4; ++i) {
    const int x = 20 + (i % 2) * 150;
    const int y = yStart + (i / 2) * 80;
    const bool selected = (selectedIndex_ == i);
    tft_.fillRoundRect(x, y, boxWidth, boxHeight, 8, selected ? TFT_BLUE : TFT_DARKGREY);
    tft_.drawRoundRect(x, y, boxWidth, boxHeight, 8, TFT_WHITE);
    tft_.setTextColor(TFT_WHITE, selected ? TFT_BLUE : TFT_DARKGREY);
    tft_.setTextSize(2);
    tft_.drawString(items[i], x + 14, y + 15);
  }
}

void LcdMenu::drawEqMenu() {
  drawHeader("10-Band EQ");
  tft_.setTextSize(1);
  for (int i = 0; i < 10; ++i) {
    const int y = 40 + i * 16;
    const bool selected = (selectedIndex_ == i);
    const int hue = selected ? TFT_YELLOW : TFT_WHITE;
    tft_.setTextColor(hue, TFT_BLACK);
    char buff[32];
    snprintf(buff, sizeof(buff), "B%d: %.1f dB", i + 1, eqValues_[i]);
    tft_.drawString(buff, 20, y);
  }
}

void LcdMenu::drawCrossoverMenu() {
  drawHeader("3-Way XOVER");
  tft_.setTextSize(2);
  tft_.setTextColor(TFT_WHITE, TFT_BLACK);
  char buff[32];
  snprintf(buff, sizeof(buff), "Low: %.0f Hz", crossover_.lowCutHz);
  tft_.drawString(buff, 20, 50);
  snprintf(buff, sizeof(buff), "MidL: %.0f Hz", crossover_.midLowHz);
  tft_.drawString(buff, 20, 90);
  snprintf(buff, sizeof(buff), "MidH: %.0f Hz", crossover_.midHighHz);
  tft_.drawString(buff, 20, 130);
  snprintf(buff, sizeof(buff), "High: %.0f Hz", crossover_.highCutHz);
  tft_.drawString(buff, 20, 170);
}

void LcdMenu::drawGainMenu() {
  drawHeader("Gain Control");
  tft_.setTextSize(2);
  tft_.setTextColor(TFT_WHITE, TFT_BLACK);
  char buff[32];
  snprintf(buff, sizeof(buff), "Master: %.1f dB", gain_.master);
  tft_.drawString(buff, 20, 50);
  snprintf(buff, sizeof(buff), "Low: %.1f dB", gain_.low);
  tft_.drawString(buff, 20, 90);
  snprintf(buff, sizeof(buff), "Mid: %.1f dB", gain_.mid);
  tft_.drawString(buff, 20, 130);
  snprintf(buff, sizeof(buff), "High: %.1f dB", gain_.high);
  tft_.drawString(buff, 20, 170);
}

}  // namespace laudspeaker
