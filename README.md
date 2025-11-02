# Automatic Temperature Controller

Temperature control system for PIC18F43K22. Reads LM35 sensor, controls heater/fan via relays. Uses hysteresis (±2°C deadband) to prevent relay chatter.

## Hardware

**Core:**
- PIC18F43K22 @ 8MHz internal oscillator
- LM35 temperature sensor (TO-92)
- 16x2 LCD (HD44780)
- 3x4 matrix keypad
- 2x 5V relay modules
- 7805 voltage regulator

**Pin Connections:**
```
RA0 (AN0)  → LM35 output
RA1        → Heater relay
RA2        → Fan relay
RB0-RB3    → Keypad rows
RB4-RB6    → Keypad columns (with 10kΩ pull-ups)
PORTC      → LCD data (D0-D7)
RD0        → LCD RS
RD1        → LCD RW
RD2        → LCD EN
```

## Build

**MPLAB X IDE:**
```bash
1. Open project in MPLAB X
2. Set device: PIC18F43K22
3. Set compiler: XC8
4. Build (Ctrl+F11)
5. Program (F5 with PICkit connected)
```

**Command line:**
```bash
xc8-cc -mcpu=18F43K22 src/*.c -o atc.hex
```

## Usage

1. Power on → LCD shows "Temp_sensor"
2. Enter setpoint (0-99°C) using keypad
3. Press `#` to confirm
4. System controls temperature:
   - Temp < (setpoint - 2°C) → Heater ON
   - Temp > (setpoint + 2°C) → Fan ON
   - Within ±2°C → No change (hysteresis prevents oscillation)

Press `*` to restart input.

## Control Logic

Hysteresis prevents relay damage:
```
Setpoint = 25°C, Hysteresis = 2°C

Temp 20°C → Heater ON
Temp 23°C → Heater stays ON (within deadband)
Temp 27°C → Heater OFF, Fan ON
Temp 25°C → Fan stays ON (within deadband)
```

## Testing

**Simulation (Proteus):**
1. Load .hex into PIC18F43K22
2. Test normal operation
3. Test sensor disconnect (should show "SENSOR ERROR")
4. Test invalid inputs (>99)

**Hardware:**
1. Check 5V power rail
2. Adjust LCD contrast (10kΩ pot)
3. Test keypad (each key should display)
4. Heat LM35 with finger → temperature should rise

## Hardware Notes

**Keep LM35 away from heat sources.** Position it away from relays and voltage regulator.

**Use 100nF caps** near every IC for power filtering.

**ADC resolution:** 10-bit gives ~0.5°C per step with 5V reference and LM35 (10mV/°C).

**Relay rating:** Match heater/fan current draw. Use opto-isolated modules for safety.

## Calibration

Compare with known thermometer:
- 0°C (ice water)
- 25°C (room temp)
- 37°C (body temp)

Add offset if needed:
```c
temperature_c += CALIBRATION_OFFSET;  // in main.c
```

## Blog

Detailed writeup: https://medium.com/@yintar5/automatic-temperature-controller-atc-for-various-purpose-by-using-c-programming-language-98950640355c
