#ifndef DOMAINS_DISPLAY_LCD_CPP
#define DOMAINS_DISPLAY_LCD_CPP

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

#include "../../definitions.h"

class LCD {
    const char * TAG  = "LCD";
    public:
        esp_lcd_panel_handle_t lcd_panel = NULL;

        LCD() {};

        esp_err_t setup() {
            esp_err_t ret = ESP_OK;

            /* LCD initialization */
            ESP_LOGI(TAG, "Initialize RGB panel");
            esp_lcd_rgb_panel_config_t panel_conf = {
                .clk_src = LCD_CLK_SRC_DEFAULT,
                .timings = {
                    .pclk_hz = LCD_PIXEL_CLOCK_HZ,
                    .h_res = LCD_H_RES,
                    .v_res = LCD_V_RES,
                    // The following parameters should refer to LCD spec
                    .hsync_pulse_width = 4,
                    .hsync_back_porch = 8,
                    .hsync_front_porch = 8,
                    .vsync_pulse_width = 4,
                    .vsync_back_porch = 16,
                    .vsync_front_porch = 16,
                    .flags = {
                        .pclk_active_neg = true,
                    },
                },
                .data_width = 16, // RGB565 in parallel mode, thus 16bit in width
                .num_fbs = LCD_RGB_BUFFER_NUMS,
                .psram_trans_align = 64,
                .hsync_gpio_num = PIN_NUM_HSYNC,
                .vsync_gpio_num = PIN_NUM_VSYNC,
                .de_gpio_num = PIN_NUM_DE,
                .pclk_gpio_num = PIN_NUM_PCLK,
                .disp_gpio_num = PIN_NUM_DISP_EN,
                .data_gpio_nums = {
                    PIN_NUM_DATA0,
                    PIN_NUM_DATA1,
                    PIN_NUM_DATA2,
                    PIN_NUM_DATA3,
                    PIN_NUM_DATA4,
                    PIN_NUM_DATA5,
                    PIN_NUM_DATA6,
                    PIN_NUM_DATA7,
                    PIN_NUM_DATA8,
                    PIN_NUM_DATA9,
                    PIN_NUM_DATA10,
                    PIN_NUM_DATA11,
                    PIN_NUM_DATA12,
                    PIN_NUM_DATA13,
                    PIN_NUM_DATA14,
                    PIN_NUM_DATA15,
                },
                // .timings = LCD_PANEL_35HZ_RGB_TIMING(),
                .flags = {
                    .fb_in_psram = true, // allocate frame buffer in PSRAM
                },
            };

            ESP_GOTO_ON_ERROR(esp_lcd_new_rgb_panel(&panel_conf, &lcd_panel), err, TAG, "RGB init failed");
            ESP_GOTO_ON_ERROR(esp_lcd_panel_init(lcd_panel), err, TAG, "LCD init failed");

            return ret;

        err:
            if (lcd_panel)
            {
                esp_lcd_panel_del(lcd_panel);
            }
            return ret;
        };
};


#endif