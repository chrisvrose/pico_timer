#pragma once
#include "ssd1306.h"
#include <cstdint>
#include <memory>
#include <string>


constexpr pico_ssd1306::Size DISPLAY_SIZE_USED = pico_ssd1306::Size::W128xH64;
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

#define FONT_USED (font_8x8)
constexpr uint8_t FONT_WIDTH = 8;
constexpr uint8_t FONT_HEIGHT = 8;


constexpr uint8_t MAX_ROWS = SCREEN_HEIGHT / FONT_HEIGHT;

constexpr uint8_t MAX_NUM_CHARS_IN_LINE = SCREEN_WIDTH / FONT_WIDTH;

constexpr uint32_t row_to_pixels_offset_y(uint32_t row_num,uint32_t offset = 0){
    return offset + FONT_HEIGHT * row_num;
}



class DisplayManager{
    private:
    std::unique_ptr<pico_ssd1306::SSD1306> display;
    public:
    DisplayManager();

    void drawText(const std::string& text, uint8_t x_offset=0,uint8_t y_offset=0) const;
    void drawTextWrapped(const std::string& text, uint8_t x_offset=0,uint8_t y_offset=0) const;
    void commit() const;
    void clear(bool commit=true) const;
};
