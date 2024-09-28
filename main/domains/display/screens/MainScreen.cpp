#ifndef DOMAINS_DISPLAY_SCREENS_MAIN_SCREEN_HPP
#define DOMAINS_DISPLAY_SCREENS_MAIN_SCREEN_HPP

#include "esp_lvgl_port.h"
#include <lvgl.h>
#include "theme.h"
#include "BaseScreen.cpp"


class MainScreen {
    // These are available for derived classes
    static constexpr char *TAG = "MainScreen";
    BaseScreen *base = nullptr;

    public:

    MainScreen(BaseScreen * _base): base(_base) {
        assert(base);
    }

// Destructor - proper cleanup of LVGL objects
    virtual ~MainScreen()
    {
        ESP_LOGI(TAG, "Destructor Start");
        // Remove event callbacks to prevent potential crashes due to dangling pointers
        // lv_obj_remove_event_cb(navigationBarBackButton, loadStatic);
        // lv_obj_remove_event_cb(navigationBarBackButtonLabel, loadStatic);
        // lv_obj_remove_event_cb(keyboard, handleCloseKeyboardStatic);

        // Clean up the entire object hierarchy by deleting the root object
        // if (screen)
        // {
        //     lv_obj_del(screen);
        //     screen = nullptr; // Nullify the pointer after deletion for safety
        // }
        ESP_LOGI(TAG, "Destructor End");
    }

    void init() {
        ESP_LOGI(TAG, "Page content setup start");
        base->prepareContentArea();
        assert(base->content);
        base->showStatusBar();
        base->showNavigationBar();
        base->showKeyboard();


        lv_obj_clear_flag(base->content, LV_OBJ_FLAG_SCROLLABLE);                                                   // Disable scrolling
        lv_obj_set_flex_flow(base->content, LV_FLEX_FLOW_COLUMN);                                                   // Use column flex layout
        lv_obj_set_flex_align(base->content, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);     // Center alignment
        lv_obj_set_style_bg_color(base->content, lv_color_hex(Theme::background), LV_PART_MAIN | LV_STATE_DEFAULT); // Set background color
        lv_obj_set_style_bg_opa(base->content, 255, LV_PART_MAIN | LV_STATE_DEFAULT);                               // Set background opacity

        // Create and configure a label for loading message
        lv_obj_t *label = lv_label_create(base->content);
        lv_label_set_text(label, "Main Content Page");                  // Set label text
        lv_obj_set_style_pad_top(label, 16, LV_PART_MAIN | LV_STATE_DEFAULT); // Padding for label
        lv_obj_set_style_text_color(label, lv_color_white(), 0);              // Set text color to white
        ESP_LOGI(TAG, "Page content setup end");        
    }

    void show() {
        base->load();

    }

    private:
    lv_obj_t *label;
};

#endif