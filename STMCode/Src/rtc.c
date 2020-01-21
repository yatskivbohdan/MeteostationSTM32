#include "rtc.h"
extern I2C_HandleTypeDef hi2c2;

#define DS3231_ADDRESS 0xD0

uint8_t decToBcd(int val){
	return (uint8_t)((val/10*16) + (val % 10));
}

int bcdToDec(uint8_t val){
	return (int)((val/16*10) + (val % 16));
}

void setTime(uint8_t sec, uint8_t min, uint8_t hours, uint8_t dow, uint8_t day, uint8_t month, uint8_t year){
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hours);
	set_time[3] = decToBcd(dow);
	set_time[4] = decToBcd(day);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);

	HAL_I2C_Mem_Write(&hi2c2, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

void getTime(TIME * time){
	uint8_t get_time[7];
	HAL_I2C_Mem_Read(&hi2c2, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
	time->seconds = bcdToDec(get_time[0]);
	time->minutes = bcdToDec(get_time[1]);
	time->hours = bcdToDec(get_time[2]);
	time->dayofweek = bcdToDec(get_time[3]);
	time->day = bcdToDec(get_time[4]);
	time->month = bcdToDec(get_time[5]);
	time->year = bcdToDec(get_time[6]);

}

