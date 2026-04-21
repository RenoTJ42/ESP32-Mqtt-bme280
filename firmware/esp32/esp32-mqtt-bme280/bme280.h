#ifndef BME280_H
#define BME280_H

#include "esp_err.h"
#include <stdint.h>

#define BME280_ADDR 0x76

esp_err_t bme280_get_chip_id(uint8_t *chip_id);

esp_err_t bme280_read_register(uint8_t reg, uint8_t *data);

esp_err_t bme280_read_temperature_raw(int32_t *temp_raw);

#endif