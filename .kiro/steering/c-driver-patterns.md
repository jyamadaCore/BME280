---
inclusion: fileMatch
fileMatchPattern: "C/**/*.{c,h}"
---

# C Driver Development Patterns

## Error Handling Pattern

Use a consistent error enumeration and propagation strategy:

```c
typedef enum {
    DRIVER_OK = 0,
    DRIVER_ERR_SPECIFIC_1,
    DRIVER_ERR_SPECIFIC_2,
    // ...
} driver_error_t;

// Always provide error-to-string conversion
const char* driver_error_string(driver_error_t error);
```

## Context-Based API Pattern

Use an opaque context structure for state management:

```c
typedef struct {
    int fd;           // Resource handle (-1 when invalid)
    // ... other state
} driver_ctx_t;

// Init must set safe defaults before any operation that could fail
driver_error_t driver_init(driver_ctx_t *ctx, ...);

// Close must be idempotent and NULL-safe
void driver_close(driver_ctx_t *ctx);
```

## Validation Macro Pattern

Reduce repetitive validation with macros:

```c
#define CHECK_CTX(ctx) \
    do { \
        if ((ctx) == NULL) return DRIVER_ERR_NULL_PTR; \
        if ((ctx)->fd < 0) return DRIVER_ERR_NOT_INIT; \
    } while (0)
```

## Named Constants for Hardware Registers

Always define named constants for:
- Register addresses
- Configuration values
- Magic numbers from datasheets

```c
/* Configuration values */
#define BME280_OSRS_HUM_1X      0x01  /* Humidity oversampling 1x */
#define BME280_CTRL_MEAS_NORMAL 0x27  /* Normal mode, T/P oversampling 1x */
#define BME280_STANDBY_1000MS   0xA0  /* Standby time 1000ms */
```

## Division Safety

When implementing compensation formulas, guard against division by zero:

```c
if (var1 == 0.0f) {
    data->pressure_hpa = 0.0f;  // or return error
} else {
    p = (p - (var2 / 4096.0f)) * 6250.0f / var1;
}
```

## Resource Cleanup on Partial Failure

When initialization involves multiple steps, clean up on failure:

```c
bme280_error_t bme280_init(bme280_ctx_t *ctx, ...) {
    ctx->fd = open(...);
    if (ctx->fd < 0) return BME280_ERR_BUS_OPEN;
    
    if (ioctl(...) < 0) {
        close(ctx->fd);      // Clean up!
        ctx->fd = -1;
        return BME280_ERR_ADDR_SET;
    }
    return BME280_OK;
}
```

## Testing Strategy

For hardware-dependent drivers:
1. Unit test pure computation functions (compensation formulas)
2. Use mock headers for platform-specific includes
3. Property-based tests for algorithm correctness
4. Integration tests require actual hardware

## File Organization

```
driver/
├── driver.h          # Public API only
├── driver.c          # Implementation
├── example_main.c    # Usage example
└── test/
    ├── test_driver.c # Unit/property tests
    └── Makefile
```
