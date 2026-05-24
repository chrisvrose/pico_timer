#pragma once
#include "ssd1306.h"
#include "textRenderer/8x8_font.h"
#include <cstdint>
#include <string>


const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;

#define FONT_USED (font_8x8)
const uint8_t FONT_WIDTH = 8;
const uint8_t FONT_HEIGHT = 8;

const uint8_t MAX_ROWS = SCREEN_HEIGHT / FONT_HEIGHT;
// const uint8_t MAX_ROWS=4;

const uint8_t MAX_NUM_CHARS_IN_LINE = SCREEN_WIDTH / FONT_WIDTH;

constexpr uint32_t row_to_pixels_offset_y(uint32_t row_num,uint32_t offset = 0){
    return offset + FONT_HEIGHT * row_num;
}



class DisplayManager{
    private:
    pico_ssd1306::SSD1306* display;
    public:
    DisplayManager();
    ~DisplayManager();

    void drawText(std::string text, uint8_t x_offset=0,uint8_t y_offset=0);
    void drawTextWrapped(const std::string& text, uint8_t x_offset=0,uint8_t y_offset=0);
    void commit();
    void clear(bool commit=true);
};
