/**
 * @file mem_mess_memory.c
 * @author Bryce Deary (bryce.deary@litegear.com)
 * @brief 
 * @date 2025-04-29
 * 
 * @copyright Copyright LiteGear (2025)
 * 
 */

#include "mem_mess_memory.h"
#include <stdbool.h>
#include <stddef.h>

// add str to the next spot in circular buffer overwriting the oldest
void mem_str_log_add(str_log_t *slog, char const* str)
{
    str_const_t *current = slog->last;
    if(slog->last)
    {
        if(str != *slog->last) // only log different from last
        {
            slog->last++;
            if(slog->last >= &slog->log[slog->arr_len])
                slog->last = &slog->log[0];
            if(slog->last == slog->oldest)
            {
                slog->oldest++;
                if(slog->oldest >= &slog->log[slog->arr_len])
                    slog->oldest = &slog->log[0];                
            }
            *slog->last = str;
            if(NULL == slog->oldest) slog->oldest = slog->last;
        }
    }
    else
    {
        slog->last = &slog->log[0];
        slog->oldest = &slog->log[0];
        *slog->last = str;
    }
}

char const *mem_str_log_get(str_log_t *slog)
{
    char const *rtn = NULL;
    if(slog->oldest) // values in the buffer
    {
        rtn = *slog->oldest;    // pop the pointer
        *slog->oldest = NULL;   // free the space
        if(slog->oldest == slog->last) // last entry?
        {
            slog->oldest = NULL;    // reset the buffer
            slog->last = NULL;      
        }
        else  // advance oldest
        {
            slog->oldest++;
            if(slog->oldest >= &slog->log[slog->arr_len])
                slog->oldest = &slog->log[0];      
        }
    }
    return rtn;
}




// we don't check for changed data here. User should check the update
// vector before calling if no update is desired.
int mem_kinset_update(kinset_t *kset, int32_t update[])
{
    bool changed = false;
    for(int i = 0; i < kset->set_size; i++)
    {
        kin_val_t *v = kset->val[i];
        if(v->target != update[i]) changed = true;
        v->target = update[i];  // set new target
        v->start = v->current;  // set new start point
        if(v->target < v->start)        
            v->current = v->start + ((v->target - v->start) - (kset->steps >> 1)) / kset->steps;        
        else
            v->current = v->start + ((v->target - v->start) + (kset->steps >> 1)) / kset->steps;        
    }
    if(changed) kset->step = 1;     // if something changed, start the filter over
    return kset->step < kset->steps ? 0: 1;  // report state of the filter
}

int mem_kin_advance(kinset_t *kset)
{
    if(kset->step < kset->steps)
    {
        kset->step++;
        for(int i = 0; i<kset->set_size; i++)
        {
            kin_val_t *v = kset->val[i];
            if(v->target < v->start)
            {
                v->current = v->start + (((v->target - v->start) * kset->step) - (kset->steps >> 1)) / kset->steps;
            }
            else
            {
                v->current = v->start + (((v->target - v->start) * kset->step) + (kset->steps >> 1)) / kset->steps;
            }
        }
        return 0;
    }
    return 1; // filter completed
}

int mem_kinset_set_start(kinset_t *kset, int32_t const start[])
{
    bool changed = false;
    for(int i = 0; i < kset->set_size; i++)
    {
        kin_val_t *v = kset->val[i];
        if(v->start != start[i]) changed = true;
        v->start = start[i];  // set new start point
        v->current = start[i];  // set new start point
    }
    if(changed) kset->step = 0;     // if something changed, start the filter over
    return kset->step < kset->steps ? 0: 1;  // report state of the filter
}

int mem_kinset_set_target(kinset_t *kset, int32_t const target[])
{
    bool changed = false;
    for(int i = 0; i < kset->set_size; i++)
    {
        kin_val_t *v = kset->val[i];
        if(v->target != target[i]) changed = true;
        v->target = target[i];  // set new start point
        v->current = target[i];  // set new start point
    }
    if(changed) kset->step = kset->steps;     // if something changed, start the filter over
    return kset->step < kset->steps ? 0: 1;  // report state of the filter
}

int mem_kinset_get_current(kinset_t *kset, int32_t current[])
{

}

int mem_kin_move(kinset_t *kset, uint16_t step)
{

}


