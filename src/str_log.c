
#include "str_log.h"

#include <stdbool.h>
#include <stddef.h>

// add str to the next spot in circular buffer overwriting the oldest
void mem_str_log_add(str_log_t *slog, char const* str)
{
    if(slog->last) // if some entries exist, add to list
    {
        str_const_t *current = slog->last;
        if(str != *slog->last) // only log different from last
        {
            slog->last++;
            if(slog->last >= &slog->log[slog->arr_len])
                slog->last = &slog->log[0];
            if(slog->last == slog->oldest) // we wrapped around, mark the loss
            {
                if(slog->lost != 0xffff) slog->lost++;
                slog->oldest++; // push oldest
                if(slog->oldest >= &slog->log[slog->arr_len])
                    slog->oldest = &slog->log[0];                
            }
            *slog->last = str;
            // if new start, establish an oldest
            if(NULL == slog->oldest) slog->oldest = slog->last;
        }
    }
    else // empty log, start new
    {
        slog->last   = &slog->log[0];
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
        *slog->oldest = NULL;   // free the space for book keeping
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
