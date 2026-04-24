#ifndef BME280_H
#define BME280_H

#include "esp_err.h"
#include <stdint.h>

#define BME280_ADDR 0x76

esp_err_t bme280_get_chip_id(uint8_t *chip_id);

esp_err_t bme280_read_register(uint8_t reg, uint8_t *data);

esp_err_t bme280_read_temperature_raw(int32_t *temp_raw);

esp_err_t bme280_read_calibration(bme280_calib_data_t *calib);
float bme280_compensate_temperature(int32_t adc_T, bme280_calib_data_t *calib);

esp_err_t bme280_read_pressure_raw(int32_t *press_raw);
esp_err_t bme280_read_humidity_raw(int32_t *hum_raw);

typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
} bme280_calib_data_t;



#endif