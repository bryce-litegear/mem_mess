// These are mockable functions for unit testing
#ifndef USER_FILES_H
#define USER_FILES_H

#include <stdint.h>
#include "mem_mess_message.h"
typedef struct mem_mess_record_t mem_mess_record_t;

// template for user supplied mapping function that remaps index in payloads
// typedef uint32_t (*mem_mess_map_t)(uint32_t);
uint32_t my_mem_mess_map(uint32_t idx);

// user supplied mapping function for remapping indexs from messages to memory images.
//typedef void (*mem_mess_background_t)(void *memadd);
void my_mem_mess_background(void *memadd);

// template for user supplied function that schedules the background functions
// when messages are processed in isr rather than background.
//typedef void (*mem_mess_scheduler_t)(mem_mess_background_t bg_func, void *memadd);
void my_mem_mess_scheduler(void (*func)(void *context), void *memadd);

// for immediate action on data, bypass the default copy
//typedef int (*mem_mess_immediate_t)(mem_mess_record_t *mesrec, uint8_t *payload);
int my_mem_mess_immediate(mem_mess_record_t *mesrec, uint8_t *payload);


#endif


