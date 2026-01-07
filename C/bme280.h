/**
 * BME280 I2C Sensor Driver
 * 
 * Distributed with a free-will license.
 * Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
 * BME280
 * This code is designed to work with the BME280_I2CS I2C Mini Module available from ControlEverything.com.
 * https://www.controleverything.com/content/Humidity?sku=BME280_I2CS#tabs-0-product_tabset-2
 */

#ifndef BME280_H
#define BME280_H

#include <stdint.h>

/*******************************************************************************
 * Default Constants
 ******************************************************************************/

#define BME280_DEFAULT_ADDRESS  0x76
#define BME280_DEFAULT_BUS      "/dev/i2c-1"

/*******************************************************************************
 * Register Address Constants
 ******************************************************************************/

/* Calibration data registers */
#define BME280_REG_CALIB_TEMP_PRESS  0x88  /* 24 bytes: T1-T3, P1-P9 */
#define BME280_REG_CALIB_HUM1        0xA1  /* 1 byte: H1 */
#define BME280_REG_CALIB_HUM2        0xE1  /* 7 bytes: H2-H6 */

/* Control registers */
#define BME280_REG_CTRL_HUM          0xF2  /* Humidity control */
#define BME280_REG_CTRL_MEAS         0xF4  /* Measurement control */
#define BME280_REG_CONFIG            0xF5  /* Configuration */

/* Data registers */
#define BME280_REG_DATA              0xF7  /* 8 bytes: P, T, H */

/*******************************************************************************
 * Error Code Enumeration
 ******************************************************************************/

typedef enum {
    BME280_OK = 0,           /* Operation successful */
    BME280_ERR_BUS_OPEN,     /* Failed to open I2C bus */
    BME280_ERR_ADDR_SET,     /* Failed to set I2C slave address */
    BME280_ERR_WRITE,        /* I2C write operation failed */
    BME280_ERR_READ,         /* I2C read operation failed */
    BME280_ERR_NULL_PTR,     /* NULL pointer passed to function */
    BME280_ERR_NOT_INIT      /* Device not initialized */
} bme280_error_t;


/*******************************************************************************
 * Calibration Data Structures
 ******************************************************************************/

/**
 * Temperature calibration coefficients
 */
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
} bme280_calib_temp_t;

/**
 * Pressure calibration coefficients
 */
typedef struct {
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} bme280_calib_press_t;

/**
 * Humidity calibration coefficients
 */
typedef struct {
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t  dig_H6;
} bme280_calib_hum_t;

/**
 * Combined calibration data structure
 */
typedef struct {
    bme280_calib_temp_t  temp;
    bme280_calib_press_t press;
    bme280_calib_hum_t   hum;
} bme280_calib_t;

/*******************************************************************************
 * Sensor Data Structure
 ******************************************************************************/

/**
 * Computed sensor readings
 */
typedef struct {
    float temperature_c;   /* Temperature in Celsius */
    float temperature_f;   /* Temperature in Fahrenheit */
    float pressure_hpa;    /* Pressure in hectopascals */
    float humidity_rh;     /* Relative humidity percentage */
} bme280_data_t;

/*******************************************************************************
 * Context Structure
 ******************************************************************************/

/**
 * BME280 device context
 */
typedef struct {
    int            fd;       /* I2C file descriptor (-1 if not open) */
    uint8_t        address;  /* I2C device address */
    bme280_calib_t calib;    /* Calibration coefficients */
    int32_t        t_fine;   /* Fine temperature for compensation */
} bme280_ctx_t;

/*******************************************************************************
 * Public API Functions
 ******************************************************************************/

/**
 * Initialize BME280 sensor connection
 * @param ctx      Pointer to context structure (caller-allocated)
 * @param bus_path I2C bus device path (e.g., "/dev/i2c-1")
 * @param address  I2C device address (typically 0x76 or 0x77)
 * @return BME280_OK on success, error code on failure
 */
bme280_error_t bme280_init(bme280_ctx_t *ctx, const char *bus_path, uint8_t address);

/**
 * Read calibration coefficients from sensor
 * @param ctx Pointer to initialized context
 * @return BME280_OK on success, error code on failure
 */
bme280_error_t bme280_read_calibration(bme280_ctx_t *ctx);

/**
 * Configure sensor operating mode
 * @param ctx Pointer to initialized context
 * @return BME280_OK on success, error code on failure
 */
bme280_error_t bme280_configure(bme280_ctx_t *ctx);

/**
 * Read and compute all sensor measurements
 * @param ctx  Pointer to initialized context with calibration data
 * @param data Pointer to structure to receive computed values
 * @return BME280_OK on success, error code on failure
 */
bme280_error_t bme280_read_data(bme280_ctx_t *ctx, bme280_data_t *data);

/**
 * Close I2C connection and release resources
 * @param ctx Pointer to context to close
 */
void bme280_close(bme280_ctx_t *ctx);

/**
 * Convert error code to human-readable string
 * @param error Error code to convert
 * @return Pointer to static string describing the error
 */
const char* bme280_error_string(bme280_error_t error);

#endif /* BME280_H */
