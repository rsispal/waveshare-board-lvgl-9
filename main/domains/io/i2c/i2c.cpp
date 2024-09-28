#ifndef DOMAINS_IO_I2C_MASTER_HPP
#define DOMAINS_IO_I2C_MASTER_HPP

#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

class I2CMaster
{
public:
    uint8_t port;
    uint8_t sda_io_num;
    uint8_t scl_io_num;
    uint32_t frequency;
    uint16_t timeout;
    
    i2c_master_bus_handle_t bus_handle;

    I2CMaster(uint8_t port_num, uint8_t sda_io, uint8_t scl_io, uint32_t freq_hz, uint16_t transmitTimeoutMs)
        : port(port_num), sda_io_num(sda_io), scl_io_num(scl_io), frequency(freq_hz), timeout(transmitTimeoutMs) {}

    esp_err_t begin()
    {
        i2c_master_bus_config_t conf = {
            .i2c_port = port,
            .sda_io_num = static_cast<gpio_num_t>(sda_io_num),
            .scl_io_num = static_cast<gpio_num_t>(scl_io_num),
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .flags = {
                .enable_internal_pullup = true,
            },
        };
        return i2c_new_master_bus(&conf, &bus_handle);
    }

    esp_err_t addDevice(uint8_t device_address, i2c_master_dev_handle_t &dev_handle)
    {
        i2c_device_config_t dev_cfg = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = device_address,
            .scl_speed_hz = frequency,
        };
        return i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);
    }

    esp_err_t writeToDevice(i2c_master_dev_handle_t dev_handle, uint8_t *data, size_t len)
    {
        return i2c_master_transmit(dev_handle, data, len, timeout / portTICK_PERIOD_MS);
    }

    // Method to read from an I2C device
    esp_err_t readFromDevice(i2c_master_dev_handle_t dev_handle, uint8_t *buffer, size_t len)
    {
        return i2c_master_receive(dev_handle, buffer, len, timeout / portTICK_PERIOD_MS);
    }

    ~I2CMaster()
    {
        if (bus_handle != nullptr)
        {
            i2c_del_master_bus(bus_handle);
        }
    }

private:

};

#endif