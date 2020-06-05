#include "AlarmHeap.h"
#include "MinMaxHeap.h"

int isLater(struct HeapNode, struct HeapNode);
int isSame(struct HeapNode, struct HeapNode);
/*
  @description : this method determines if the given year is a leap year

  @var year    : user given year
  @return      : 1 this year is a leap year 0 if not
*/
int isLeapYear(int year) {
	// If a year is multiple of 400,  
	// then it is a leap year 
	if (year % 400 == 0)
		return 1;

	// Else If a year is muliplt of 100, 
	// then it is not a leap year 
	if (year % 100 == 0)
		return 0;

	// Else If a year is muliplt of 4, 
	// then it is a leap year 
	if (year % 4 == 0)
		return 1;

	return 0;
}
/*
  @description : this method gets the number of days in a given month of a given year

  @var year    : alarm year
  @var month   : alarm  month
  @return      : the number of days in a month
*/
int nDaysInMonth(int month, int year) {
	if (month > 12) {
		month = month % 12 + 1;
		year++;
	}

	switch (month) {
	case 1: return 31;
	case 2: {
		if (isLeapYear(year))
			return 29;
		else
			return 28;
	}
	case 3: return 31;
	case 4: return 30;
	case 5: return 31;
	case 6: return 30;
	case 7: return 31;
	case 8: return 31;
	case 9: return 30;
	case 10: return 31;
	case 11: return 30;
	case 12: return 31;
	default: return 0;
	}

}
/*
  @description : adds a node to the alarm heap

  @var date			: the alarm date contained by the node
  @var heap			: the heap to which we are adding the node
  @var userFunction : the finction to be called upon alarm activation
  @var param		: the parameter to be passed to @var userFunction
  @var recurType	: specifies if an alarm is recurrant
  @var recurDate	: specifies the interval on which this alarm recurs
  @var recurNum		: number of time to repeat the alarm (-1 for infinity)
  @return			: 0 if heap is full, 1 if successfully inserted a new element
*/
int addNode(struct Date date, struct Heap* heap, alarmAction userFunction, uint8_t param, enum AlarmType recurType, struct Date recurDate, int numRecur) {

	struct HeapNode n;
	n.date = date;
	n.p = userFunction;
	n.param = param;
	n.almType = recurType;
	n.recurIn = recurDate;
	n.numRecur = numRecur;

	return insert(heap, n);
}

/*
  @description : Checks to see if node a occurs after node b (isLater(a , b) reads : is a later than b)

  @var a       : node a
  @var b       : node b
  @return      : 1 if a is after b and 0 if not
*/
int isAfter(struct HeapNode a, struct HeapNode b) {

	if (a.date.year < b.date.year) {
		return 0;
	}
	else if (a.date.year > b.date.year) {
		return 1;
	}
	else if (a.date.month < b.date.month) {
		return 0;
	}
	else if (a.date.month > b.date.month) {
		return 1;
	}
	else if (a.date.day < b.date.day) {
		return 0;
	}
	else if (a.date.day > b.date.day) {
		return 1;
	}
	else if (a.date.hour < b.date.hour) {
		return 0;
	}
	else if (a.date.hour > b.date.hour) {
		return 1;
	}
	else if (a.date.minute < b.date.minute) {
		return 0;
	}
	else if (a.date.minute > b.date.minute) {
		return 1;
	}
	else if (a.date.second < b.date.second) {
		return 0;
	}
	else if (a.date.second > b.date.second) {
		return 1;
	}
	return 0;
}
/*
  @description : Determines if two nodes are the same while allowing nodes that occur at the same time

  @var a       : node a
  @var b       : node b
  @return      : 1 if the nodes are the same, 0 if not
*/
int isSame(struct HeapNode a, struct HeapNode b) {

	if (a.date.year == b.date.year && a.date.month == b.date.month && a.date.day == b.date.day && a.date.hour == b.date.hour && a.date.minute == b.date.minute && a.date.second == b.date.second && a.p == b.p && a.param == b.param)
		return 1;
	return 0;

}
/*
  @description : Determines if two nodes are the same

  @var a       : node a
  @var b       : node b
  @return      : 1 if the nodes have the same date, 0 if not
*/
int sameDate(struct HeapNode a, struct HeapNode b) {
	if (a.date.year == b.date.year && a.date.month == b.date.month && a.date.day == b.date.day && a.date.hour == b.date.hour && a.date.minute == b.date.minute && a.date.second == b.date.second)
		return 1;
	return 0;
}
/*
  @description		: Deletes first occurence of an alarm node
  @var date			: Date of node we want to remove
  @var *queue       : pointer of queue we want to alter
  @var userFunction : node function pointer of node to be deleted
  @var param		: node param of function to be deleted
  @return			: NULL if node does not exist and returns the removed node if no error
*/
int removeAlarm(struct Heap* heap , struct HeapNode n ) {

	return removeNode(heap, n);
}

