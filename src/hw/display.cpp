#include "ssd1306.h"
#include "textRenderer/TextRenderer.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <hw/display.hh>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include "textRenderer/8x8_font.h"
#include <memory>
#include <string>


#define I2C_DISPLAY_PORT i2c1
#define I2C_DISPLAY_ADDRESS 0x3C

DisplayManager::DisplayManager(){
    i2c_init(i2c1, 50'000);
    printf("# I2c established\n");
    gpio_set_function(14,GPIO_FUNC_I2C);
    gpio_set_function(15,GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
    printf("# I2c pullups established\n");

    sleep_ms(250);


    this->display = std::make_unique<pico_ssd1306::SSD1306>(I2C_DISPLAY_PORT,I2C_DISPLAY_ADDRESS,pico_ssd1306::Size::W128xH64);
    this->display->setOrientation(0);
}

void DisplayManager::drawText(const std::string& text, uint8_t x_offset,uint8_t y_offset){
    pico_ssd1306::drawText(this->display.get(), font_8x8,text.c_str(),x_offset,y_offset);
}

void DisplayManager::drawTextWrapped(const std::string& text,uint8_t x_offset, uint8_t y_offset){
    char buffer[17] = {0};

    for(uint8_t i_string_index=0,row_number=0;row_number<MAX_ROWS && i_string_index<text.length();i_string_index+=MAX_NUM_CHARS_IN_LINE,row_number+=1){
        uint8_t copy_len = std::min(MAX_NUM_CHARS_IN_LINE,(uint8_t)(text.length()-i_string_index));
        memcpy(buffer,text.c_str()+i_string_index,copy_len);
        buffer[copy_len] = 0;

        pico_ssd1306::drawText(this->display.get(), FONT_USED, buffer, x_offset,row_to_pixels_offset_y(row_number,y_offset));
    }
}

void DisplayManager::clear(bool commit){
    this->display->clear();
    if (commit) {
      this->display->sendBuffer();
    }
}

void DisplayManager::commit(){
    this->display->sendBuffer();
}
