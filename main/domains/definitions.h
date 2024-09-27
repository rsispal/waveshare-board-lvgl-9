#ifndef DEFINITIONS_H
#define DEFINITIONS_H

extern "C"
{
#include "driver/i2c.h"
#include "driver/uart.h"
}
// I2C
static const i2c_port_t I2C_MASTER_NUM = static_cast<i2c_port_t>(0); /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
static const uint8_t I2C_MASTER_SCL_IO = 9;                          /*!< GPIO number used for I2C master clock */
static const uint8_t I2C_MASTER_SDA_IO = 8;                          /*!< GPIO number used for I2C master data  */
static const uint64_t I2C_MASTER_FREQ_HZ = 400000;                   /*!< I2C master clock frequency */
static const uint8_t I2C_MASTER_TX_BUF_DISABLE = 0;                  /*!< I2C master doesn't need buffer */
static const uint8_t I2C_MASTER_RX_BUF_DISABLE = 0;                  /*!< I2C master doesn't need buffer */
static const uint16_t I2C_MASTER_TIMEOUT_MS = 1000;

// RS485
static const uint8_t RS485_UART_RX = 15;
static const uint8_t RS485_UART_TX = 16;
static const uart_port_t RS485_UART_PORT_NUMBER = static_cast<uart_port_t>(2);
static const long RS485_UART_BAUD = 1200;

// PARALLEL LCD
static const uint8_t LCD_RGB_BUFFER_NUMS = 1;
static const uint64_t LCD_PIXEL_CLOCK_HZ = (16 * 1000 * 1000);
static const int PIN_NUM_BK_LIGHT = -1;
static const uint8_t LCD_RGB_HPW = 4;
static const uint8_t LCD_RGB_HBP = 8;
static const uint8_t LCD_RGB_HFP = 8;
static const uint8_t LCD_RGB_VPW = 4;
static const uint8_t LCD_RGB_VBP = 16;
static const uint8_t LCD_RGB_VFP = 16;
static const uint8_t LCD_RGB_PCLK_ACTIVE_NEG = 1;         // 0: rising edge, 1: falling edge
static const uint8_t LCD_RGB_DATA_WIDTH = 16;             //  8 | 16
static const uint8_t LCD_RGB_PIXEL_BITS = 16;             // 24 | 16
static const uint8_t LCD_RGB_FRAME_BUF_NUM = 1;           // 1/2/3
static const uint8_t LCD_RGB_BOUNCE_BUF_SIZE = 800 * 480; // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                          // To enable the bounce buffer, set it to a non-zero value. Typically set to `ESP_PANEL_LCD_WIDTH * 10`
                                                          // The size of the Bounce Buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`,
                                                          // where N is an even number.

static const uint8_t LCD_BK_LIGHT_ON_LEVEL = 1;
static const uint8_t LCD_BK_LIGHT_OFF_LEVEL = !LCD_BK_LIGHT_ON_LEVEL;
static const uint8_t PIN_NUM_HSYNC = 46;
static const uint8_t PIN_NUM_VSYNC = 3;
static const uint8_t PIN_NUM_DE = 5;
static const uint8_t PIN_NUM_PCLK = 7;
static const uint8_t PIN_NUM_DATA0 = 14;  // B3
static const uint8_t PIN_NUM_DATA1 = 38;  // B4
static const uint8_t PIN_NUM_DATA2 = 18;  // B5
static const uint8_t PIN_NUM_DATA3 = 17;  // B6
static const uint8_t PIN_NUM_DATA4 = 10;  // B7
static const uint8_t PIN_NUM_DATA5 = 39;  // G2
static const uint8_t PIN_NUM_DATA6 = 0;   // G3
static const uint8_t PIN_NUM_DATA7 = 45;  // G4
static const uint8_t PIN_NUM_DATA8 = 48;  // G5
static const uint8_t PIN_NUM_DATA9 = 47;  // G6
static const uint8_t PIN_NUM_DATA10 = 21; // G7
static const uint8_t PIN_NUM_DATA11 = 1;  // R3
static const uint8_t PIN_NUM_DATA12 = 2;  // R4
static const uint8_t PIN_NUM_DATA13 = 42; // R5
static const uint8_t PIN_NUM_DATA14 = 41; // R6
static const uint8_t PIN_NUM_DATA15 = 40; // R7
static const int PIN_NUM_DISP_EN = -1;

const uint8_t IO_EXPANDER_TOUCH_PANEL_RESET = 1 << 1;
const uint8_t IO_EXPANDER_LCD_BACKLIGHT = 1 << 2;
const uint8_t IO_EXPANDER_LCD_RESET = 1 << 3;
const uint8_t IO_EXPANDER_SD_CS = 1 << 4;
const uint8_t IO_EXPANDER_USB_SELECT = 1 << 5;

// CH442G GPIO Extender
static const uint8_t CH442G_GPIO_EXTENDER_SLAVE_ADDRESS = 0x24;

// TFT?
static const gpio_num_t GPIO_INPUT_IO_4 = static_cast<gpio_num_t>(4);
static const uint64_t GPIO_INPUT_PIN_SEL = 1ULL << (uint8_t)GPIO_INPUT_IO_4;

// The pixel number in horizontal and vertical
static const int LCD_H_RES = 800;
static const int LCD_V_RES = 480;

// LVGL Configuration
static const int LVGL_TICK_PERIOD_MS = 2;
static const int LVGL_TASK_MAX_DELAY_MS = 500;
static const int LVGL_TASK_MIN_DELAY_MS = 1;
static const int LVGL_TASK_STACK_SIZE = (4 * 1024);
static const int LVGL_TASK_PRIORITY = 2;

#endif
