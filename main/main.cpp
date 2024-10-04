/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include <esp_lcd_touch_gt911.h>
#include "esp_lvgl_port.h"

#include "domains/definitions.h"
#include "domains/io/i2c/i2c.cpp"
#include "domains/state/state.cpp"

#include "domains/display/Display.cpp"
#include "domains/display/screens/BaseScreenElements.cpp"
#include "domains/display/screens/BaseScreen.cpp"
#include "domains/display/screens/BootScreen.cpp"

#include <stdio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



static const char *TAG = "EXAMPLE";

// LVGL image declare
LV_IMG_DECLARE(esp_logo)



/**
 * Print the memory usage periodically
 * @param param
 */
void lvgl_memory_monitor( void * pvParameters ) {

    while (1)
    {
        lv_mem_monitor_t mon;
        lv_mem_monitor(&mon);
        ESP_LOGI("LOGGER", "used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n",
        (int)mon.total_size - mon.free_size, mon.used_pct, mon.frag_pct,
        (int)mon.free_biggest_size);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void log_memory_usage()
{
    size_t free_heap_size = esp_get_free_heap_size();
    size_t min_free_heap_size = esp_get_minimum_free_heap_size();
    size_t free_psram_size = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);

    ESP_LOGI("Memory Usage", "Free Heap Size: %d bytes", free_heap_size);
    ESP_LOGI("Memory Usage", "Minimum Free Heap Size: %d bytes", min_free_heap_size);
    ESP_LOGI("Memory Usage", "Free PSRAM Size: %d bytes", free_psram_size);
}

void log_task_stack_usage(TaskHandle_t task)
{
    UBaseType_t high_water_mark = uxTaskGetStackHighWaterMark(task);
    ESP_LOGI("Task Stack", "High Water Mark: %d words", high_water_mark);
}


I2CMaster *i2cMaster = nullptr;
State *state = nullptr;

Display *display = nullptr;

BaseScreenElements *baseScreenElements = nullptr;
BootScreen *bootScreen = nullptr;


static void _app_button_cb(lv_event_t *e)
{
    lv_disp_rotation_t rotation = lv_disp_get_rotation(display->lvgl->lvgl_display);
    rotation = static_cast<lv_disp_rotation_t>(rotation + 1);
    if (rotation > LV_DISPLAY_ROTATION_270)
    {
        rotation = LV_DISPLAY_ROTATION_0;
    }

    /* LCD HW rotation */
    lv_disp_set_rotation(display->lvgl->lvgl_display, rotation);
}

static void app_main_display(void)
{
    lv_obj_t *scr = lv_scr_act();

    /* Your LVGL objects code here .... */

    /* Create image */
    lv_obj_t *img_logo = lv_img_create(scr);
    lv_img_set_src(img_logo, &esp_logo);
    lv_obj_align(img_logo, LV_ALIGN_TOP_MID, 0, 20);

    /* Label */
    lv_obj_t *label = lv_label_create(scr);
    lv_obj_set_width(label, LCD_H_RES);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

    lv_label_set_text(label, LV_SYMBOL_BELL "V9 Hello world Espressif and LVGL " LV_SYMBOL_BELL "\n " LV_SYMBOL_WARNING " For simplier initialization, use BSP " LV_SYMBOL_WARNING);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 20);

    /* Button */
    lv_obj_t *btn = lv_btn_create(scr);
    label = lv_label_create(btn);
    lv_label_set_text_static(label, "Rotate screen");
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_add_event_cb(btn, _app_button_cb, LV_EVENT_CLICKED, NULL);
}

extern "C" void app_main(void)
{
    i2cMaster = new I2CMaster(0,I2C_MASTER_SDA_IO,I2C_MASTER_SCL_IO,I2C_MASTER_FREQ_HZ, I2C_MASTER_TIMEOUT_MS);
    i2cMaster->begin();
    
    display = new Display(i2cMaster);
    
    // Initialise LCD, TFT and LVGL
    display->setup();

    // /* LCD HW initialization */
    // ESP_ERROR_CHECK(app_lcd_init());

    // /* Touch initialization */
    // ESP_ERROR_CHECK(app_touch_init());

    // /* LVGL initialization */
    // ESP_ERROR_CHECK(app_lvgl_init());


    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);


    /* Show LVGL objects */
    display->lvgl_mutex_lock(0);
    app_main_display();
    display->lvgl_mutex_unlock();
    vTaskDelay(pdMS_TO_TICKS(5000));

    display->lvgl_mutex_lock(0);
    state = new State();

    baseScreenElements = new BaseScreenElements();
    bootScreen = new BootScreen(baseScreenElements, state);


    display->lvgl_mutex_unlock();

    display->lvgl_mutex_lock(0);
    bootScreen->load();
    display->lvgl_mutex_unlock();
    
}
