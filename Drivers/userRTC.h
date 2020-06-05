#ifndef _USERRTC_
#define _USERRTC_

#ifndef _DEFINITIONS_
#include "definitions.h"
#endif

enum XTALSEL{INTERNAL = 0 , EXTERNAL = 1};
enum CLOCKDESCRIPTOR{SECOND = RTC_MODE2_CLOCK_SECOND_Msk , MINUTE = RTC_MODE2_CLOCK_MINUTE_Msk , HOUR = RTC_MODE2_CLOCK_HOUR_Msk , DAY = RTC_MODE2_CLOCK_DAY_Msk , MONTH = RTC_MODE2_CLOCK_MONTH_Msk , YEAR = RTC_MODE2_CLOCK_YEAR_Msk};
enum ALARMEN{ALARMOFF = 0,ALARMON = 1};
enum AlarmType { RUNONCE = 0, RECURRING = 1 };

struct Date {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};

void initRTC(enum ALARMEN);
void resetRTC(void);
void waitRTCSync(void);
// year, month, day, hour, minute, second
void setAlarm(int,int,int,int,int,int);
uint32_t getClock(void);

uint32_t getSecond(void);
uint32_t getMinute(void);
uint32_t getHour(void);
uint32_t getDay(void);
uint32_t getMonth(void);
uint32_t getYear(void);

#endif