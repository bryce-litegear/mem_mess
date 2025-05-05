/**
 * @file kinset.c
 * @author Bryce Deary (bdeary@chauvetcinema.com)
 * @brief kinematic set module, move a set of values together between two end
 *  states (start and target) use in smoothing and fader math
 * @date 2025-05-04
 * 
 * Copyright Chauvet Cinema Group (2025)
 * 
 */
#include "kinset.h"

// integer math for step in steps applied to a range with direction sensitive rounding.
static void update_current(kinset_t const *kset, kin_val_t *v)
{
    if(v->target < v->start)
    {
        v->current = v->start + (((v->target - v->start) * kset->step) - (kset->steps >> 1)) / kset->steps;
    }
    else
    {
        v->current = v->start + (((v->target - v->start) * kset->step) + (kset->steps >> 1)) / kset->steps;
    }   
}

// test if current is between start and target
static bool current_inside(kin_val_t *v)
{
    return ((v->target - v->current) + (v->current - v->start)) == (v->target-v->start);
}


// we don't check for changed data here. User should check the update
// vector before calling if no update is desired.
int kinset_update(kinset_t *kset, int32_t update[])
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

int kinset_advance(kinset_t *kset)
{
    if(kset->step < kset->steps)
    {
        kset->step++;
        for(int i = 0; i<kset->set_size; i++)
        {
            kin_val_t *v = kset->val[i];
            update_current(kset, v);
        }
        return 0;
    }
    return 1; // filter completed
}

int kinset_set_start(kinset_t *kset, int32_t const start[])
{
    bool changed = false;
    for(int i = 0; i < kset->set_size; i++)
    {
        kin_val_t *v = kset->val[i];
        if(v->start != start[i]) changed = true;
        v->start = start[i];  // set new start point
        if(! current_inside(v))
            v->current = start[i];  // set new start point
    }
    if(changed) kset->step = 0;     // if something changed, start the filter over
    return kset->step < kset->steps ? 0: 1;  // report state of the filter
}

int kinset_set_target(kinset_t *kset, int32_t const target[])
{
    bool changed = false;
    for(int i = 0; i < kset->set_size; i++)
    {
        kin_val_t *v = kset->val[i];
        if(v->target != target[i]) changed = true;
        v->target = target[i];  // set new start point
        if(! current_inside(v))
            v->current = target[i];  // set new start point
    }
    if(changed) kset->step = kset->steps;     // if something changed, start the filter over
    return kset->step < kset->steps ? 0: 1;  // report state of the filter
}

int kinset_get_current(kinset_t *kset, int32_t current[])
{
    for(int i = 0; i < kset->set_size; i++)
    {
        current[i] = kset->val[i]->current;
    }
    return kset->step < kset->steps ? 0: 1;  // report if current is target
}

int mem_kin_move(kinset_t *kset, uint16_t step)
{
    if(step <= kset->steps)
    {
        kset->step = step;
        for(int i = 0; i<kset->set_size; i++)
        {
            kin_val_t *v = kset->val[i];
            update_current(kset, v);
        }
        return 0;
    }
    return 1; // filter completed

}