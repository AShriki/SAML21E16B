#ifndef _AlarmHeap_
#define _AlarmHeap_

#include <stdint.h>
#include "MinMaxHeap.h"

#define REFYEAR 2016

int addNode(struct Date , struct Heap* , alarmAction , uint8_t , enum AlarmType , struct Date , int );
int removeAlarm(struct Heap* , struct HeapNode);
int parseAlarm(struct Heap*);
int nDaysInMonth(int, int);
int isLeapYear(int);
#endif

