#ifndef _RINGQUEUE_H_
#define _RINGQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

struct _ringqueue;
typedef struct _ringqueue *ringqueue_t;

ringqueue_t ringqueue_new(size_t capacity);
void ringqueue_init(ringqueue_t queue, size_t capacity);
void ringqueue_free(ringqueue_t queue);
size_t ringqueue_size(ringqueue_t queue);
int ringqueue_full(ringqueue_t queue);
int ringqueue_empty(ringqueue_t queue);
int ringqueue_push(ringqueue_t queue, void *data);
void *ringqueue_popfront(ringqueue_t queue);
void *ringqueue_popback(ringqueue_t queue);

#ifdef __cplusplus
}
#endif

#endif /* _RINGQUEUE_H_ */
