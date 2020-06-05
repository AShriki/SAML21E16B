#include "MinMaxHeap.h"
/*
	Created by Avshalom Shriki
	E-mail: Av.shriki@gmail.com

	This file implements an array based 2-heap which is sorted based on a user implemented nodeCompare on a user implemented structure "HeapNode"
	
	See the .h file to change the type of heap to maxheap.

	The first node is stored at location 1
*/

/*
	@description : Swaps two indecies in a heap

	@var h       : a pointer to a heap
	@var m       : position to swap
	@var n       : position to swap
	@return      : nothing
*/
void swap(struct Heap* h, int m, int n) {// swap m and n
	struct HeapNode tmp;
	tmp = h->heapArray[m];
	h->heapArray[m] = h->heapArray[n];
	h->heapArray[n] = tmp;
}
/*
	@description : Returns the node of highest/lowest priority without removing it

	@var h       : a pointer to a heap
	@return      : a HeapNode of highest/lowest priority
*/
struct HeapNode peek(struct Heap h) {
	return h.heapArray[1];
}
/*
	@description : Inserts a HeapNode into the heap using perc up

	@var h       : a pointer to a heap
	@var n       : node to be inserted
	@return      : 0 if heap is full, 1 if successfully inserted a new element
*/
int insert(struct Heap* h, struct HeapNode n) {
	if (h->numItems == MAXNODES)
		return 0;
	int index = ++h->numItems ;
	h->heapArray[index] = n;
	// perc up
	if (h->numItems - 1) {
		// if parent occurs after child, swap and continue
		while (index / 2 > 0 && nodeCompare(h->heapArray[index / 2], h->heapArray[index]) == 1) {
			swap(h, index, index / 2);
			index /= 2;
		}
	}
	return 1;
}
/*
	@description : Removes the highest/lowest priority node

	@var h       : a pointer to a heap
	@return      : the HeapNode of highest/lowest priority
*/
struct HeapNode pop(struct Heap* h) {
	if (!h->numItems)
		return h->heapArray[0];
	struct HeapNode n = h->heapArray[1];
	h->heapArray[1] = h->heapArray[h->numItems--];
	h->heapArray[h->numItems + 1] = h->heapArray[0];
	int i = 1,c = 1;
	// perc down
	c = nodeCompare(h->heapArray[i * 2], h->heapArray[i * 2 + 1]) == 1 ? ((i * 2 + 1 <= h->numItems) ? i * 2 + 1 : -1) : ((i * 2 <= h->numItems) ? i * 2 : -1);

	while (c != -1) {
		// does parent need to swap with child?
		if (nodeCompare(h->heapArray[i], h->heapArray[c]) == 1) {
			swap(h, i, c);
			i = c;
		}
		else
			break;

		c = nodeCompare(h->heapArray[i * 2], h->heapArray[i * 2 + 1]) == 1 ? ((i * 2 + 1 <= h->numItems) ? i * 2 + 1 : -1) : ((i * 2 <= h->numItems) ? i * 2 : -1);
	}

	return n;
}
/*
	@description : Searches the heap and removes a node

	@var h       : a pointer to a heap
	@var n       : node to be removed
	@return      : 1 on success, 0 if the node is not present and -1 if the list is empty
*/
int removeNode(struct Heap* h, struct HeapNode n) {
	if (!h->numItems)
		return -1;
	int found = 0;
	int i = 1;

	for (; i <= h->numItems; i++) {
		if (nodeCompare(h->heapArray[i], n) == -1) {
			found = 1;
			break;
		}
	}

	if (i > h->numItems)
		return 0;

	h->heapArray[i] = h->heapArray[h->numItems--];
	h->heapArray[h->numItems + 1] = h->heapArray[0];
	int c = 0;

	// determine direction to percolate
	// if i*2 child is after i*2+1 child and that child is in the array then return the index of the smaller child, else return -1
	// We are choosing the smallest child that exists
	c = nodeCompare(h->heapArray[i * 2], h->heapArray[i * 2 + 1]) == 1 ? ((i * 2 + 1 <= h->numItems) ? i * 2 + 1 : -1) : ((i * 2 <= h->numItems) ? i * 2 : -1);
	// if the child doesn't exist and we can perc up
	if (c == -1) {
		//perc up
		while (i / 2 > 0 && nodeCompare(h->heapArray[i / 2], h->heapArray[i]) == 1) {
			swap(h, i, i / 2);
			i /= 2;
		}
	}
	else {

		while (c != -1) {

			// does parent need to swap with child?
			if (nodeCompare(h->heapArray[i], h->heapArray[c]) == 1) {
				swap(h, i, c);
				i = c;
			}
			else
				break;

			c = nodeCompare(h->heapArray[i * 2], h->heapArray[i * 2 + 1]) == 1 ? ((i * 2 + 1 <= h->numItems) ? i * 2 + 1 : -1) : ((i * 2 <= h->numItems) ? i * 2 : -1);
		}
	}

	return found;
}


