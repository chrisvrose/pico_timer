#pragma once
#include "ssd1306.h"
#include <cstdint>
#include <string>
class DisplayManager{
    private:
    pico_ssd1306::SSD1306* display;
    public:
    DisplayManager();
    ~DisplayManager();

    void drawText(std::string text, uint8_t x_offset=0,uint8_t y_offset=0);
    void drawTextWrapped(std::string text, uint8_t x_offset=0,uint8_t y_offset=0);
    void commit();
    void clear(bool commit=true);
};
