/**
 * BME280 Example Program
 * 
 * Distributed with a free-will license.
 * Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
 * BME280
 * This code is designed to work with the BME280_I2CS I2C Mini Module available from ControlEverything.com.
 * https://www.controleverything.com/content/Humidity?sku=BME280_I2CS#tabs-0-product_tabset-2
 * 
 * This example demonstrates full usage of the BME280 driver library.
 */

#include "bme280.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    bme280_ctx_t ctx;
    bme280_data_t data;
    bme280_error_t err;

    /* Initialize sensor with default parameters */
    err = bme280_init(&ctx, BME280_DEFAULT_BUS, BME280_DEFAULT_ADDRESS);
    if (err != BME280_OK) {
        fprintf(stderr, "Init failed: %s\n", bme280_error_string(err));
        return 1;
    }

    /* Read calibration coefficients */
    err = bme280_read_calibration(&ctx);
    if (err != BME280_OK) {
        fprintf(stderr, "Read calibration failed: %s\n", bme280_error_string(err));
        bme280_close(&ctx);
        return 1;
    }

    /* Configure sensor operating mode */
    err = bme280_configure(&ctx);
    if (err != BME280_OK) {
        fprintf(stderr, "Configure failed: %s\n", bme280_error_string(err));
        bme280_close(&ctx);
        return 1;
    }

    /* Read sensor data */
    err = bme280_read_data(&ctx, &data);
    if (err != BME280_OK) {
        fprintf(stderr, "Read data failed: %s\n", bme280_error_string(err));
        bme280_close(&ctx);
        return 1;
    }

    /* Print formatted output matching original format */
    printf("Temperature in Celsius : %.2f C\n", data.temperature_c);
    printf("Temperature in Fahrenheit : %.2f F\n", data.temperature_f);
    printf("Pressure : %.2f hPa \n", data.pressure_hpa);
    printf("Relative Humidity : %.2f %%\n", data.humidity_rh);

    /* Cleanup */
    bme280_close(&ctx);

    return 0;
}
