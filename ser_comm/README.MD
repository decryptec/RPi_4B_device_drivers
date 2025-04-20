# Raspberry Pi Kernel Module Practice

Focused on creating and experimenting with kernel modules for UART, I2C, and SPI on a Raspberry Pi. For I2C and SPI, modules will be implemented both with and without using the device tree.

## Project Structure

- `uart/`: Contains kernel modules related to UART.
- `i2c/`
  - `i2c_char/`: I2C kernel modules without the device tree.
  - `i2c_dts/`: I2C kernel modules with the device tree.
- `spi/`
  - `spi_char/`: SPI kernel modules without the device tree.
  - `spi_dts/`: SPI kernel modules with the device tree.

## Objectives

1. Understand and implement kernel modules for UART communication.
2. Explore I2C and SPI protocols with and without device tree integration.
3. Gain hands-on experience with driver development on Raspberry Pi.
