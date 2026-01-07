/**
 * BME280 I2C Sensor Driver Implementation
 * 
 * Distributed with a free-will license.
 * Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
 * BME280
 * This code is designed to work with the BME280_I2CS I2C Mini Module available from ControlEverything.com.
 * https://www.controleverything.com/content/Humidity?sku=BME280_I2CS#tabs-0-product_tabset-2
 */

#include "bme280.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

/*******************************************************************************
 * Error String Conversion
 ******************************************************************************/

const char* bme280_error_string(bme280_error_t error)
{
    switch (error) {
        case BME280_OK:
            return "Success";
        case BME280_ERR_BUS_OPEN:
            return "Failed to open I2C bus";
        case BME280_ERR_ADDR_SET:
            return "Failed to set I2C slave address";
        case BME280_ERR_WRITE:
            return "I2C write operation failed";
        case BME280_ERR_READ:
            return "I2C read operation failed";
        case BME280_ERR_NULL_PTR:
            return "NULL pointer passed to function";
        case BME280_ERR_NOT_INIT:
            return "Device not initialized";
        default:
            return "Unknown error";
    }
}

/*******************************************************************************
 * Initialization and Cleanup Functions
 ******************************************************************************/

bme280_error_t bme280_init(bme280_ctx_t *ctx, const char *bus_path, uint8_t address)
{
    if (ctx == NULL || bus_path == NULL) {
        return BME280_ERR_NULL_PTR;
    }

    /* Initialize context to safe defaults */
    ctx->fd = -1;
    ctx->address = address;
    ctx->t_fine = 0;

    /* Open I2C bus */
    ctx->fd = open(bus_path, O_RDWR);
    if (ctx->fd < 0) {
        return BME280_ERR_BUS_OPEN;
    }

    /* Set I2C slave address */
    if (ioctl(ctx->fd, I2C_SLAVE, address) < 0) {
        close(ctx->fd);
        ctx->fd = -1;
        return BME280_ERR_ADDR_SET;
    }

    return BME280_OK;
}

void bme280_close(bme280_ctx_t *ctx)
{
    if (ctx != NULL && ctx->fd >= 0) {
        close(ctx->fd);
        ctx->fd = -1;
    }
}


/*******************************************************************************
 * Calibration Reading Function
 ******************************************************************************/

bme280_error_t bme280_read_calibration(bme280_ctx_t *ctx)
{
    if (ctx == NULL) {
        return BME280_ERR_NULL_PTR;
    }

    if (ctx->fd < 0) {
        return BME280_ERR_NOT_INIT;
    }

    uint8_t reg;
    uint8_t buf[24];
    ssize_t ret;

    /* Read 24 bytes of temperature and pressure calibration data from register 0x88 */
    reg = BME280_REG_CALIB_TEMP_PRESS;
    if (write(ctx->fd, &reg, 1) != 1) {
        return BME280_ERR_WRITE;
    }

    ret = read(ctx->fd, buf, 24);
    if (ret != 24) {
        return BME280_ERR_READ;
    }

    /* Parse temperature coefficients */
    ctx->calib.temp.dig_T1 = (uint16_t)(buf[0] | (buf[1] << 8));
    ctx->calib.temp.dig_T2 = (int16_t)(buf[2] | (buf[3] << 8));
    ctx->calib.temp.dig_T3 = (int16_t)(buf[4] | (buf[5] << 8));

    /* Parse pressure coefficients */
    ctx->calib.press.dig_P1 = (uint16_t)(buf[6] | (buf[7] << 8));
    ctx->calib.press.dig_P2 = (int16_t)(buf[8] | (buf[9] << 8));
    ctx->calib.press.dig_P3 = (int16_t)(buf[10] | (buf[11] << 8));
    ctx->calib.press.dig_P4 = (int16_t)(buf[12] | (buf[13] << 8));
    ctx->calib.press.dig_P5 = (int16_t)(buf[14] | (buf[15] << 8));
    ctx->calib.press.dig_P6 = (int16_t)(buf[16] | (buf[17] << 8));
    ctx->calib.press.dig_P7 = (int16_t)(buf[18] | (buf[19] << 8));
    ctx->calib.press.dig_P8 = (int16_t)(buf[20] | (buf[21] << 8));
    ctx->calib.press.dig_P9 = (int16_t)(buf[22] | (buf[23] << 8));

    /* Read 1 byte of humidity calibration data from register 0xA1 (H1) */
    reg = BME280_REG_CALIB_HUM1;
    if (write(ctx->fd, &reg, 1) != 1) {
        return BME280_ERR_WRITE;
    }

    ret = read(ctx->fd, buf, 1);
    if (ret != 1) {
        return BME280_ERR_READ;
    }

    ctx->calib.hum.dig_H1 = buf[0];

    /* Read 7 bytes of humidity calibration data from register 0xE1 (H2-H6) */
    reg = BME280_REG_CALIB_HUM2;
    if (write(ctx->fd, &reg, 1) != 1) {
        return BME280_ERR_WRITE;
    }

    ret = read(ctx->fd, buf, 7);
    if (ret != 7) {
        return BME280_ERR_READ;
    }

    /* Parse humidity coefficients */
    ctx->calib.hum.dig_H2 = (int16_t)(buf[0] | (buf[1] << 8));
    ctx->calib.hum.dig_H3 = buf[2];
    ctx->calib.hum.dig_H4 = (int16_t)((buf[3] << 4) | (buf[4] & 0x0F));
    ctx->calib.hum.dig_H5 = (int16_t)(((buf[4] >> 4) & 0x0F) | (buf[5] << 4));
    ctx->calib.hum.dig_H6 = (int8_t)buf[6];

    return BME280_OK;
}


