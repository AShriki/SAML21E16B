#include "userRTC.h"

void initRTC(enum ALARMEN alarmEn){

    // ensure apba mask value for rtc is 1
    MCLK->APBAMASK.reg |= MCLK_APBAMASK_RTC ;
    // set the interrupts to be triggered to alarm0
    RTC->MODE2.INTENSET.reg |= RTC_MODE2_INTENSET_ALARM0 ;
    // set the alarm mask
    RTC->MODE2.Mode2Alarm->MASK.reg = RTC_MODE2_MASK_SEL_MMDDHHMMSS ;
    // set prescaler to 1.024KHz
    // set rtc mode to clock
    // set sync to allow clock reading
    // enable RTC
    RTC->MODE2.CTRLA.reg |= RTC_MODE2_CTRLA_PRESCALER_DIV1024 | RTC_MODE2_CTRLA_MODE_CLOCK | RTC_MODE2_CTRLA_ENABLE | RTC_MODE2_CTRLA_CLOCKSYNC;
    // enable RTC run during debug
    //RTC->MODE2.DBGCTRL.reg |= RTC_DBGCTRL_DBGRUN ;
    // set date
    resetRTC();
    // enable NVIC interrupt for RTC
    NVIC_EnableIRQ(RTC_IRQn);
    // clear rtc interrupts
    RTC->MODE2.INTFLAG.reg |= RTC_MODE2_INTFLAG_ALARM0 ;

}
/*
  @description : waits for sync to complete

  @return      : nothing
*/
void waitRTCSync(){
      // Wait for values to sync
       while( RTC->MODE2.SYNCBUSY.bit.CLOCK || RTC->MODE2.SYNCBUSY.bit.CLOCKSYNC){;}
}
/*
  @description : resets the RTC date and time

  @return      : nothing  
*/
void resetRTC(){
    
    // reset the date and time in CLOCK
    RTC->MODE2.CLOCK.reg = RTC_MODE2_CLOCK_YEAR(0) | RTC_MODE2_CLOCK_MONTH(1) | RTC_MODE2_CLOCK_DAY(1) | RTC_MODE2_CLOCK_HOUR(0) | RTC_MODE2_CLOCK_MINUTE(0) | RTC_MODE2_CLOCK_SECOND(0);
    waitRTCSync() ; 
}
/*
  @description : sets the alarm for a specific date and time on the register level

  @var year    : alarm year
  @var month   : alarm  month
  @var day     : alarm day
  @var hour    : alarm hour
  @var minute  : alarm minute
  @var second  : alarm second
  @return      : nothing
*/
void setAlarm(int year , int month , int day , int hour , int minute , int second){
  // set the mode2 alarm
  RTC->MODE2.Mode2Alarm->ALARM.reg =  RTC_MODE2_ALARM_YEAR(year) | RTC_MODE2_ALARM_MONTH(month) | RTC_MODE2_ALARM_DAY(day) | RTC_MODE2_ALARM_HOUR(hour) | RTC_MODE2_ALARM_MINUTE(minute) | RTC_MODE2_ALARM_SECOND(second) ;
}

uint32_t getClock(){return RTC->MODE2.CLOCK.reg ;}
uint32_t getSecond(){return RTC->MODE2.CLOCK.bit.SECOND ;}
uint32_t getMinute(){return RTC->MODE2.CLOCK.bit.MINUTE ;}
uint32_t getHour(){return RTC->MODE2.CLOCK.bit.HOUR ;}
uint32_t getDay(){return RTC->MODE2.CLOCK.bit.DAY ;}
uint32_t getMonth(){return RTC->MODE2.CLOCK.bit.MONTH ;}
uint32_t getYear(){return RTC->MODE2.CLOCK.bit.YEAR ;}