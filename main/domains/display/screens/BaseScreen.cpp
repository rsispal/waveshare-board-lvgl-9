#ifndef DOMAINS_DISPLAY_SCREENS_BASE_SCREEN_HPP
#define DOMAINS_DISPLAY_SCREENS_BASE_SCREEN_HPP

#include "esp_lvgl_port.h"
#include <lvgl.h>
#include "theme.h"
#include "BaseScreenElements.cpp"
#include "../../state/state.cpp"

LV_IMG_DECLARE(BC2020)


class BaseScreen {
protected:
    BaseScreenElements *elements = nullptr;
    State *state = nullptr;
    lv_obj_t *contentBuffer = nullptr;  // This holds the specific page content
    BaseScreen* backButtonTargetScreen = nullptr;  // Store the back button target screen

    // Reusable flag to use in Squareline-generated pages to suppress compiler error "invalid conversion from 'int' to 'lv_obj_flag_t' [-fpermissive]"
    const lv_obj_flag_t scrollableClickableFlags = static_cast<lv_obj_flag_t>(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

private:
    // Private init method to create the base UI structure
    void init() {

        if (elements->screen == nullptr) {
            elements->screen = lv_obj_create(nullptr);
            lv_obj_clear_flag( elements->screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
        }
        if (elements->pageContainer == nullptr) {
            elements->pageContainer = lv_obj_create(elements->screen);
            lv_obj_remove_style_all(elements->pageContainer);
            lv_obj_set_width( elements->pageContainer, 800);
            lv_obj_set_height( elements->pageContainer, 480);
            lv_obj_set_align( elements->pageContainer, LV_ALIGN_TOP_MID );
            lv_obj_clear_flag( elements->pageContainer, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE) );    /// Flags
            lv_obj_set_style_bg_color(elements->pageContainer, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(elements->pageContainer, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->statusBar == nullptr) {
            elements->statusBar = lv_obj_create(elements->pageContainer);
            lv_obj_remove_style_all(elements->statusBar);
            lv_obj_set_height( elements->statusBar, 20);
            lv_obj_set_width( elements->statusBar, lv_pct(100));
            lv_obj_set_align( elements->statusBar, LV_ALIGN_TOP_MID );
            lv_obj_set_flex_flow(elements->statusBar,LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(elements->statusBar, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_clear_flag( elements->statusBar, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE) );    /// Flags
            lv_obj_set_style_bg_color(elements->statusBar, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_bg_opa(elements->statusBar, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(elements->statusBar, 4, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(elements->statusBar, 4, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(elements->statusBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(elements->statusBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->statusBarNetworkStatusLabel == nullptr) {
            elements->statusBarNetworkStatusLabel = lv_label_create(elements->statusBar);
            lv_obj_set_width( elements->statusBarNetworkStatusLabel, LV_SIZE_CONTENT);  /// 1
            lv_obj_set_height( elements->statusBarNetworkStatusLabel, LV_SIZE_CONTENT);   /// 1
            lv_obj_set_align( elements->statusBarNetworkStatusLabel, LV_ALIGN_CENTER );
            lv_label_set_text(elements->statusBarNetworkStatusLabel,"Network Status");
            lv_obj_set_style_text_color(elements->statusBarNetworkStatusLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(elements->statusBarNetworkStatusLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(elements->statusBarNetworkStatusLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(elements->statusBarNetworkStatusLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(elements->statusBarNetworkStatusLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(elements->statusBarNetworkStatusLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(elements->statusBarNetworkStatusLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->statusBarPanelConnectionLabel == nullptr) {
            elements->statusBarPanelConnectionLabel = lv_label_create(elements->statusBar);
            lv_obj_set_width( elements->statusBarPanelConnectionLabel, LV_SIZE_CONTENT);  /// 1
            lv_obj_set_height( elements->statusBarPanelConnectionLabel, LV_SIZE_CONTENT);   /// 1
            lv_obj_set_align( elements->statusBarPanelConnectionLabel, LV_ALIGN_CENTER );
            lv_label_set_text(elements->statusBarPanelConnectionLabel,"Connection");
            lv_obj_set_style_text_color(elements->statusBarPanelConnectionLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(elements->statusBarPanelConnectionLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(elements->statusBarPanelConnectionLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(elements->statusBarPanelConnectionLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(elements->statusBarPanelConnectionLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(elements->statusBarPanelConnectionLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(elements->statusBarPanelConnectionLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->statusBarAccessLevelLabel == nullptr) {
            elements->statusBarAccessLevelLabel = lv_label_create(elements->statusBar);
            lv_obj_set_width( elements->statusBarAccessLevelLabel, LV_SIZE_CONTENT);  /// 1
            lv_obj_set_height( elements->statusBarAccessLevelLabel, LV_SIZE_CONTENT);   /// 1
            lv_obj_set_align( elements->statusBarAccessLevelLabel, LV_ALIGN_CENTER );
            lv_label_set_text(elements->statusBarAccessLevelLabel,"Access Level");
            lv_obj_set_style_text_color(elements->statusBarAccessLevelLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(elements->statusBarAccessLevelLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(elements->statusBarAccessLevelLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(elements->statusBarAccessLevelLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(elements->statusBarAccessLevelLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(elements->statusBarAccessLevelLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(elements->statusBarAccessLevelLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->statusBarTimeLabel == nullptr) {
            elements->statusBarTimeLabel = lv_label_create(elements->statusBar);
            lv_obj_set_width( elements->statusBarTimeLabel, LV_SIZE_CONTENT);  /// 1
            lv_obj_set_height( elements->statusBarTimeLabel, LV_SIZE_CONTENT);   /// 1
            lv_obj_set_align( elements->statusBarTimeLabel, LV_ALIGN_CENTER );
            lv_label_set_text(elements->statusBarTimeLabel,"Monday 1st January 1970");
            lv_obj_set_style_text_color(elements->statusBarTimeLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(elements->statusBarTimeLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(elements->statusBarTimeLabel, &lv_font_montserrat_12, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(elements->statusBarTimeLabel, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(elements->statusBarTimeLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(elements->statusBarTimeLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(elements->statusBarTimeLabel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->navigationBar == nullptr) {
            elements->navigationBar = lv_obj_create(elements->pageContainer);
            lv_obj_remove_style_all(elements->navigationBar);
            lv_obj_set_width( elements->navigationBar, 800);
            lv_obj_set_height( elements->navigationBar, 40);
            lv_obj_set_x( elements->navigationBar, 0 );
            lv_obj_set_y( elements->navigationBar, 20 );
            lv_obj_set_align( elements->navigationBar, LV_ALIGN_TOP_MID );
            lv_obj_clear_flag( elements->navigationBar, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE) );    /// Flags
            lv_obj_set_style_pad_left(elements->navigationBar, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(elements->navigationBar, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(elements->navigationBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(elements->navigationBar, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->navigationBarBackButtonContainer == nullptr) {
            elements->navigationBarBackButtonContainer = lv_obj_create(elements->navigationBar);
            lv_obj_remove_style_all(elements->navigationBarBackButtonContainer);
            lv_obj_set_width( elements->navigationBarBackButtonContainer, LV_SIZE_CONTENT);
            lv_obj_set_height( elements->navigationBarBackButtonContainer, 40);
            lv_obj_set_align( elements->navigationBarBackButtonContainer, LV_ALIGN_LEFT_MID );
            lv_obj_add_flag( elements->pageContainer, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE) );    /// Flags
            lv_obj_clear_flag( elements->navigationBarBackButtonContainer, (lv_obj_flag_t)( LV_OBJ_FLAG_SCROLLABLE) );    /// Flags
        }
        if (elements->navigationBarBackButton == nullptr) {
            elements->navigationBarBackButton = lv_imgbtn_create(elements->navigationBarBackButtonContainer);
            lv_imgbtn_set_src(elements->navigationBarBackButton, LV_IMGBTN_STATE_RELEASED, NULL, &BC2020, NULL);
            lv_imgbtn_set_src(elements->navigationBarBackButton, LV_IMGBTN_STATE_PRESSED, NULL, &BC2020, NULL);
            lv_imgbtn_set_src(elements->navigationBarBackButton, LV_IMGBTN_STATE_DISABLED, NULL, &BC2020, NULL);
            lv_imgbtn_set_src(elements->navigationBarBackButton, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &BC2020, NULL);
            lv_imgbtn_set_src(elements->navigationBarBackButton, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &BC2020, NULL);
            lv_imgbtn_set_src(elements->navigationBarBackButton, LV_IMGBTN_STATE_CHECKED_DISABLED, NULL, &BC2020, NULL);
            lv_obj_set_width( elements->navigationBarBackButton, 20);
            lv_obj_set_height( elements->navigationBarBackButton, 20);
            lv_obj_set_align( elements->navigationBarBackButton, LV_ALIGN_LEFT_MID );
            lv_obj_set_style_img_recolor(elements->navigationBarBackButton, lv_color_hex(0x0A85FF), LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_img_recolor_opa(elements->navigationBarBackButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_add_flag( elements->pageContainer, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE) );    /// Flags

        }
        if (elements->navigationBarBackButtonLabel == nullptr) {
            elements->navigationBarBackButtonLabel = lv_label_create(elements->navigationBarBackButtonContainer);
            lv_obj_set_width( elements->navigationBarBackButtonLabel, LV_SIZE_CONTENT);  /// 1
            lv_obj_set_height( elements->navigationBarBackButtonLabel, LV_SIZE_CONTENT);   /// 1
            lv_obj_set_x( elements->navigationBarBackButtonLabel, 24 );
            lv_obj_set_y( elements->navigationBarBackButtonLabel, 0 );
            lv_obj_set_align( elements->navigationBarBackButtonLabel, LV_ALIGN_LEFT_MID );
            lv_obj_set_style_text_color(elements->navigationBarBackButtonLabel, lv_color_hex(0x0A85FF), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(elements->navigationBarBackButtonLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(elements->navigationBarBackButtonLabel, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_add_flag( elements->pageContainer, (lv_obj_flag_t)(LV_OBJ_FLAG_CLICKABLE) );    /// Flags
        }
        if (elements->navigationBarTitle == nullptr) {
            elements->navigationBarTitle = lv_label_create(elements->navigationBar);
            lv_obj_set_width( elements->navigationBarTitle, LV_SIZE_CONTENT);  /// 1
            lv_obj_set_height( elements->navigationBarTitle, LV_SIZE_CONTENT);   /// 1
            lv_obj_set_align( elements->navigationBarTitle, LV_ALIGN_CENTER );
            lv_label_set_text(elements->navigationBarTitle,"Title");
            lv_obj_set_style_text_color(elements->navigationBarTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(elements->navigationBarTitle, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(elements->navigationBarTitle, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->content == nullptr) {
            elements->content = lv_obj_create(elements->pageContainer);
            lv_obj_remove_style_all(elements->content);
            lv_obj_set_width( elements->content, 800);
            lv_obj_set_height( elements->content, lv_pct(100));
            lv_obj_set_x( elements->content, 0 );
            lv_obj_set_y( elements->content, 60 );
            lv_obj_set_align( elements->content, LV_ALIGN_TOP_MID );
            lv_obj_set_style_pad_left(elements->content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(elements->content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(elements->content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(elements->content, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
            // lv_obj_set_style_bg_color(elements->content, lv_color_hex(0x), LV_PART_MAIN | LV_STATE_DEFAULT );
            // lv_obj_set_style_bg_opa(elements->content, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
        }
        if (elements->keyboard == nullptr) {
            elements->keyboard = lv_keyboard_create(elements->screen);
            lv_obj_set_width( elements->keyboard, 800);
            lv_obj_set_height( elements->keyboard, 256);
            lv_obj_set_align( elements->keyboard, LV_ALIGN_BOTTOM_MID );
            lv_obj_add_flag( elements->keyboard, LV_OBJ_FLAG_HIDDEN );   /// Flags
        }
    }

public:
    BaseScreen(BaseScreenElements *elements, State *state) : elements(elements), state(state), contentBuffer(nullptr) {
        init();  // Initialize the UI structure
    }

   virtual ~BaseScreen() {
        // Clean up: Remove the callback when the screen is destroyed
        if (elements->navigationBarBackButton != nullptr) {
            lv_obj_remove_event_cb(elements->navigationBarBackButtonContainer, loadStatic);  // Remove callback when screen is destroyed
        }
    }

    // Abstract methods that derived classes will implement
    virtual void onPageBuild() = 0;
    virtual void onPageLoad() = 0;
    virtual void onPageLeave() = 0;

    // Method to load the screen
    virtual void load() {
        // Call onPageLeave for the current screen
        if (state->currentScreen != nullptr) {
            state->currentScreen->onPageLeave();
            
            // Remove the back button callback associated with the current screen
            if (elements->navigationBarBackButtonContainer != nullptr) {
                lv_obj_remove_event_cb_with_user_data(
                    elements->navigationBarBackButtonContainer, 
                    loadStatic, 
                    state->currentScreen->backButtonTargetScreen  // Pass the current screen as user data to remove its callback
                );
            }
        }

        // Clear the content area
        lv_obj_clean(elements->content);

        // Create and set new content for this screen
        onPageBuild();

        // Apply the new back button callback if there is a target screen
        if (backButtonTargetScreen != nullptr) {
            lv_obj_add_event_cb(
                elements->navigationBarBackButtonContainer, 
                loadStatic, 
                LV_EVENT_CLICKED, 
                backButtonTargetScreen
            );
        }

        // Load the page's specific content
        onPageLoad();

        // Update the state with the current screen
        state->currentScreen = this;

        // Load the screen on the display
        lv_disp_load_scr(elements->screen);
    }

    // The static method to load the target screen on button press
    static void loadStatic(lv_event_t* e) {
        BaseScreen* targetScreen = static_cast<BaseScreen*>(lv_event_get_user_data(e));
   
        // Switch to the new screen
        targetScreen->load();
    }


    // Show the status bar
    void showStatusBar()
    {
        lv_obj_clear_flag(elements->statusBar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_color(elements->statusBar, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarNetworkStatusLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarPanelConnectionLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarAccessLevelLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarTimeLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT );

        lv_obj_set_height( elements->statusBar, 20);
        elements->isStatusBarVisible = true;
        uint8_t y = 20;
        if (elements->isNavigationBarVisible) {
            y += 40;
        }
        lv_obj_set_y( elements->content, y );

    }

    // Hide the status bar
    void hideStatusBar()
    {
        lv_obj_add_flag(elements->statusBar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height( elements->statusBar, 0);
        elements->isStatusBarVisible = false;
        uint8_t y = 0;
        if (elements->isNavigationBarVisible) {
            y += 40;
        }
        lv_obj_set_y( elements->content, y );
    }

    // Show the navigation bar
    void showNavigationBar() {
        lv_obj_clear_flag(elements->navigationBar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height( elements->navigationBar, 40);
        elements->isNavigationBarVisible = true;
        uint8_t y = 40;
        if (elements->isStatusBarVisible) {
            y += 20;
        }
        lv_obj_set_y( elements->content, y );
    }

    // Hide the navigation bar
    void hideNavigationBar() {
        lv_obj_add_flag(elements->navigationBar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height( elements->navigationBar, 0);
        elements->isNavigationBarVisible = false;
        uint8_t y = 0;
        if (elements->isStatusBarVisible) {
            y += 20;
        }
        lv_obj_set_y( elements->content, y );

    }

    // Show the navigation bar back button
    void showNavigationBarBackButton()
    {
        lv_obj_clear_flag(elements->navigationBarBackButtonContainer, LV_OBJ_FLAG_HIDDEN);
    }

    // Hide the navigation bar back button
    void hideNavigationBarBackButton()
    {
        lv_obj_add_flag(elements->navigationBarBackButtonContainer, LV_OBJ_FLAG_HIDDEN);
    }

    // Show the navigation bar title
    void showNavigationBarTitle()
    {
        lv_obj_clear_flag(elements->navigationBarTitle, LV_OBJ_FLAG_HIDDEN);
    }

    // Hide the navigation bar title
    void hideNavigationBarTitle()
    {
        lv_obj_add_flag(elements->navigationBarTitle, LV_OBJ_FLAG_HIDDEN);
    }

    // Set status bar colour
    void setStatusBarColour(uint32_t hexColour, uint32_t textColour) {
        lv_obj_set_style_bg_color(elements->statusBar, lv_color_hex(hexColour), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarNetworkStatusLabel, lv_color_hex(textColour), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarPanelConnectionLabel, lv_color_hex(textColour), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarAccessLevelLabel, lv_color_hex(textColour), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(elements->statusBarTimeLabel, lv_color_hex(textColour), LV_PART_MAIN | LV_STATE_DEFAULT );
    }
    // Set click action for the navigation bar's back button
    void setNavigationBarBackButtonAction(BaseScreen *targetScreen) {
        backButtonTargetScreen = targetScreen;
    }

    // Set the title for the navigation bar
    void setNavigationBarTitle(const char *title) {
        lv_label_set_text(elements->navigationBarTitle, title);
    }

    // Set the label for the navigation bar's back button
    void setNavigationBarBackButtonLabel(const char *label)
    {
        lv_label_set_text(elements->navigationBarBackButtonLabel, label);
    }

};




#endif