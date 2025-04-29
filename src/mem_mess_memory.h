
#ifndef MEM_MESS_MEMORY_H
#define MEM_MESS_MEMORY_H
#include <stdint.h>

typedef char const * str_const_t;
typedef struct str_log_t
{
    str_const_t *last;  // last use
    str_const_t *next;  // next location
    uint16_t arr_len;   // 
    str_const_t log[];
} str_log_t;

/**
 * @brief Add a new (different) string to the log
 * 
 * @param log pointer to the log structure
 * @param str the string const pointer to add
 */
void mem_log_str(str_log_t *log, str_const_t str);

/**
 * @brief pop the oldest string from the log
 * 
 * @param log pointer to the log structure
 * @return str_const_t the pointer, removed from the log
 */
str_const_t mem_log_str_get(str_log_t *log);

#define STR_LOG_DEF(name, size) \
str_const_t name ## _slog[size]; \
str_const_t *next_ ## name = name ## _slog; \
void add_ ## name(str_const_t str)  \
{   \
    if(*next_ ## name == NULL) *next_ ## name = str; \
    /* not a repeat, write it */ \
    else if(*next_ ## name != str) \
    {   \
        next_ ## name++;  \
        if(next_ ## name >= &name ## _slog) next_ ## name = name ## _slog; \
        *next_ ## name = str; \
    } \
}


#define STR_LOG_DEC(name, size) \
extern str_const_t name ## _slog[ size ]; \
void add_ ## name(str_const_t str)


/*  Move Set functions
    Requirement is to move a set of values from one state to another
    over a series of steps such that all members arrive at the terminal 
    state at the same step. 
    The total number of states is at least two (start and end) with some 
    number of additional states from 0 to n in-between. 
    The set maintains two overall states, current and target and will 
    periodically advance as long as current does not equal target.
    Integer math issues occur when change in value is small relative to
    number of steps.

    In Int we reorder  new = current_c + ((target_c - current_c) * step))/steps

*/
typedef struct kin_val_t
{
    int32_t current;
    int32_t start;
    int32_t target;
} kin_val_t;

typedef struct kin_set_t
{
    uint16_t const set_size;
    uint16_t step;
    int32_t steps;
    kin_val_t *val[];
} kin_set_t;

/**
 * @brief apply a new data vector (set) to the kin_set_t object
 * 
 * @param kset the data set object with internally constant length
 * @param update a vactor of the same length as kset
 * @return int 0 for normal operation, positive if represents no change
 */
int mem_kin_start_move(kin_set_t *kset, int32_t update[]);

/**
 * @brief cycle the kin_set_t object, advance it to next state or
 *      if at terminal state do nothing.
 * 
 * @param kset 
 * @return int 0 for normal, positive when exhausted 
 */
int mem_kin_move(kin_set_t *kset);

#define KIN_SET_SIZE(_name) \
    (_name ## _set.set_size)

#define KIN_SET_STEPS(_name) \
    (_name ## _set.steps)    

// helper to define kinematic set
#define DEF_KIN_SET(_name, _size, _steps) \
kin_set_t _name ## _set ={.set_size = _size, .steps = _steps}; \
/* add static inits of the kin_val_t pointers */ \
kin_val_t* name ## _values[ _size ] = \


// helper to declare kinematic set
#define DEC_KIN_SET(_name, _size) \
extern kin_set_t _name ## _set; \
extern kin_val_t* name ## _values[ _size ] \


#endif // MEM_MESS_MEMORY_H
