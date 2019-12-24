# Meteostation

A simple meteostation based on the STMF411E-Discovery board. It uses temperature, pressure and humidity sensors to read the data. Information is displayed on LCD display and server.

## What is inside
#### STM32F411E-DISCOVERY
The project is based on STM32F411E-DISCOVERY demoboard with a STM32F411VETx microcontroller on it.

#### NodeMCU
A NodeMCU DEVKIT board with ESP8266 chip. Used as Wi-Fi module.
- voltage - 3V
- used frequency 115200 Bits/s

#### DHT22
A hygrometer sensor. Gets humidity and temperature data. Connects with a one-wire interface.
- voltage - 3-5 V
- humidity range 0-100%
- humidity accuracy 2-5%
- temperature range -40..+125C
- temperature accuracy +- 0.5C

#### BMP180

Digital pressure sensor. Temperature measurements included. Uses I2C interface.
- voltage 1.8 ... 3.6V
- pressure range 300 ... 1100hPa (+9000m ... -500m relating to sea level)
- pressure accuracy +- 1hPa
- temperature range -40..+85C
- temperature accuracy +- 1C

#### LCD 1602 

An LCD display that can display a max of 16x2 characters. Has I2C interface converter using the PCF8574 chip.
- voltage 5V
- display color - blue
- regulated sharpness

#### RTC module
A DS1302 based real-time clock module. Uses CR2032 lithium battery to store the data in registers without power. Uses SPI interface.
- voltage 3.3V
- 31 bytes of battery-backed RAM
- stores: seconds, minutes, hours, date, day of the week, year

## How it works


