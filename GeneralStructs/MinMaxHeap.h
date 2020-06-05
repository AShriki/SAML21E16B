#ifndef _MMHEAP_
#define _MMHEAP_

#include <stdint.h>

#ifndef  _USERRTC_
#include "Drivers/userRTC.h"
#endif 

#ifndef  _USERTC_
#include "Drivers/userTC.h"
#endif

/* - - - USER DEFINED FIELDS - - - */

#define MAXNODES 64 // The maximum number of items in the array

struct HeapNode {

	struct Date date;

	uint8_t param;
	alarmAction p;

	enum AlarmType almType;
	struct Date recurIn;
	int numRecur;
};

/* - - - END USER DEFINED FIELDS - - - */

// The User must implement nodeCompare to compare nodes. Output should be 1 for a percolate up or 0 for no percolate and -1 if the nodes are the same
extern int nodeCompare(struct HeapNode, struct HeapNode);

struct Heap {
	struct HeapNode heapArray[MAXNODES + 1];
	int numItems;
};

struct HeapNode peek(struct Heap);
int insert(struct Heap*, struct HeapNode);
struct HeapNode pop(struct Heap*);
int removeNode(struct Heap*, struct HeapNode);


#endif
