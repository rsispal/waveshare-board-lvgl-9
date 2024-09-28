#ifndef DOMAINS_DISPLAY_SCREENS_BASE_SCREEN_HPP
#define DOMAINS_DISPLAY_SCREENS_BASE_SCREEN_HPP

#include "esp_lvgl_port.h"
#include <lvgl.h>
#include "theme.h"

LV_IMG_DECLARE(BC2020)

class BaseScreen {
    static constexpr char *TAG = "BaseScreen";
    
    public:
    // These are available for derived classes
    lv_obj_t *screen = nullptr;
    lv_obj_t *pageContainer = nullptr;
    lv_obj_t *statusBar = nullptr;
    lv_obj_t *statusBarNetworkStatusLabel = nullptr;
    lv_obj_t *statusBarPanelConnectionLabel = nullptr;
    lv_obj_t *statusBarAccessLevelLabel = nullptr;
    lv_obj_t *statusBarTimeLabel = nullptr;

    lv_obj_t *navigationBar = nullptr;
    lv_obj_t *navigationBarBackButtonContainer = nullptr;
    lv_obj_t *navigationBarBackButton = nullptr;
    lv_obj_t *navigationBarBackButtonLabel = nullptr;
    lv_obj_t *navigationBarTitle = nullptr;

    lv_obj_t *content = nullptr;
    lv_obj_t *keyboard = nullptr;


    BaseScreen() {
        ESP_LOGI(TAG, "Constructor Start");
        init();
        ESP_LOGI(TAG, "Constructor End");
    }
    
    // Destructor - proper cleanup of LVGL objects
    virtual ~BaseScreen()
    {
        ESP_LOGI(TAG, "Destructor Start");
        // Remove event callbacks to prevent potential crashes due to dangling pointers
        lv_obj_remove_event_cb(navigationBarBackButton, loadStatic);
        lv_obj_remove_event_cb(navigationBarBackButtonLabel, loadStatic);
        lv_obj_remove_event_cb(keyboard, handleCloseKeyboardStatic);

        // Clean up the entire object hierarchy by deleting the root object
        if (screen)
        {
            lv_obj_del(screen);
            screen = nullptr; // Nullify the pointer after deletion for safety
        }
        ESP_LOGI(TAG, "Destructor End");
    }

