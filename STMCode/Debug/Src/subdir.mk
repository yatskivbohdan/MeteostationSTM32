################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DS1302.c \
../Src/bmp180.c \
../Src/dht22.c \
../Src/dhtxx.c \
../Src/dwt_delay.c \
../Src/gpio.c \
../Src/i2c-lcd.c \
../Src/i2c.c \
../Src/i2s.c \
../Src/main.c \
../Src/rtc.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/sys.c \
../Src/syscalls.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/us_timing.c \
../Src/usart.c \
../Src/usb_host.c \
../Src/usbh_conf.c \
../Src/usbh_platform.c 

OBJS += \
./Src/DS1302.o \
./Src/bmp180.o \
./Src/dht22.o \
./Src/dhtxx.o \
./Src/dwt_delay.o \
./Src/gpio.o \
./Src/i2c-lcd.o \
./Src/i2c.o \
./Src/i2s.o \
./Src/main.o \
./Src/rtc.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/sys.o \
./Src/syscalls.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/us_timing.o \
./Src/usart.o \
./Src/usb_host.o \
./Src/usbh_conf.o \
./Src/usbh_platform.o 

C_DEPS += \
./Src/DS1302.d \
./Src/bmp180.d \
./Src/dht22.d \
./Src/dhtxx.d \
./Src/dwt_delay.d \
./Src/gpio.d \
./Src/i2c-lcd.d \
./Src/i2c.d \
./Src/i2s.d \
./Src/main.d \
./Src/rtc.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/sys.d \
./Src/syscalls.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/us_timing.d \
./Src/usart.d \
./Src/usb_host.d \
./Src/usbh_conf.d \
./Src/usbh_platform.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F411xE -I"D:/UCU/POCO/STM/PMD/PMDLCDUART/Inc" -I"D:/UCU/POCO/STM/PMD/PMDLCDUART/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/UCU/POCO/STM/PMD/PMDLCDUART/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/UCU/POCO/STM/PMD/PMDLCDUART/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"D:/UCU/POCO/STM/PMD/PMDLCDUART/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc" -I"D:/UCU/POCO/STM/PMD/PMDLCDUART/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/UCU/POCO/STM/PMD/PMDLCDUART/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


