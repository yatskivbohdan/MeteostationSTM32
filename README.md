# Meteostation

A simple meteostation based on the STMF411E-Discovery board. It uses temperature, pressure and humidity sensors to read the data. Information is displayed on LCD display and server.

## What is used
#### STM32F411E-DISCOVERY
The project is based on STM32F411E-DISCOVERY demoboard with a STM32F411VETx microcontroller on it.

![alt text](https://media.rs-online.com/t_large/F8463503-01.jpg)


#### NodeMCU
A NodeMCU DEVKIT board with ESP8266 chip. Used as Wi-Fi module.

![alt text](https://images-na.ssl-images-amazon.com/images/I/71efjnKymHL._SX342_.jpg)
- voltage - 3V
- used frequency 115200 Bits/s


#### DHT22
A hygrometer sensor. Gets humidity and temperature data. Connects with a one-wire interface.

![alt text](https://cdn2.bigcommerce.com/n-arxsrf/07ifr7/products/5627/images/11051/High_Precision_AM2302_DHT22_Digital_Temperature_Humidity_Sensor_Module_AB117-1__42948.1544510190.1280.1280.png?c=2)
- voltage - 3-5 V
- humidity range 0-100%
- humidity accuracy 2-5%
- temperature range -40..+125C
- temperature accuracy +- 0.5C

#### BMP180
Digital pressure sensor. Temperature measurements included. Uses I2C interface.

![alt text](https://cdn1.bigcommerce.com/server800/a8995/products/691/images/3673/bmp180_barometric_pressure_module_oddwires__11269.1489616942.500.500.jpg?c=2)
- voltage 1.8 ... 3.6V
- pressure range 300 ... 1100hPa (+9000m ... -500m relating to sea level)
- pressure accuracy +- 1hPa
- temperature range -40..+85C
- temperature accuracy +- 1C

#### LCD 1602 

An LCD display that can display a max of 16x2 characters. Has I2C interface converter using the PCF8574 chip.

![alt text](https://i.ebayimg.com/images/g/eokAAOSw-jhUGjSr/s-l400.jpg)
- voltage 5V
- display color - blue
- regulated sharpness

#### RTC module
A DS1302 based real-time clock module. Uses CR2032 lithium battery to store the data in registers without power. Uses SPI interface.

![alt text](https://gsm-komplekt.ua/57723-large_default/57723.jpg)
- voltage 3.3V
- 31 bytes of battery-backed RAM
- stores: seconds, minutes, hours, date, day of the week, year

## How it works
![Alt text](scheme.jpg?raw=true "Title")

