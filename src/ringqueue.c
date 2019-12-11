#include <stdio.h>
#include <stdlib.h>

#include <ringqueue.h>

struct _ringqueue {
	size_t capacity; // max capacity
	size_t size;     // count of queue elements
	size_t head;     // position of first element
	// size_t tail;  // position after last element
	void **data;
};

ringqueue_t ringqueue_new() { return malloc(sizeof(struct _ringqueue)); }

void ringqueue_init(ringqueue_t queue, size_t capacity) {
	queue->capacity = capacity;
	queue->head = 0;
	// queue->tail = 1;
	queue->size = 0;
	queue->data = malloc(sizeof(queue->data) * capacity);
}

void ringqueue_free(ringqueue_t queue) { free(queue->data); }

size_t ringqueue_size(ringqueue_t queue) { return queue->size; }

int ringqueue_full(ringqueue_t queue) {
	return (queue->size == queue->capacity);
}

int ringqueue_empty(ringqueue_t queue) { return (queue->size == 0); }

int ringqueue_push(ringqueue_t queue, void *data) {
	if (ringqueue_full(queue)) {
		return -1;
	} else {
		size_t index = queue->head + queue->size++;
		if (index >= queue->capacity) {
			index = 0;
		}
		queue->data[index] = data;
		//fprintf(stderr, "push at %zu, head: %zu, size: %zu, capacity: %zu\n", index, queue->head, queue->size, queue->capacity);
		return 0;
	}
}

void *ringqueue_popfront(ringqueue_t queue) {
	if (ringqueue_empty(queue)) {
		return NULL;
	} else {
		/* FIFO implementation */
		void *element = queue->data[queue->head++];
		queue->size--;
		if (queue->head == queue->capacity) {
			queue->head = 0;
		}
		//fprintf(stderr, "popfront, head: %zu, size: %zu, capacity: %zu\n", queue->head, queue->size, queue->capacity);
		return element;
	}
}

void *ringqueue_popback(ringqueue_t queue) {
	if (ringqueue_empty(queue)) {
		return NULL;
	} else {
		/* LIFO implementation */
		size_t index = queue->head + queue->size - 1;
		if (index >= queue->capacity) {
			index -= queue->capacity;
		}
		queue->size--;
		//fprintf(stderr, "popback, head: %zu, size: %zu, capacity: %zu\n", queue->head, queue->size, queue->capacity);
		return queue->data[index];
	}
}
