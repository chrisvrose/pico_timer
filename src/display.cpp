#include "ssd1306.h"
#include "textRenderer/8x8_font.h"
#include "textRenderer/TextRenderer.h"
#include <cstdint>
#include <cstdio>
#include <display.hh>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <hardware/structs/io_bank0.h>
#include <string>



DisplayManager::DisplayManager(){
    i2c_init(i2c1, 500'000);
    printf("I2c established\n");
    gpio_set_function(14,GPIO_FUNC_I2C);
    gpio_set_function(15,GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);
    printf("I2c pullups established\n");

    sleep_ms(250);


    this->display = new pico_ssd1306::SSD1306(i2c1,0x3C,pico_ssd1306::Size::W128xH64);
    this->display->setOrientation(0);
}

void DisplayManager::drawText(std::string text, uint8_t x_offset,uint8_t y_offset){
    pico_ssd1306::drawText(this->display, font_8x8,text.c_str(),x_offset,y_offset);

}

void DisplayManager::drawTextWrapped(std::string text,uint8_t x_offset, uint8_t y_offset){
    char buffer[17] = {0};
    const uint8_t MAX_ROWS=4;
    const uint8_t MAX_NUM_CHARS_IN_LINE=128/8;

    uint16_t i=0;
    uint8_t row=0;
    while(row<=MAX_ROWS && i<text.length()){
        for(int j=0;j<text.length()&& j<MAX_NUM_CHARS_IN_LINE-x_offset/8;j++){
            buffer[j] = text[i+j];

        }
        i+=16;row+=1;
        pico_ssd1306::drawText(this->display, font_8x8, buffer, x_offset,y_offset+row*8);
    }




}

void DisplayManager::clear(bool commit){
    // TODO fill
    this->display->clear();
    if(commit){
        this->display->sendBuffer();
    }
}

void DisplayManager::commit(){
    this->display->sendBuffer();
}

DisplayManager::~DisplayManager(){
    delete this->display;
}
