#ifndef DOMAINS_DISPLAY_CPP
#define DOMAINS_DISPLAY_CPP

#include "./lcd/LCD.cpp"
#include "./tft/TFT.cpp"
#include "./lvgl/LVGL.cpp"

class Display {
        const char * TAG  = "DISPLAY";
        I2CMaster *i2cMaster = nullptr;
        LCD *lcd = nullptr;
        TFT *tft = nullptr;

    public:
        Lvgl *lvgl = nullptr;

        Display(I2CMaster *i2cMaster) : i2cMaster(i2cMaster) {
            assert(i2cMaster);
            lcd = new LCD();
            tft = new TFT(i2cMaster);
        };

        esp_err_t setup() {
            // Setup LCD display
            ESP_ERROR_CHECK(lcd->setup());

            // Setup TFT panel
            ESP_ERROR_CHECK(tft->setup());
            
            lvgl = new Lvgl();

            // Setup LVGL
            lvgl->setup(lcd->lcd_panel, tft->touch_handle);

            return ESP_OK;
        };
        
        bool lvgl_mutex_lock(uint32_t timeout_ms) {
            return lvgl->mutex_lock(timeout_ms);
        }

        void lvgl_mutex_unlock() {
            return lvgl->mutex_unlock();
        }
};

#endif