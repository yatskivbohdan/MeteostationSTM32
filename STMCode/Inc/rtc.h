#include "stm32f4xx_hal.h"

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t dayofweek;
	uint8_t day;
	uint8_t month;
	uint8_t year;
} TIME;

uint8_t decToBcd(int val);

int bcdToDec(uint8_t val);

void setTime(uint8_t sec, uint8_t min, uint8_t hours, uint8_t dow, uint8_t day, uint8_t month, uint8_t year);

void getTime(TIME * time);
