[![BME280](BME280_I2CS.png)](https://www.controleverything.com/content/Humidity?sku=BME280_I2CS)
# BME280
BME280 Digital Humidity, Pressure and Temperature Sensor

The BME280 is a combined humidity, pressure and temperature sensor.

This Device is available from ControlEverything.com [SKU: BME280_I2CS]

https://shop.controleverything.com/products/digital-humidity-pressure-and-temperature-sensor?variant=25687652235

This Sample code can be used with Raspberry Pi, Arduino, Particle Photon, Beaglebone Black and Onion Omega.

## Java
Download and install pi4j library on Raspberry pi. Steps to install pi4j are provided at:

http://pi4j.com/install.html

Download (or git pull) the code in pi.

Compile the java program.
```cpp
$> pi4j BME280.java
```

Run the java program.
```cpp
$> pi4j BME280
```

## Python
Download and install smbus library on Raspberry pi. Steps to install smbus are provided at:

https://pypi.python.org/pypi/smbus-cffi/0.5.1

Download (or git pull) the code in pi. Run the program.

```cpp
$> python BME280.py
```

## Arduino
Download and install Arduino Software (IDE) on your machine. Steps to install Arduino are provided at:

https://www.arduino.cc/en/Main/Software

Download (or git pull) the code and double click the file to run the program.

Compile and upload the code on Arduino IDE and see the output on Serial Monitor.


## Particle Photon

Login to your Photon and setup your device according to steps provided at:

https://docs.particle.io/guide/getting-started/connect/photon/

Download (or git pull) the code. Go to online IDE and copy the code.

https://build.particle.io/build/

Verify and flash the code on your Photon. Code output is shown in logs at dashboard:

https://dashboard.particle.io/user/logs


## C (BeagleBone Black)

Setup your BeagleBone Black according to steps provided at:

https://beagleboard.org/getting-started

Download (or git pull) the code in Beaglebone Black.

### File Structure

The C implementation is organized as a modular library:

- `bme280.h` - Header file with API declarations and type definitions
- `bme280.c` - Library implementation
- `example_main.c` - Example program demonstrating usage

### Building

Compile the library and example program:
```bash
gcc C/bme280.c C/example_main.c -o C/bme280_example
```

Run the program:
```bash
./C/bme280_example
```

### Using as a Library

Include the header in your project and link with `bme280.c`:

```c
#include "bme280.h"

int main(void) {
    bme280_ctx_t ctx;
    bme280_data_t data;
    
    bme280_init(&ctx, BME280_DEFAULT_BUS, BME280_DEFAULT_ADDRESS);
    bme280_read_calibration(&ctx);
    bme280_configure(&ctx);
    bme280_read_data(&ctx, &data);
    
    printf("Temperature: %.2f C\n", data.temperature_c);
    
    bme280_close(&ctx);
    return 0;
}
```

### Running Tests

```bash
cd C/test
make
./test_bme280
```

## Onion Omega

Get Started and setting up the Onion Omega according to steps provided at :

https://wiki.onion.io/Get-Started

To install the Python module, run the following commands:
```cpp
opkg update
```
```cpp
opkg install python-light pyOnionI2C
```

Download (or git pull) the code in Onion Omega. Run the program.

```cpp
$> python BME280.py
```
#####The code output is the relative humidity in %RH, pressure in hPa and temperature reading in degree celsius and fahrenheit.