    void init() {
        ESP_LOGI(TAG, "Initialising base layout");
        // If a screen already exists, delete it first to avoid memory leaks
        if (screen != nullptr)
        {
            ESP_LOGI(TAG, "Screen already exists, deleting");
            lv_obj_del(screen);
            screen = nullptr;
        }

        screen = lv_obj_create(NULL); // Create a new screen object
        assert(screen);           // Ensure the screen is created
        lv_obj_clear_flag( screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

        pageContainer = lv_obj_create(screen);
        lv_obj_remove_style_all(pageContainer);
        lv_obj_set_width( pageContainer, 800);
        lv_obj_set_height( pageContainer, 480);
        lv_obj_set_align( pageContainer, LV_ALIGN_TOP_MID );
        lv_obj_clear_flag( pageContainer, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE) );    /// Flags
        lv_obj_set_style_bg_color(pageContainer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_bg_opa(pageContainer, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

        statusBar = lv_obj_create(pageContainer);
        lv_obj_remove_style_all(statusBar);
        lv_obj_set_height( statusBar, 20);
        lv_obj_set_width( statusBar, lv_pct(100));
        lv_obj_set_align( statusBar, LV_ALIGN_TOP_MID );
        lv_obj_set_flex_flow(statusBar,LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(statusBar, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_clear_flag( statusBar, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE) );    /// Flags
        lv_obj_set_style_bg_color(statusBar, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_bg_opa(statusBar, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(statusBar, 4, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(statusBar, 4, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(statusBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(statusBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

        statusBarNetworkStatusLabel = lv_label_create(statusBar);
        lv_obj_set_width( statusBarNetworkStatusLabel, LV_SIZE_CONTENT);  /// 1
        lv_obj_set_height( statusBarNetworkStatusLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_align( statusBarNetworkStatusLabel, LV_ALIGN_CENTER );
        lv_label_set_text(statusBarNetworkStatusLabel,"Network Status");
        lv_obj_set_style_text_color(statusBarNetworkStatusLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_opa(statusBarNetworkStatusLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(statusBarNetworkStatusLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(statusBarNetworkStatusLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(statusBarNetworkStatusLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(statusBarNetworkStatusLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(statusBarNetworkStatusLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

        statusBarPanelConnectionLabel = lv_label_create(statusBar);
        lv_obj_set_width( statusBarPanelConnectionLabel, LV_SIZE_CONTENT);  /// 1
        lv_obj_set_height( statusBarPanelConnectionLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_align( statusBarPanelConnectionLabel, LV_ALIGN_CENTER );
        lv_label_set_text(statusBarPanelConnectionLabel,"Connection");
        lv_obj_set_style_text_color(statusBarPanelConnectionLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_opa(statusBarPanelConnectionLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(statusBarPanelConnectionLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(statusBarPanelConnectionLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(statusBarPanelConnectionLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(statusBarPanelConnectionLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(statusBarPanelConnectionLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

        statusBarAccessLevelLabel = lv_label_create(statusBar);
        lv_obj_set_width( statusBarAccessLevelLabel, LV_SIZE_CONTENT);  /// 1
        lv_obj_set_height( statusBarAccessLevelLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_align( statusBarAccessLevelLabel, LV_ALIGN_CENTER );
        lv_label_set_text(statusBarAccessLevelLabel,"Access Level");
        lv_obj_set_style_text_color(statusBarAccessLevelLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_opa(statusBarAccessLevelLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(statusBarAccessLevelLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(statusBarAccessLevelLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(statusBarAccessLevelLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(statusBarAccessLevelLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(statusBarAccessLevelLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

        statusBarTimeLabel = lv_label_create(statusBar);
        lv_obj_set_width( statusBarTimeLabel, LV_SIZE_CONTENT);  /// 1
        lv_obj_set_height( statusBarTimeLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_align( statusBarTimeLabel, LV_ALIGN_CENTER );
        lv_label_set_text(statusBarTimeLabel,"Monday 1st January 1970");
        lv_obj_set_style_text_color(statusBarTimeLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_opa(statusBarTimeLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(statusBarTimeLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(statusBarTimeLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(statusBarTimeLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(statusBarTimeLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(statusBarTimeLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

        navigationBar = lv_obj_create(pageContainer);
        lv_obj_remove_style_all(navigationBar);
        lv_obj_set_width( navigationBar, 800);
        lv_obj_set_height( navigationBar, 40);
        lv_obj_set_x( navigationBar, 0 );
        lv_obj_set_y( navigationBar, 20 );
        lv_obj_set_align( navigationBar, LV_ALIGN_TOP_MID );
        lv_obj_clear_flag( navigationBar, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE) );    /// Flags
        lv_obj_set_style_pad_left(navigationBar, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(navigationBar, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(navigationBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(navigationBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

        navigationBarBackButtonContainer = lv_obj_create(navigationBar);
        lv_obj_remove_style_all(navigationBarBackButtonContainer);
        lv_obj_set_width( navigationBarBackButtonContainer, 100);
        lv_obj_set_height( navigationBarBackButtonContainer, 40);
        lv_obj_set_align( navigationBarBackButtonContainer, LV_ALIGN_LEFT_MID );
        lv_obj_clear_flag( navigationBarBackButtonContainer, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE) );    /// Flags

        navigationBarBackButton = lv_imgbtn_create(navigationBarBackButtonContainer);
        lv_imgbtn_set_src(navigationBarBackButton, LV_IMGBTN_STATE_RELEASED, NULL, &BC2020, NULL);
        lv_imgbtn_set_src(navigationBarBackButton, LV_IMGBTN_STATE_PRESSED, NULL, &BC2020, NULL);
        lv_imgbtn_set_src(navigationBarBackButton, LV_IMGBTN_STATE_DISABLED, NULL, &BC2020, NULL);
        lv_imgbtn_set_src(navigationBarBackButton, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &BC2020, NULL);
        lv_imgbtn_set_src(navigationBarBackButton, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &BC2020, NULL);
        lv_imgbtn_set_src(navigationBarBackButton, LV_IMGBTN_STATE_CHECKED_DISABLED, NULL, &BC2020, NULL);
        lv_obj_set_width( navigationBarBackButton, 20);
        lv_obj_set_height( navigationBarBackButton, 20);
        lv_obj_set_align( navigationBarBackButton, LV_ALIGN_LEFT_MID );
        lv_obj_set_style_img_recolor(navigationBarBackButton, lv_color_hex(0x0A85FF), LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_img_recolor_opa(navigationBarBackButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

        navigationBarBackButtonLabel = lv_label_create(navigationBarBackButtonContainer);
        lv_obj_set_width( navigationBarBackButtonLabel, LV_SIZE_CONTENT);  /// 1
        lv_obj_set_height( navigationBarBackButtonLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_x( navigationBarBackButtonLabel, 24 );
        lv_obj_set_y( navigationBarBackButtonLabel, 0 );
        lv_obj_set_align( navigationBarBackButtonLabel, LV_ALIGN_LEFT_MID );
        lv_obj_set_style_text_color(navigationBarBackButtonLabel, lv_color_hex(0x0A85FF), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_opa(navigationBarBackButtonLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(navigationBarBackButtonLabel, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

        navigationBarTitle = lv_label_create(navigationBar);
        lv_obj_set_width( navigationBarTitle, LV_SIZE_CONTENT);  /// 1
        lv_obj_set_height( navigationBarTitle, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_align( navigationBarTitle, LV_ALIGN_CENTER );
        lv_label_set_text(navigationBarTitle,"Title");
        lv_obj_set_style_text_color(navigationBarTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_opa(navigationBarTitle, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(navigationBarTitle, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

        content = lv_obj_create(pageContainer);
        lv_obj_remove_style_all(content);
        lv_obj_set_width( content, 800);
        lv_obj_set_height( content, 420);
        lv_obj_set_x( content, 0 );
        lv_obj_set_y( content, 60 );
        lv_obj_set_align( content, LV_ALIGN_TOP_MID );
        lv_obj_set_style_pad_left(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);

        keyboard = lv_keyboard_create(screen);
        lv_obj_set_width( keyboard, 800);
        lv_obj_set_height( keyboard, 256);
        lv_obj_set_align( keyboard, LV_ALIGN_BOTTOM_MID );
        lv_obj_add_flag( keyboard, LV_OBJ_FLAG_HIDDEN );   /// Flags

        // lv_obj_add_event_cb(keyboard, handleCloseKeyboardStatic, LV_EVENT_ALL, keyboard); // DISABLED FOR TESTING
    }

    void setTitle(const char *title)
    {
        lv_label_set_text(navigationBarTitle, title);
    }

    // Set navigation bar back button action with proper callback binding
    void setNavigationBarBackButtonAction(BaseScreen *cls)
    {
        lv_obj_add_event_cb(navigationBarBackButton, loadStatic, LV_EVENT_CLICKED, cls);
        lv_obj_add_event_cb(navigationBarBackButtonLabel, loadStatic, LV_EVENT_CLICKED, cls);
    }

    void showStatusBar()
    {
        lv_obj_clear_flag(statusBar, LV_OBJ_FLAG_HIDDEN);
    }

    void hideStatusBar()
    {
        lv_obj_add_flag(statusBar, LV_OBJ_FLAG_HIDDEN);
    }

    void setStatusBarNetworkStatusLabel(const char *value)
    {
        lv_label_set_text(statusBarNetworkStatusLabel, value);
    }

    void setStatusBarPanelConnectionLabel(const char *value)
    {
        lv_label_set_text(statusBarPanelConnectionLabel, value);
    }

    void setStatusBarAccessLevelLabel(const char *value)
    {
        lv_label_set_text(statusBarAccessLevelLabel, value);
    }

    void setStatusBarTimeLabel(const char *value)
    {
        lv_label_set_text(statusBarTimeLabel, value);
    }

    void showNavigationBar()
    {
        lv_obj_clear_flag(navigationBar, LV_OBJ_FLAG_HIDDEN);
    }

    void hideNavigationBar()
    {
        lv_obj_add_flag(navigationBar, LV_OBJ_FLAG_HIDDEN);
    }

    void showNavigationBarBackButton()
    {
        lv_obj_clear_flag(navigationBarBackButton, LV_OBJ_FLAG_HIDDEN);
    }

    void hideNavigationBarBackButton()
    {
        lv_obj_add_flag(navigationBarBackButton, LV_OBJ_FLAG_HIDDEN);
    }

    void setNavigationBarBackButtonLabel(const char *title)
    {
        lv_label_set_text(navigationBarBackButtonLabel, title);
    }

    void showTitle()
    {
        lv_obj_clear_flag(navigationBarTitle, LV_OBJ_FLAG_HIDDEN);
    }

    void hideTitle()
    {
        lv_obj_add_flag(navigationBarTitle, LV_OBJ_FLAG_HIDDEN);
    }

    void showKeyboard(bool numericOnly = false)
    {
        if (numericOnly)
        {
            lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_NUMBER);
        }
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_pad_bottom(content, 500, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(content, static_cast<lv_obj_flag_t>(LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE)); /// Flags
    }

    void hideKeyboard()
    {
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_pad_bottom(content, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_flag(content, static_cast<lv_obj_flag_t>(LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE)); /// Flags
    }

    void prepareContentArea() {
        if (content) {
            lv_obj_del(content);
            content = nullptr;
        }

       content = lv_obj_create(pageContainer);
        lv_obj_remove_style_all(content);
        lv_obj_set_width( content, 800);
        lv_obj_set_height( content, 420);
        lv_obj_set_x( content, 0 );
        lv_obj_set_y( content, 60 );
        lv_obj_set_align( content, LV_ALIGN_TOP_MID );
        lv_obj_set_style_pad_left(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
    }

    void load()
    {
        ESP_LOGI(TAG, "Load start");
        if (screen)
        {
            // Load the screen
            lv_disp_load_scr(screen);
        }
        ESP_LOGI(TAG, "Load end");
    }

    // Static wrapper function that can be passed to LVGL safely
    static void loadStatic(lv_event_t *e)
    {
        BaseScreen *instance = static_cast<BaseScreen *>(lv_event_get_user_data(e));
        if (instance)
        {
            instance->load();
        }
    }

    static void handleCloseKeyboardStatic(lv_event_t *e)
    {
        lv_obj_t *keyboard = static_cast<lv_obj_t *>(lv_event_get_user_data(e));
        lv_event_code_t code = lv_event_get_code(e);
        if (code == LV_EVENT_CANCEL || code == LV_EVENT_READY)
        {
            lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN); // Hide the keyboard
            lv_keyboard_set_textarea(keyboard, NULL);      // Remove the field association
        }
    }


};

#endif