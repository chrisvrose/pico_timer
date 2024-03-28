#include <stdio.h>
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico-ssd1306/ssd1306.h"
#include "pico-ssd1306/textRenderer/TextRenderer.h"
#include "pico.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "tusb.h"

int main() {
    const uint led_pin = 25;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();
    rtc_init();

    // printf("waiting for usb host");
    // while (!tud_cdc_connected()) {
    //     printf(".");
    //     sleep_ms(500);
    // }
    // printf("\nusb host detected!\n");

    i2c_init(i2c_default, 100 * 1000);

    // i2c_init(i2c0, 1000000); //Use i2c port with baud rate of 1Mhz
    // //Set pins for I2C operation
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // //Create a new display object
    pico_ssd1306::SSD1306 display =
        pico_ssd1306::SSD1306(i2c_default, 0x3C, pico_ssd1306::Size::W128xH64);
    datetime_t current_datetime = {.year = 2020,
                                   .month = 06,
                                   .day = 05,
                                   .dotw = 5,  // 0 is Sunday, so 5 is Friday
                                   .hour = 15,
                                   .min = 45,
                                   .sec = 00};
    rtc_set_datetime(&current_datetime);
    // create a vertical line on x: 64 y:0-63
    for (int y = 0; y < 64; y++) {
        display.setPixel(64, y);
    }
    char a[6] = "Hello";
    pico_ssd1306::drawText(&display, font_12x16, a, 0, 0);
    display.sendBuffer();

    char s[100] = {0};
    sleep_ms(2000);
    // Loop forever
    while (true) {
        display.clear();
        bool stat = rtc_get_datetime(&current_datetime);
        if(stat)
        sprintf(s, "%02d:%02d:%02d", current_datetime.hour,
                current_datetime.min, current_datetime.sec);
        else
        sprintf(s,"E:RTC not running");
        pico_ssd1306::drawText(&display, font_12x16, s, 20, 10);
        display.sendBuffer();
        // Blink LED
        printf("Tick!\r\n");
        sleep_ms(1000);
    }
}
