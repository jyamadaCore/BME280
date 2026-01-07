# Project Structure

```
BME280/
├── Arduino/
│   └── BME280.ino          # Arduino sketch
├── C/
│   └── BME280.c            # BeagleBone Black implementation
├── Java/
│   └── BME280.java         # Raspberry Pi (pi4j)
├── Onion_Omega_Python/
│   └── BME280.py           # Onion Omega implementation
├── Particle/
│   └── BME280.ino          # Particle Photon sketch
├── Python/
│   └── BME280.py           # Raspberry Pi (smbus)
├── BME280_I2CS.png         # Sensor module image
├── LICENSE                 # License file
└── README.md               # Setup instructions per platform
```

## Conventions
- One folder per target platform
- Each folder contains a single `BME280.*` file with the complete implementation
- File naming matches the sensor model name
- All implementations are self-contained (no shared code between platforms)
- Each file includes the same license header and ControlEverything.com attribution

## Code Pattern
All implementations follow the same algorithm:
1. Read calibration coefficients from sensor registers
2. Configure sensor (humidity, measurement, config registers)
3. Read raw ADC data
4. Apply compensation formulas from BME280 datasheet
5. Output formatted temperature, pressure, and humidity values
