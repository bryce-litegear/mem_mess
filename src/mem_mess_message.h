/**
 * @file mem_mess_message.h
 * @author Bryce Deary (bryce.deary@litegear.com)
 * @brief 
 * @date 2025-04-04
 * 
 * @copyright Copyright LiteGear (2025)
 * @details message to memory definition maps a byte stream message to a memory
 *  structure with a set of optional special conditions like indexing or variable
 *  length.  The basic flow is, message is processed after a unique token has been 
 *  discovered, token is looked up to find mem_mess_record_t structure which contains 
 *  pointers to associated memory locations. message payload is processed according 
 *  to the mem_mess_record, which generally consists of do a memcpy from the payload
 *  buffer to memory, or memory to a payload buffer in the case of a get message.
 *  Get messages may have a second token to define a return message if the get 
 *  message is complex or shares some common response message.
 *  memcpy will always be used to avoid any alignment issues. 
 *  Finally at least two optional functions are available, one immediate and one 
 *  scheduled it optional conditions like change-of-data detection.
 * 
 */
#ifndef MEM_MESS_MESSAGE_H
#define MEM_MESS_MESSAGE_H

#ifndef ARR_LEN
#define ARR_LEN(arr) (sizeof arr / sizeof arr[0])
#endif

#include <stdint.h>
#include <stdbool.h>

#ifdef TEST
#define STATIC
#else
#define STATIC static
#endif

//#define MEM_REC_IS_SORTED

typedef struct mem_mess_record_t mem_mess_record_t;

// any special mapping function for indexing
typedef uint32_t (*mem_mess_map_t)(uint32_t);
typedef void (*mem_mess_background_t)(void *memadd);

typedef void (*mem_mess_scheduler_t)(mem_mess_background_t bg_func, void *memadd);
extern mem_mess_scheduler_t mem_mess_scheduler;

// for immediate action on data, bypass the default copy
typedef int (*mem_mess_immediate_t)(mem_mess_record_t *mesrec, uint8_t *payload);

int mem_mess_setter_copy(mem_mess_record_t const *mesrec, uint8_t *payload);
int mem_mess_getter_copy(mem_mess_record_t const *mesrec, uint8_t *payload);


// optional features assume that 0 or NULL is disabled

typedef struct mem_mess_record_t
{
    mem_mess_map_t mapper;
    mem_mess_immediate_t immediate;
    mem_mess_background_t background;
    void *mem_pnt;
    uint16_t mem_struct_len;
    uint16_t token;     // the sortable unique token that identifies the message (message_no)
    struct {
        uint16_t byte_indexed: 1;   // byte in payload is an index into array of data structs
        uint16_t word_indexed: 1;   // 4 bytes in payload are index

        uint16_t byte_length: 1;    // byte in payload is a length
        uint16_t word_length: 1;    // 4 bytes in payload is a length
        uint16_t is_getter: 1;      // the message is a getter (causes a new message to be built)
        uint16_t bg_on_change: 1;   // only call background function on change

        uint16_t index_offset: 4;   // index must start in the first 16 bytes of the payload
        uint16_t len_offset: 4;     // index must start in the first 16 bytes of the payload
    };


} mem_mess_record_t;





#endif // MEM_MESS_MESSAGE_H
