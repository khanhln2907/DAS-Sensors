#pragma once
#include <stdint.h>

template<typename T> struct Sample {
	T Value;
	uint64_t TimeStamp;
};

struct Position_WGS84 {
	double lon;
	double lat;
	double alt;
};

struct UTCTime {
	uint8_t hour;
	uint8_t minute;
	float second;
};

struct Date {
	uint8_t day;
	uint8_t month;
	uint16_t year;
};