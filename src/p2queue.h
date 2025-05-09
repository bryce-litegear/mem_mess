/**
 * @file p2queue.h
 * @author Bryce Deary (bryce.deary@litegear.com)
 * @brief module to make byte queues with lengths as powers of 2
 * @date 2025-02-12
 * 
 * @copyright Copyright LightGear (2025)
 * 
 */
#ifndef P2QUEUE_H
#define P2QUEUE_H
#include <stdint.h>
#include <stdbool.h>
#include "lg_semaphore.h"

typedef struct p2queue_t
{
    lg_sem_t sem;
    uint8_t * const queue;
    uint32_t const mask;
    uint32_t in;
    uint32_t out;
} p2queue_t;

/**
 * @brief make a queue structure to manage a power 2 length queue
 * 
 * @param queue_space - pointer to a byte buffer with length with power of 2
 * @param queue_space_length power of two length of queue_space
 * @return p2queue_t a structure to manage the queue.
 */
p2queue_t p2q_bind(uint8_t *queue_space, uint32_t queue_space_length);

/**
 * @brief add a byte to a queue
 * 
 * @param p2 pointer to queue
 * @param val [in] byte to add
 * @return true - value was added
 * @return false - add was not complete
 */
bool  p2q_in(p2queue_t *p2, uint8_t val);

/**
 * @brief copy a buffer of bytes into a byte queue,
 *  do all or nothing.
 * 
 * @param p2 - pointer to queue
 * @param buf - buffer of data
 * @param len - number of byte in buffer
 * @return true - if transfer was complete
 * @return false - transfer was not performed
 */
bool  p2q_in_buf(p2queue_t *p2, uint8_t *buf, uint32_t len);

/**
 * @brief get next byte from queue.
 * 
 * @param p2 - pointer to queue
 * @param val [out] pointer to extracted byte
 * @return true - if data copied to val
 * @return false - queue was empty or error
 */
bool p2q_out(p2queue_t *p2, uint8_t *val);

/**
 * @brief reset the queue to empty
 * 
 * @param p2 pointer to the queue
 */
void p2q_reset(p2queue_t *p2);

/**
 * @brief return available space in queue
 * 
 * @param p2 - pointer to queue structure
 * @return uint32_t - number of byte available in queue
 */
uint32_t p2q_space(p2queue_t *p2);


#endif // P2QUEUE_H
