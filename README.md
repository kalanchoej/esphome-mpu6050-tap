# MPU6050 Tap Detection for ESPHome

This component enables tap detection functionality for MPU6050-based accelerometer modules (like GY-521 breakout boards) in ESPHome. When properly configured, it can detect taps and report them to Home Assistant.

## Hardware Setup

Connect your MPU6050 module to your ESPHome-compatible board:
- VCC → 3.3V (or 5V, depending on your board's voltage compatibility)
- GND → GND
- SCL → GPIO22 (default I2C clock pin)
- SDA → GPIO21 (default I2C data pin)
- INT → Any free GPIO pin on your board (defaults to GPIO 10)

Note: The INT pin on the MPU6050 outputs a signal when tap detection occurs. This needs to be connected to a GPIO pin on your board.

## ESPHome Configuration

1. Add this repository to your ESPHome configuration:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/kalanchoej/esphome-mpu6050-tap
      ref: main
    components: [mpu6050-tap]
```

2. Configure the component:

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

binary_sensor:
  - platform: gpio
    pin:
      number: GPIO26  # The GPIO pin where you connected the INT pin
      mode: INPUT_PULLUP
    name: "MPU6050 Tap Detected"
    on_press:
      then:
        - logger.log: "Tap detected!"

custom_component:
  - id: mpu6050_sensor
    type: MPU6050Custom
    interrupt_pin: 10  # The GPIO pin where you connected the INT pin
    threshold: "medium"  # How hard you need to tap
    duration: "short"     # How long the tap should be
```

## Configuration Options

### Threshold Settings
The threshold determines how strong a tap needs to be to trigger detection:
- `light`: Detects very light taps, triggers easily
- `medium`: Standard tap detection
- `strong`: Requires stronger taps to trigger

### Duration Settings
Duration determines how long the tap movement should be:
- `short`: Quick taps
- `medium`: Medium duration taps
- `long`: Longer taps

## Technical Details

The MPU6050 tap detection uses several key registers:

- Power Management (0x6B): Wakes up the MPU6050
- Accelerometer Configuration (0x1C): Sets accelerometer sensitivity to ±2g
- Motion Threshold (0x1F): Controls tap detection sensitivity
- Motion Duration (0x20): Sets the tap duration requirement
- Interrupt Enable (0x38): Enables motion detection interrupt

The component will log its configuration on startup, showing the selected threshold and duration settings.

## Debugging

If tap detection isn't working:
1. Verify your physical connections, especially the INT pin
2. Check that the I2C scan shows the device (enabled in the config above)
3. Monitor the ESPHome logs for the configuration message and tap detection events
4. If you have a scope you can watch the INT pin during a tap event

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the GPL-3.0 license - see the LICENSE file for details.
