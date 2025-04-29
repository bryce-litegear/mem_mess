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


int mem_kin_start_move(kin_set_t *kset, int32_t update[])
{
    kset->step = 1;
    for(int i = 0; i < kset->set_size; i++)
    {
        kin_val_t *v = kset->val[i];
        v->start = v->current;
        v->target = update[i];
        v->current = v->start + (v->target - v->start) / kset->steps;        
    }
    return kset->step < kset->set_size ? 0: 1;
}

int mem_kin_move(kin_set_t *kset)
{
    if(kset->step < kset->steps)
    {
        kset->step++;
        for(int i = 0; i<kset->set_size; i++)
        {
            kin_val_t *v = kset->val[i];
            v->current = v->start + ((v->target - v->start) * kset->step) / kset->steps;
        }
        return 0;
    }
    return 1;
}
