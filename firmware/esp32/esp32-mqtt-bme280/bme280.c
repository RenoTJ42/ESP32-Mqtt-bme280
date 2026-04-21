#include "bme280.h"
#include "driver/i2c.h"

#define I2C_MASTER_NUM I2C_NUM_0
#define BME280_CHIP_ID_REG 0xD0
#define MAX_RETRIES 3

esp_err_t bme280_read_register(uint8_t reg, uint8_t *data)
{
    return i2c_master_write_read_device(
        I2C_MASTER_NUM,
        BME280_ADDR,
        &reg,
        1,
        data,
        1,
        1000 / portTICK_PERIOD_MS);
}

esp_err_t bme280_get_chip_id(uint8_t *chip_id)
{
    esp_err_t ret;

    for (int i = 0; i < MAX_RETRIES; i++)
    {
        ret = bme280_read_register(BME280_CHIP_ID_REG, chip_id);

        if (ret == ESP_OK)
        {
            return ESP_OK;
        }

        // small delay before retry
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    return ret;
}

esp_err_t bme280_read_temperature_raw(int32_t *temp_raw)
{
    uint8_t data[3];

    esp_err_t ret = i2c_master_write_read_device(
        I2C_MASTER_NUM,
        BME280_ADDR,
        (uint8_t[]){0xFA},
        1,
        data,
        3,
        1000 / portTICK_PERIOD_MS);

    if (ret != ESP_OK)
        return ret;

    *temp_raw = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));

    return ESP_OK;
}
