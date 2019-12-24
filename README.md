# General
This is a course project for Principles of Computer Organization course at UCU, Lviv.

Authors [Bohdan Yatskiv](https://github.com/yatskivbohdan) and [Volodymyr Lesyk](https://github.com/EntityFile)
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
- 32kHZ buit-in crystal
- stores: seconds, minutes, hours, date, day of the week, year

## How it works
##### Connection scheme
![Alt text](scheme.jpg?raw=true "Scheme")
### Reading the data
**DHT22** is connected with one wire connection, so it uses signals of different time to distinguish the data, so it needs to work with microsecond delays(there is **us_timing.h** library for this). It sends 4 bytes of data and one checksum byte.

**BMP180** is connected with **I2C** interface. At first, the data is received uncompensated, then it compensates and we recieve floating point type temperature value and integer type pressure.
There are libraries provided(**dhtxx.h, bmp180.h**) for each of the sensors(dht22, bmp180). Using them the data is read, but first the program checks whether there are some errors. If so, the coresponding error message is displayed on LCD and on the server. 

Also there is a library(**DS1302.h**) for **RTC** module which is storing the hour and the date even when STM is unpowered. This is provided by CR2032 lithium battery which gives the low power to store the data. When the time and date are set, the data is written in registers of **DS1302**, then using built-in crystal, RTC changes the time and the date.
### Displaying the data
#### LCD display 
When the data is read, it is displayed on **LCD 1602** display. It uses PCF8574 chip to convert to **I2C**. A simple library i2c-lcd.h from here(https://controllerstech.com/i2c-lcd-in-stm32/) was taken and modified a bit. As there is only two rows on display the data is shown one by one for 5 seconds each: first time and date, then hygrometer data and then data from barometer.
#### Server
Using NodeMCU and **esp8266** chip on it the data is transmitted to the server. It uses Arduino code, so the source code could be find in following folder. At first the data is transmitted by **UART** from the STM to NodeMCU. However, as it is impossible to transmit floating point numbers via UART, the data is refactored and send by parts. Also each data part is send two times and NodeMCU checks if they are the same to avoid some errors.
## Our progress
The main part of work with reading and displaying data is done, but there are still some things to do.
### What is done
- reading the data from dht and bmp
- displaying the data on display
- refactoring ang optimizing data
- sending data to local web server
### What should be done
- reading real time clock data is not working yet(will be fixed soon)
- the data should be stored to display changing trend for some periods of time
- a larger display could be used
- web server now is extremely primitive and needs new functions, such setting the date and displaying data changes
- some power modes could be designed
