/**
 * @file kinset.h
 * @author Bryce Deary (bdeary@chauvetcinema.com)
 * @brief kinematic set module, move a set of values proportionally
 *  between two values.  Useful in building smoothing filters and
 *  faders.
 * @date 2025-05-01
 * 
 * @copyright Copyright LiteGear (2025)
 * 
 */

#ifndef KINSET_H
#define KINSET_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*  Move Set functions, This can be used to do smooth transition like 
    DMX smoothing or any intensity or color transitions.
    Requirement is to move a set of values from one state to another
    over a series of steps such that all members arrive at the terminal 
    state at the same step. 
    The total number of states is at least two (start and end) with some 
    number of additional states from 0 to n in-between. 
    The set maintains three states for each member in the set (start, target, current) 
    and three state variables for the entre set (size, steps, step). 

    functions exist to periodically advance as long as current does not equal target,
    and to set arbitrary step point.

    the update function sets a new target and replaces the start with the current


    Design issues for integer math require scaling rather than sums and division a
    division per set member is required when calculating a new state.

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
 * @brief apply a new data vector (set) to the kinset_t object, 
 *  advance current to start and set new targets
 * 
 * @param kset the data set object with internally constant length
 * @param update a vactor of the same length as kset
 * @return int 0 for normal operation, positive if represents no change
 */
int kinset_update(kinset_t *kset, int32_t update[]);

int kinset_set_start(kinset_t *kset, int32_t const update[]);
int kinset_set_target(kinset_t *kset, int32_t const update[]);
int kinset_get_current(kinset_t *kset, int32_t current[]);

/**
 * @brief cycle the kinset_t object, advance it to next state or
 *      if at terminal state do nothing.
 * 
 * @param kset 
 * @return int 0 for normal, positive when exhausted 
 */
int kinset_advance(kinset_t *kset);

#endif // KINSET_H
