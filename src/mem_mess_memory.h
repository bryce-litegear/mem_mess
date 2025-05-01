
#ifndef MEM_MESS_MEMORY_H
#define MEM_MESS_MEMORY_H
#include <stdint.h>

/**
 * @brief alias for pointer to string const
 */
typedef char const * str_const_t;

/**
 * @brief the log is just a collection of pointers to const string
 */
typedef struct str_log_t
{
    str_const_t *oldest;  // last use
    str_const_t *last;  // next location
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
str_log_t name ## _slog = {.arr_len = size}; \
str_const_t name ## _slog_store[size]; \

#define STR_LOG_DEC(name) \
extern str_log_t name ## _slog

void mem_str_log_add(str_log_t *slog, char const* str);
char const *mem_str_log_get(str_log_t *slog);


/*  Move Set functions, This can be used to do smooth transition like 
    DMX smoothing or any intensity or color transitions.
    Requirement is to move a set of values from one state to another
    over a series of steps such that all members arrive at the terminal 
    state at the same step. 
    The total number of states is at least two (start and end) with some 
    number of additional states from 0 to n in-between. 
    The set maintains two overall states, current and target and will 
    periodically advance as long as current does not equal target.
    Integer math issues occur when change in value is small relative to
    number of steps.

    Calling the update function applies a new set of target values to the
    set object, setting up new start and end points and resetting the step
    counter to 1.

    Calling the advance function moves the set toward the target values by
    one step.  Calling advance after reaching the target has no effect.


    In Integer math  new = current_c + ((target_c - current_c) * step))/steps

*/
/**
 * @brief The structure to hold one member of a set. This structure may be 
 * embedded inside other structures and is pointed to by the set object (below)
 */
typedef struct kin_val_t
{
    int32_t start;      // a first or start point
    int32_t target;     // a second or target point
    int32_t current;    // the current calculated intermediate state
} kin_val_t;

/**
 * @brief The set object structure that describes the set and points to all the 
 * member objects. It maintains the state of the set as it advances from 
 * start to target.
 */
typedef struct kinset_t
{
    uint16_t const set_size;    // immutable size of the set
    uint16_t step;              // the current step (65535 at 3ms update is 196 seconds)
    int32_t steps;              // the number of steps to move over, (32bit just for math and alignment, max is really 65535)
    kin_val_t *val[];           // array of kin_val_t *val[set_size] immediately following this struct in memory
} kinset_t;

// get the set size by name (useful in iterating over set)
#define GET_KINSET_SIZE(_name) \
    (_name ## _set.set_size)

// get the current number of steps in the named set
#define GET_KINSET_STEPS(_name) \
    (_name ## _set.steps)    

// Set the number of steps in the named set, setting this while processing 
// a sequence may cause a noticeable jump depending on current state.
#define SET_KINSET_STEPS(_name, _steps) \
    do{_name ## _set.steps = (_steps & 0xffff);}while(0)

// helper to define kinematic set
#define DEF_KINSET(_name, _size, _steps) \
kinset_t _name ## _set ={.set_size = _size, .steps = _steps}; \
/* add static inits of the kin_val_t pointers */ \
kin_val_t* name ## _values[ _size ] = \


// helper to declare kinematic set
#define DEC_KINSET(_name, _size) \
extern kinset_t _name ## _set; \
extern kin_val_t* name ## _values[ _size ] \

/**
 * @brief apply a new data vector (set) to the kinset_t object
 * 
 * @param kset the data set object with internally constant length
 * @param update a vactor of the same length as kset
 * @return int 0 for normal operation, positive if represents no change
 */
int mem_kinset_update(kinset_t *kset, int32_t update[]);

int mem_kinset_update(kinset_t *kset, int32_t update[]);

/**
 * @brief cycle the kinset_t object, advance it to next state or
 *      if at terminal state do nothing.
 * 
 * @param kset 
 * @return int 0 for normal, positive when exhausted 
 */
int mem_kin_advance(kinset_t *kset);




#endif // MEM_MESS_MEMORY_H
