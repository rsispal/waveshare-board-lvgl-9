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

#include "domains/display/screens/BaseScreen.cpp"
#include "domains/display/screens/BootScreen.cpp"
#include "domains/display/screens/MainScreen.cpp"
#include "domains/display/screens/SomeOtherScreen.cpp"

#include <stdio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


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

void memory_monitor_task(void *pvParameter)
{
    while (1)
    {
        log_memory_usage();
        log_task_stack_usage(xTaskGetCurrentTaskHandle());
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void lv_tick_task(void *pvParameter)
{
    while (1)
    {
        lv_tick_inc(5);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

bool isBooted = true;
void boot_task(void *pvParameter)
{
    while (1)
    {
        if (!isBooted)
        {

            lvgl_port_lock(0);
            // bootScreen->load();
            lvgl_port_unlock();
            isBooted = true;
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 1 second
    }
}

void status_task(void *pvParameter)
{
    while (1)
    {

        lvgl_port_lock(0);
        // statusScreen->load();
        lvgl_port_unlock();

        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 1 second
    }
}


#define LCD_PANEL_35HZ_RGB_TIMING()  \
    {                                \
        .pclk_hz = 18 * 1000 * 1000, \
        .h_res = LCD_H_RES,          \
        .v_res = LCD_V_RES,          \
        .hsync_pulse_width = 40,     \
        .hsync_back_porch = 40,      \
        .hsync_front_porch = 48,     \
        .vsync_pulse_width = 23,     \
        .vsync_back_porch = 32,      \
        .vsync_front_porch = 13,     \
        .flags = {                   \
            .pclk_active_neg = true, \
        },                           \
    }

static const char *TAG = "EXAMPLE";

// LVGL image declare
LV_IMG_DECLARE(esp_logo)

/* LCD IO and panel */
static esp_lcd_panel_handle_t lcd_panel = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;

/* LVGL display and touch */
static lv_display_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

static esp_err_t app_lcd_init(void)
{
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
}



I2CMaster *i2cMaster = nullptr;
i2c_master_dev_handle_t gt911Slave;

void initialiseI2CMaster() {
   i2cMaster = new I2CMaster(0,I2C_MASTER_SDA_IO,I2C_MASTER_SCL_IO,I2C_MASTER_FREQ_HZ, I2C_MASTER_TIMEOUT_MS);
   i2cMaster->begin();
}



static esp_err_t app_touch_init(void)
{
    // /* Initilize I2C */
    // const i2c_config_t i2c_conf = {
    //     .mode = I2C_MODE_MASTER,
    //     .sda_io_num = I2C_MASTER_SDA_IO,
    //     .scl_io_num = I2C_MASTER_SCL_IO,
    //     .sda_pullup_en = GPIO_PULLUP_ENABLE,
    //     .scl_pullup_en = GPIO_PULLUP_ENABLE,
    //     .master = {
    //         .clk_speed = I2C_MASTER_FREQ_HZ,
    //     },
    // };
    // ESP_RETURN_ON_ERROR(i2c_param_config(I2C_MASTER_NUM, &i2c_conf), TAG, "I2C configuration failed");
    // ESP_RETURN_ON_ERROR(i2c_driver_install(I2C_MASTER_NUM, i2c_conf.mode, 0, 0, 0), TAG, "I2C initialization failed");

    // /* Initialize touch HW */
    // const esp_lcd_touch_config_t tp_cfg = {
    //     .x_max = LCD_H_RES,
    //     .y_max = LCD_V_RES,
    //     .rst_gpio_num = GPIO_NUM_NC,
    //     .int_gpio_num = GPIO_NUM_NC,
    //     .levels = {
    //         .reset = 0,
    //         .interrupt = 0,
    //     },
    //     .flags = {
    //         .swap_xy = 0,
    //         .mirror_x = 0,
    //         .mirror_y = 0,
    //     },
    // };
    // esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    // const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
    // esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)I2C_MASTER_NUM, &tp_io_config, &tp_io_handle);
    // return esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);


    initialiseI2CMaster();
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
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;

    esp_lcd_new_panel_io_i2c_v2(i2cMaster->bus_handle,  &tp_io_config, &tp_io_handle);
    return esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);
}

static esp_err_t app_lvgl_init(void)
{
    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,       /* LVGL task priority */
        .task_stack = 6144,       /* LVGL task stack size */
        .task_affinity = -1,      /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500, /* Maximum sleep in LVGL task */
        .timer_period_ms = 5      /* LVGL timer tick period in ms */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    uint32_t buff_size = LCD_H_RES * 100 * sizeof(lv_color_t);
    ESP_LOGI(TAG, "LVGL PSRAM Buffer Size: %lu", buff_size);

    #if LCD_LVGL_FULL_REFRESH || LCD_LVGL_DIRECT_MODE
        buff_size = LCD_H_RES * LCD_V_RES;
    #endif

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
            .buff_dma = true,
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
    lvgl_disp = lvgl_port_add_disp_rgb(&disp_cfg, &rgb_cfg);

    /* Add touch input (for selected screen) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

    return ESP_OK;
}

static void _app_button_cb(lv_event_t *e)
{
    lv_disp_rotation_t rotation = lv_disp_get_rotation(lvgl_disp);
    rotation = static_cast<lv_disp_rotation_t>(rotation + 1);
    if (rotation > LV_DISPLAY_ROTATION_270)
    {
        rotation = LV_DISPLAY_ROTATION_0;
    }

    /* LCD HW rotation */
    lv_disp_set_rotation(lvgl_disp, rotation);
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


BaseScreen *baseScreen = nullptr;
BootScreen *bootScreen = nullptr;
MainScreen *mainScreen = nullptr;
SomeOtherScreen *someOtherScreen = nullptr;

extern "C" void app_main(void)
{


    /* LCD HW initialization */
    ESP_ERROR_CHECK(app_lcd_init());

    /* Touch initialization */
    ESP_ERROR_CHECK(app_touch_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);


    /* Show LVGL objects */
    lvgl_port_lock(0);
    app_main_display();
    lvgl_port_unlock();
    vTaskDelay(pdMS_TO_TICKS(500));

    baseScreen= new BaseScreen();
    bootScreen = new BootScreen(baseScreen);
    mainScreen = new MainScreen(baseScreen);
    someOtherScreen = new SomeOtherScreen(baseScreen);
    // xTaskCreate(&lvgl_memory_monitor, "lvgl_memory_monitor", 2048,NULL,4,NULL );

while (1) {
    lvgl_port_lock(0);
    bootScreen->init();
    bootScreen->show();
    lvgl_port_unlock();

    vTaskDelay(pdMS_TO_TICKS(1000));

    lvgl_port_lock(0);
    mainScreen->init();
    mainScreen->show();
    lvgl_port_unlock();

    vTaskDelay(pdMS_TO_TICKS(1000));
        
    lvgl_port_lock(0);
    someOtherScreen->init();
    someOtherScreen->show();
    lvgl_port_unlock();

    vTaskDelay(pdMS_TO_TICKS(1000));
}

    
}
