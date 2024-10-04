#ifndef DOMAINS_DISPLAY_SCREENS_BASE_SCREEN_ELEMENTS_HPP
#define DOMAINS_DISPLAY_SCREENS_BASE_SCREEN_ELEMENTS_HPP

#include <lvgl.h>

class BaseScreenElements {
public:
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

    bool isStatusBarVisible = false;
    bool isNavigationBarVisible = false;


    BaseScreenElements() {
        // Constructor (do nothing, actual initialization happens in BaseScreen)
    }

    ~BaseScreenElements() {
        // Destructor (free LVGL objects here if needed)
        // screen = nullptr;
        // pageContainer = nullptr;
        // statusBar = nullptr;
        // statusBarNetworkStatusLabel = nullptr;
        // statusBarPanelConnectionLabel = nullptr;
        // statusBarAccessLevelLabel = nullptr;
        // statusBarTimeLabel = nullptr;

        // navigationBar = nullptr;
        // navigationBarBackButtonContainer = nullptr;
        // navigationBarBackButton = nullptr;
        // navigationBarBackButtonLabel = nullptr;
        // navigationBarTitle = nullptr;

        // content = nullptr;
        // keyboard = nullptr;

        // currentScreen = nullptr;
    }
};


#endif