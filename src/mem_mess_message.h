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
#include <stdlib.h> // for ldiv_t

#ifdef TEST
#define STATIC
#else
#define STATIC static
#endif


// we pass the target pointer to background process, this macro will validate
// that pointer as an member of an array of structure. So the array must be in the 
// scope of the background function.
#define MEM_MESS_VALIDATE_PTR(ARR, INSTANCE) mm_get_element_of((uint8_t*)ARR, sizeof ARR, sizeof ARR[0], (uint8_t*)(INSTANCE))
static inline int32_t mm_get_element_of(uint8_t *arr, uint32_t arr_size, int32_t element_size, uint8_t *element)
{
    ldiv_t res = ldiv((int32_t)(element - arr), element_size);
    bool in_range = element < &arr[arr_size] && element >= arr;
    return res.rem || !in_range ? -1: res.quot;  // if non zero remainder then not a valid pointer to array element
}

// forward reference
typedef struct mem_mess_record_t mem_mess_record_t;

// template for user supplied mapping function that remaps index in payloads
typedef uint32_t (*mem_mess_map_t)(uint32_t);

// user supplied mapping function for remapping indexs from messages to memory images.
extern mem_mess_map_t mem_mess_map;


typedef void (*mem_mess_background_t)(void *mem_inst);


// template for user supplied function that schedules the background functions
// when messages are processed in isr rather than background.
typedef void (*mem_mess_scheduler_t)(mem_mess_background_t bg_func, void *memadd);

// user supplied scheduler function 
extern mem_mess_scheduler_t mem_mess_scheduler;

// for immediate action on data, bypass the default copy
typedef int (*mem_mess_immediate_t)(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size);

// the default processing functions, available for user processing in user supplied immediate functions.
int mem_mess_setter_copy(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size);
int mem_mess_getter_copy(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size);

// helper macros for building table, memory objects are either single or array and sizes are recorded for 
// proper error checking.
#define MM_OBJ_ARR(arr) .mem_obj_pnt = arr, .mem_obj_len = sizeof arr, .mem_instance_len = sizeof arr[0]
#define MM_OBJ(obj) .mem_obj_pnt = &obj, .mem_obj_len = sizeof obj, .mem_instance_len = sizeof obj
#define MM_STRS(arr_of_str) .mem_obj_pnt = arr_of_str, .mem_obj_len = sizeof arr_of_str, .mem_instance_len = sizeof arr_of_str[0]

// message definition record, maps message token to memory area and processing options
// optional features assume that 0 or NULL is disabled
typedef struct mem_mess_record_t
{
    mem_mess_map_t mapper;              // optional index map function
    mem_mess_immediate_t immediate;     // optional user provided override function
    mem_mess_background_t background;   // optional user background processor of instance data
    void *mem_obj_pnt;                  // root pointer
    uint32_t mem_obj_len;               // root length
    uint16_t mem_instance_len;          // instance len if subscripted, else equal to mem_obj_len
    uint16_t token;                     // the sortable unique token that identifies the message (message_no)
    uint16_t getter_token;              // for getters, the response token
    uint16_t pl_offset;                 // position in pl of instance data
    struct {
        uint16_t indexed8: 1;       // byte in payload is an index into array of data structs
        uint16_t indexed32: 1;      // 4 bytes in payload are index
        uint16_t len8: 1;           // byte in payload is a length
        uint16_t len32: 1;          // 4 bytes in payload is a length
        uint16_t is_getter: 1;      // the message is a getter (causes a new message to be built)
        uint16_t bg_on_change: 1;   // only call background function on change
        uint16_t res: 10;           // reserved
    };
    uint8_t index_offset;   // offset of index within the pl_offset
    uint8_t len_offset;     // offset of len withing pl_offset

} mem_mess_record_t;


/**
 * @brief main caller for processing payloads against message descriptions
 * 
 * @param mesrec 
 * @param payload 
 * @param pl_size 
 * @return int 
 */
int mem_mess_process(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size);




#endif // MEM_MESS_MESSAGE_H
