#ifndef DOMAINS_DISPLAY_TFT_CPP
#define DOMAINS_DISPLAY_TFT_CPP

#include "driver/i2c.h"
#include <esp_lcd_touch_gt911.h>

#include "../../io/i2c/i2c.cpp"
#include "../../definitions.h"


class TFT {
    const char * TAG  = "TFT";
    I2CMaster *i2cMaster = nullptr;
    esp_lcd_panel_io_handle_t tp_io_handle = nullptr;

    public:
        esp_lcd_touch_handle_t touch_handle = NULL;
        TFT(I2CMaster *i2cMaster) : i2cMaster(i2cMaster) {
            assert(i2cMaster);
        }

        esp_err_t setup() {
            esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
            tp_io_config.scl_speed_hz = i2cMaster->frequency;

            /* Initialize touch HW */
            const esp_lcd_touch_config_t tp_cfg = {
                .x_max = LCD_H_RES,
                .y_max = LCD_V_RES,
                .rst_gpio_num = GPIO_NUM_NC,
                .int_gpio_num = GPIO_NUM_NC,
                .levels = {
                    .reset = 0,
                    .interrupt = 0,
                },
                .flags = {
                    .swap_xy = 0,
                    .mirror_x = 0,
                    .mirror_y = 0,
                }
            };

            tp_io_handle = NULL;

            // Use new i2cmaster driver
            esp_lcd_new_panel_io_i2c_v2(i2cMaster->bus_handle,  &tp_io_config, &tp_io_handle);
            return esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);
        };
};


#endif