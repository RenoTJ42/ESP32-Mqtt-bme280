#include <stdio.h>
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bme280.h"

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

void app_main()
{
    i2c_master_init();

    /*while (1)
    {
        i2c_scanner();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }*/

    
   uint8_t chip_id = 0;


    if (bme280_get_chip_id(&chip_id) == ESP_OK)
    {
        printf("BME280 Chip ID: 0x%X\n", chip_id);
    }
    else
    {
        printf("Failed to read chip ID after retries\n");
    }

    if (*chip_id == 0x60)
    {
        printf("Success.\n");
    }

    else
    {
        printf("Invalid device detected.\n");
    }

    while (1)
    {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}