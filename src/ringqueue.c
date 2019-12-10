#include <ringqueue.h>

#include <stdio.h>
#include <stdlib.h>

struct buffer {
	size_t size;  // size of data element
	size_t count; // maximal count of queue elements
	size_t head;  // position of first element
	size_t tail;
	void **data;
};

typedef struct buffer buffer_t;
