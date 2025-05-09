/**
 * @file p2queue.c
 * @author Bryce Deary (bryce.deary@litegear.com)
 * @brief byte queues with length as powers of two from 2^1 to 2^31
 * 
 * @date 2025-02-12
 * 
 * @copyright Copyright LightGear (2025)
 * 
 */
#include <stddef.h>
#include "p2queue.h"
#include "lg_semaphore.h"


p2queue_t p2q_bind(uint8_t *queue_space, uint32_t queue_space_length)
{
    if((queue_space_length & (queue_space_length-1)) | queue_space_length <= 1) return (p2queue_t){.mask=0};
    return (p2queue_t){.sem = {.cnt=1, .max=1}, .queue=queue_space, .mask=queue_space_length-1};
}

// add to queue return false if no room (not added)
bool p2q_in(p2queue_t *p2q, uint8_t val)
{
    if(((p2q->out - p2q->in) & p2q->mask) == 1) 
    {   
        return false;
    }
    if(p2q->queue)
    {
        p2q->queue[p2q->in++] = val;
        p2q->in &= p2q->mask;
        return true;
    }
    return false;
}

// return while data available
bool p2q_out(p2queue_t *p2q, uint8_t *val)
{
    if(p2q->in != p2q->out) // data in queue
    {
        *val = p2q->queue[p2q->out++];
        p2q->out &= p2q->mask;
        return true;
    }
    return false;
}

// return available space in queue
uint32_t p2q_space(p2queue_t *p2q)
{
    return ((p2q->out - p2q->in) - 1) & p2q->mask;
}

// destroy the queue, set to empty
void p2q_reset(p2queue_t *p2q)
{
    p2q->out = p2q->in; 
}

/**
 * @brief copy buffer to output stream protected by semaphore, return false
 *  if busy or no space.
 * 
 * @param p2    - pointer to the queue
 * @param buf 
 * @param len 
 * @return true 
 * @return false 
 */
bool p2q_in_buf(p2queue_t *p2, uint8_t *buf, uint32_t len)
{
    static lg_sem_t *sem = NULL;
    if(!sem)
    {
        static lg_sem_t local_sem;
        lg_sem_init(&local_sem, 1, 1);
        sem = &local_sem;
    }

    if(lg_sem_try_acquire(sem))
    {
        if(len <= p2q_space(p2))
        {
            while(len--) p2q_in(p2, *buf++);
            lg_sem_release(sem);
            return true;
        }
        lg_sem_release(sem);
        return false;
    }
    return false;
}