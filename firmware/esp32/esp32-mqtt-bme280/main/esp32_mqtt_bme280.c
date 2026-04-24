#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bme280.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "wifi.h"

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

#define BME280_ADDR 0x76   // or 0x77 depending on your scan

void i2c_master_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void i2c_scanner()
{
    printf("Scanning I2C bus...\n");

    for (uint8_t i = 1; i < 127; i++)
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();

        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);

        esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        if (ret == ESP_OK)
        {
            printf("Device found at address: 0x%X\n", i);
        }
    }
}

/*esp_err_t bme280_read_register(uint8_t reg, uint8_t *data)
{
    return i2c_master_write_read_device(
        I2C_MASTER_NUM,
        BME280_ADDR,
        &reg,
        1,
        data,
        1,
        1000 / portTICK_PERIOD_MS);
}*/

void sensor_task(void *pvParameter)
{
    bme280_calib_data_t calib;
    int32_t temp_raw;

    if (bme280_read_calibration(&calib) != ESP_OK)
    {
        printf("Calibration failed\n");
        vTaskDelete(NULL);
    }

    while (1)
    {
        if (bme280_read_temperature_raw(&temp_raw) == ESP_OK)
        {
            float temp = bme280_compensate_temperature(temp_raw, &calib);
            printf("Temperature: %.2f °C\n", temp);
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    i2c_master_init();

    wifi_init();

    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
}