/*******************************************************************************
 * Sensor Configuration Function
 ******************************************************************************/

bme280_error_t bme280_configure(bme280_ctx_t *ctx)
{
    if (ctx == NULL) {
        return BME280_ERR_NULL_PTR;
    }

    if (ctx->fd < 0) {
        return BME280_ERR_NOT_INIT;
    }

    uint8_t config[2];

    /* Set humidity oversampling rate to 1x (0x01) */
    config[0] = BME280_REG_CTRL_HUM;
    config[1] = 0x01;
    if (write(ctx->fd, config, 2) != 2) {
        return BME280_ERR_WRITE;
    }

    /* Set measurement control: normal mode, temp and pressure oversampling = 1x (0x27) */
    config[0] = BME280_REG_CTRL_MEAS;
    config[1] = 0x27;
    if (write(ctx->fd, config, 2) != 2) {
        return BME280_ERR_WRITE;
    }

    /* Set config: standby time = 1000ms (0xA0) */
    config[0] = BME280_REG_CONFIG;
    config[1] = 0xA0;
    if (write(ctx->fd, config, 2) != 2) {
        return BME280_ERR_WRITE;
    }

    return BME280_OK;
}


/*******************************************************************************
 * Data Reading and Compensation Function
 ******************************************************************************/

bme280_error_t bme280_read_data(bme280_ctx_t *ctx, bme280_data_t *data)
{
    if (ctx == NULL || data == NULL) {
        return BME280_ERR_NULL_PTR;
    }

    if (ctx->fd < 0) {
        return BME280_ERR_NOT_INIT;
    }

    uint8_t reg;
    uint8_t buf[8];
    ssize_t ret;

    /* Read 8 bytes of data from register 0xF7 */
    reg = BME280_REG_DATA;
    if (write(ctx->fd, &reg, 1) != 1) {
        return BME280_ERR_WRITE;
    }

    ret = read(ctx->fd, buf, 8);
    if (ret != 8) {
        return BME280_ERR_READ;
    }

    /* Convert raw ADC values to 20-bit (pressure, temperature) and 16-bit (humidity) */
    int32_t adc_p = ((int32_t)buf[0] << 12) | ((int32_t)buf[1] << 4) | ((int32_t)buf[2] >> 4);
    int32_t adc_t = ((int32_t)buf[3] << 12) | ((int32_t)buf[4] << 4) | ((int32_t)buf[5] >> 4);
    int32_t adc_h = ((int32_t)buf[6] << 8) | (int32_t)buf[7];

    /* Temperature compensation (from BME280 datasheet) */
    float var1 = (((float)adc_t) / 16384.0f - ((float)ctx->calib.temp.dig_T1) / 1024.0f) 
                 * ((float)ctx->calib.temp.dig_T2);
    float var2 = ((((float)adc_t) / 131072.0f - ((float)ctx->calib.temp.dig_T1) / 8192.0f) 
                 * (((float)adc_t) / 131072.0f - ((float)ctx->calib.temp.dig_T1) / 8192.0f)) 
                 * ((float)ctx->calib.temp.dig_T3);
    ctx->t_fine = (int32_t)(var1 + var2);
    data->temperature_c = (var1 + var2) / 5120.0f;
    data->temperature_f = data->temperature_c * 1.8f + 32.0f;

    /* Pressure compensation (from BME280 datasheet) */
    var1 = ((float)ctx->t_fine / 2.0f) - 64000.0f;
    var2 = var1 * var1 * ((float)ctx->calib.press.dig_P6) / 32768.0f;
    var2 = var2 + var1 * ((float)ctx->calib.press.dig_P5) * 2.0f;
    var2 = (var2 / 4.0f) + (((float)ctx->calib.press.dig_P4) * 65536.0f);
    var1 = (((float)ctx->calib.press.dig_P3) * var1 * var1 / 524288.0f 
           + ((float)ctx->calib.press.dig_P2) * var1) / 524288.0f;
    var1 = (1.0f + var1 / 32768.0f) * ((float)ctx->calib.press.dig_P1);
    
    float p = 1048576.0f - (float)adc_p;
    p = (p - (var2 / 4096.0f)) * 6250.0f / var1;
    var1 = ((float)ctx->calib.press.dig_P9) * p * p / 2147483648.0f;
    var2 = p * ((float)ctx->calib.press.dig_P8) / 32768.0f;
    data->pressure_hpa = (p + (var1 + var2 + ((float)ctx->calib.press.dig_P7)) / 16.0f) / 100.0f;

    /* Humidity compensation (from BME280 datasheet) */
    float var_H = ((float)ctx->t_fine) - 76800.0f;
    var_H = (adc_h - (ctx->calib.hum.dig_H4 * 64.0f + ctx->calib.hum.dig_H5 / 16384.0f * var_H)) 
            * (ctx->calib.hum.dig_H2 / 65536.0f 
            * (1.0f + ctx->calib.hum.dig_H6 / 67108864.0f * var_H 
            * (1.0f + ctx->calib.hum.dig_H3 / 67108864.0f * var_H)));
    data->humidity_rh = var_H * (1.0f - ctx->calib.hum.dig_H1 * var_H / 524288.0f);

    /* Clamp humidity to valid range [0, 100] */
    if (data->humidity_rh > 100.0f) {
        data->humidity_rh = 100.0f;
    } else if (data->humidity_rh < 0.0f) {
        data->humidity_rh = 0.0f;
    }

    return BME280_OK;
}
