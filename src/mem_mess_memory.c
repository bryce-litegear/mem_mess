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

// we don't check for changed data here. User should check the update
// vector before calling if no update is desired.
int mem_kin_set_update(kin_set_t *kset, int32_t update[])
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

int mem_kin_advance(kin_set_t *kset)
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
