/**
 * Mock linux/i2c-dev.h for compilation testing on non-Linux systems
 * This file is only used for syntax/compilation verification on macOS/Windows
 * On actual BeagleBone Black (Linux), the real linux/i2c-dev.h is used
 */

#ifndef MOCK_LINUX_I2C_DEV_H
#define MOCK_LINUX_I2C_DEV_H

/* I2C slave address ioctl command */
#define I2C_SLAVE 0x0703

#endif /* MOCK_LINUX_I2C_DEV_H */
