# Tech Stack & Build Commands

## Languages & Platforms

| Platform | Language | Dependencies |
|----------|----------|--------------|
| Raspberry Pi | Python 2 | `smbus` library |
| Raspberry Pi | Java | `pi4j` library |
| BeagleBone Black | C | Linux I2C headers (`linux/i2c-dev.h`) |
| Arduino | C++ (.ino) | `Wire.h` (built-in) |
| Particle Photon | C++ (.ino) | `application.h`, `spark_wiring_i2c.h` |
| Onion Omega | Python 2 | `OmegaExpansion.onionI2C`, `pyOnionI2C` |

## Build & Run Commands

### Python (Raspberry Pi)
```bash
python BME280.py
```

### Java (Raspberry Pi)
```bash
pi4j BME280.java   # Compile
pi4j BME280        # Run
```

### C (BeagleBone Black)
```bash
gcc BME280.c -o BME280
./BME280
```

### Arduino
Open `BME280.ino` in Arduino IDE, compile, and upload to board.

### Particle Photon
Copy code to Particle Build IDE (https://build.particle.io/build/), verify and flash.

### Onion Omega
```bash
opkg update
opkg install python-light pyOnionI2C
python BME280.py
```

## Key Technical Details
- I2C communication protocol
- Sensor address: `0x76`
- All implementations use the same calibration coefficient algorithm from the BME280 datasheet
- Oversampling rate: 1x for humidity, temperature, and pressure
- Standby time: 1000ms
