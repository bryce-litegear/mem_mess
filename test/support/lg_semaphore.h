#ifndef LG_SEMAPHORE_H
#define LG_SEMAPHORE_H
#include <stdint.h>
#include <stdbool.h>

typedef struct lg_sem_t
{
    int32_t cnt;
    int32_t max;
} lg_sem_t;


#ifdef TEST
static inline void lg_sem_init(lg_sem_t *sem, int16_t init_acc, int16_t max_acc)
{sem->cnt = init_acc; sem->max = max_acc;}
static inline bool lg_sem_try_acquire(lg_sem_t *sem)
{if(--sem->cnt < 0) {sem->cnt++; return false;}return true;}
static inline bool lg_sem_release(lg_sem_t *sem)
{if(sem->cnt >= sem->max){return false;}sem->cnt++; return true;}
#else
void lg_sem_init(lg_sem_t *sem, int16_t init_acc, int16_t max_acc);
bool lg_sem_try_acquire(lg_sem_t *sem);
bool lg_sem_release(lg_sem_t *sem);
#endif

#endif // LG_SEMAPHORE_H