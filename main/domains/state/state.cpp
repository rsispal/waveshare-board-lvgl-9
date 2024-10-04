#ifndef DOMAINS_STATE_HPP
#define DOMAINS_STATE_HPP

#include "time.h"
#include "esp_system.h"

class BaseScreen;  // Forward declare BaseScreen since State will store a pointer to it

class State
{



public:

    BaseScreen *currentScreen = nullptr;

    // Constructor
    State() : currentScreen(nullptr)
    {
    }

    ~State() {

    }


};
#endif