/*
  @description  : Calculates the next occurence of a recurring alarm node and inserts it into the queue

  @var node     : node to be attached
  @var fPointer : how long to wait before firing again
  @return       : nothing
*/
void handleRecurrentNode(struct Heap* heap , struct HeapNode prev) { // UNTESTED
	uint8_t s = (prev.date.second + prev.recurIn.second);
	uint8_t m = (prev.date.minute + s / 60 + prev.recurIn.minute);
	uint8_t h = (prev.date.hour + m / 60 + prev.recurIn.hour);
	uint8_t d = (prev.date.day + h / 24 + prev.recurIn.day);
	uint8_t mo = (prev.date.month + prev.recurIn.month + d / nDaysInMonth((prev.date.month + prev.recurIn.month), prev.date.year));
	uint16_t y = (prev.date.year + prev.recurIn.year) + mo / 12;

	struct HeapNode tmp;

       	tmp.numRecur = (prev.numRecur == -1) ? -1 : prev.numRecur - 1;
	if (tmp.numRecur == 0)
		return;

	tmp.almType = RECURRING;
	tmp.date = prev.date;
	tmp.p =prev.p;
	tmp.param = prev.param;
	tmp.recurIn = prev.recurIn;

	struct Date date = { y , mo % 12 , d % nDaysInMonth(prev.date.month , prev.date.year + REFYEAR) , h % 24 , m % 60 , s % 60 };
	addNode(date , heap , tmp.p , tmp.param , RECURRING , prev.recurIn , tmp.numRecur);
}

/*
  @description  : Pops the highest priority alarm off the stack, performs the attached action and removes and adds the node depending on node recurrance type.

  @var *queue	: Queue to be popped
  @return       : -1 if empty queue, 1 if successful
*/
int parseAlarm(struct Heap* heap) {

	struct HeapNode first, tmp;

	first = pop(heap);
	tmp = first;

	// run the alarm action
	do {
		first.p(first.param, &first);
		if (tmp.almType == RUNONCE)
			;// do nothing
		else {
			handleRecurrentNode(heap, tmp);
		}
	} while (heap->numItems > 0 && sameDate(peek(*heap), first));
	// sets the next hardware alarm
	setAlarm(tmp.date.year, tmp.date.month, tmp.date.day, tmp.date.hour, tmp.date.minute, tmp.date.second);
	return 1;
}

/*
  @description  : Performs the node comparison as specified by MinMaxHeap

  @var a		: node we are focused on (i.e. is node a larger than node b)
  @var b		: node we are comparing to
  @return       : 1 if a occurs after b chronologically, -1 if the nodes are the same and 0 otherwise
*/

int nodeCompare(struct HeapNode a, struct HeapNode b) {

	if (isAfter(a, b))
		return 1;
	if (isSame(a, b))
		return -1;

	return 0;
}