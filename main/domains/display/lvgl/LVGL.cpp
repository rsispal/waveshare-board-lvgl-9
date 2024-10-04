#ifndef DOMAINS_DISPLAY_LVGL_CPP
#define DOMAINS_DISPLAY_LVGL_CPP

#include "esp_lvgl_port.h"
#include "../../definitions.h"

class Lvgl {
    const char * TAG  = "Lvgl";

    public:
    /* LVGL display and touch */
    lv_indev_t *lvgl_touch_indev = NULL;
    lv_display_t *lvgl_display = NULL;

        Lvgl() {}

        esp_err_t setup(esp_lcd_panel_handle_t &lcd_panel, esp_lcd_touch_handle_t &touch_handle) {
            /* Initialize LVGL */
            const lvgl_port_cfg_t lvgl_cfg = {
                .task_priority = 4,       /* LVGL task priority */
                .task_stack = 9192,       /* LVGL task stack size */
                .task_affinity = -1,      /* LVGL task pinned to core (-1 is no affinity) */
                .task_max_sleep_ms = 500, /* Maximum sleep in LVGL task */
                .timer_period_ms = 10      /* LVGL timer tick period in ms */
            };
            ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

            uint32_t buff_size = LCD_H_RES * 200 * sizeof(lv_color_t);
            ESP_LOGI(TAG, "LVGL PSRAM Buffer Size: %lu", buff_size);

            /* Add LCD screen */
            ESP_LOGD(TAG, "Add LCD screen");
            const lvgl_port_display_cfg_t disp_cfg = {
                .panel_handle = lcd_panel,
                .buffer_size = buff_size,
                .double_buffer = false,
                .hres = LCD_H_RES,
                .vres = LCD_V_RES,
                .monochrome = false,
                .rotation = {
                    .swap_xy = false,
                    .mirror_x = false,
                    .mirror_y = false,
                },
                .color_format = LV_COLOR_FORMAT_RGB565,
                .flags = {
                    .buff_dma = false,
                    .buff_spiram = true,
                    .sw_rotate = false,
                    .swap_bytes = false,
                    .full_refresh = false,
                    .direct_mode = false,
                },
            };
            const lvgl_port_display_rgb_cfg_t rgb_cfg = {
                .flags = {
                    .bb_mode = false,
                    .avoid_tearing = false,
                },
            };
            lvgl_display = lvgl_port_add_disp_rgb(&disp_cfg, &rgb_cfg);

            /* Add touch input (for selected screen) */
            const lvgl_port_touch_cfg_t touch_cfg = {
                .disp = lvgl_display,
                .handle = touch_handle,
            };
            lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

            return ESP_OK;
        };


        bool mutex_lock(uint32_t timeout_ms) {
            return lvgl_port_lock(timeout_ms);
        }
        
        void mutex_unlock() {
            lvgl_port_unlock();
        }

};

#endif