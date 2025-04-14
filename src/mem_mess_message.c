/**
 * @file mem_mess_message.c
 * @author Bryce Deary (bryce.deary@litegear.com)
 * @brief 
 * @date 2025-04-04
 * 
 * @copyright Copyright LiteGear (2025)
 * 
 */

#include "mem_mess_message.h"
#include <stddef.h>
#include <string.h>

mem_mess_scheduler_t mem_mess_scheduler = NULL;
mem_mess_map_t mem_mess_map = NULL; // global mapper if not rec specific

/**
 * @brief main copy function, called by default if no user immediate is provided 
 *      for non-getter messages.
 * 
 * @param mesrec - pointer to const 
 * @param payload 
 * @param pl_size - actual size read or maximum possible 
 * @return int 
 */
int mem_mess_setter_copy(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size)
{
    if(mesrec) // need a message description to process, payload is optional
    {   
        bool changed = false;                   // set when copied data is changed
        uint8_t*mem_start = mesrec->mem_obj_pnt;    // initial copy target from message description
        uint32_t index = 0;                     // optional index
        uint32_t pl_len = mesrec->mem_instance_len;  // initial payload length
        if(payload) // if we have a payload, process and copy from it to memory
        {
            if(mesrec->indexed32) // payload has a 32bit sized address
            {
                memcpy(&index, &payload[mesrec->index_offset], 4);
                if(mesrec->mapper) index = mesrec->mapper(index);
                else if (mem_mess_map) index = mem_mess_map(index); // global mapper
            }
            else if(mesrec->indexed8) // payload has a byte size address
            {
                memcpy(&index, &payload[mesrec->index_offset], 1);
                if(mesrec->mapper) index = mesrec->mapper(index);
                else if (mem_mess_map) index = mem_mess_map(index);
            }

            if(mesrec->len32)  // payload was variable length with 32bit length value
            {
                pl_len = 0;
                memcpy(&pl_len, &payload[mesrec->len_offset], 4);
            }
            else if(mesrec->len8) // payload was variable length with 8bit length value
            {
                pl_len = 0;
                memcpy(&pl_len, &payload[mesrec->len_offset], 1);
            }

            // if final payload length is non zero check for change and copy the data
            if(pl_len)
            {
                mem_start +=  index * pl_len;
                // if payload fits within the object storage space
                if( &mem_start[pl_len] <= &((uint8_t*)mesrec->mem_obj_pnt)[mesrec->mem_obj_len]) 
                {
                    uint16_t pl_start = mesrec->pl_offset;
                    changed = memcmp(mem_start, &payload[pl_start], pl_len);
                    if( changed ) memcpy(mem_start, &payload[pl_start], pl_len);
                }
            }

            // if specific change or default behaviour, schedule the background function.
            // typically if this function is running in an isr, the scheduler would be the correct path
            // if this function is already running in background, it is ok to just call the function.
            if( ! mesrec->bg_on_change || changed)
            {
                if(mesrec->background) // if background function exists, schedule or do direct call
                {
                    if(mem_mess_scheduler) // if user defined a scheduler
                    {
                        mem_mess_scheduler(mesrec->background, mem_start);
                    }
                    else // if no scheduler, just call the function
                    {
                        mesrec->background(mem_start);
                    }
                }
            }
        }
    }
    return 0;
}


/**
 * @brief 
 * 
 * @param mesrec 
 * @param payload 
 * @param pl_size 
 * @return int 
 */
int mem_mess_getter_copy(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size)
{
    if(mesrec && payload)
    {   
        bool changed = false;
        uint8_t*mem_start = mesrec->mem_obj_pnt;
        uint32_t index = 0;
        uint32_t pl_start = 0;
        uint32_t pl_len = mesrec->mem_obj_len;
        if(payload)
        {
            if(mesrec->indexed32)
            {
                memcpy(&index, &payload[mesrec->index_offset], 4);
                if(mesrec->mapper) index = mesrec->mapper(index);
                else if (mem_mess_map) index = mem_mess_map(index);
                pl_start = (uint32_t)mesrec->index_offset + 4;
                pl_len -= 4;
            }
            else if(mesrec->indexed8)
            {
                memcpy(&index, &payload[mesrec->index_offset], 1);
                if(mesrec->mapper) index = mesrec->mapper(index);
                else if (mem_mess_map) index = mem_mess_map(index);                
                pl_start = (uint32_t)mesrec->index_offset + 1;
                pl_len -= 1;
            }

            if(mesrec->len32)
            {
                pl_len = 0;
                memcpy(&pl_len, &payload[mesrec->len_offset], 4);
                if(mesrec->len_offset + 4 > pl_start) pl_start = mesrec->len_offset + 4;
            }
            else if(mesrec->len8)
            {
                pl_len = 0;
                memcpy(&pl_len, &payload[mesrec->len_offset], 1);
                if(mesrec->len_offset + 1 > pl_start) pl_start = mesrec->len_offset + 1;
            }

            if(pl_len)
            {
                mem_start +=  index * pl_len;
                changed = memcmp(mem_start, &payload[pl_start], pl_len);
                if( changed ) memcpy(mem_start, &payload[pl_start], pl_len);
            }

            if( ! mesrec->bg_on_change || changed)
            {
                if(mem_mess_scheduler)
                    mem_mess_scheduler(mesrec->background, mem_start);
            }
        }
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param mesrec - pointer to the message definition record
 * @param payload - pointer to the payload
 * @return int 
 */

/**
 * @brief main process payload function, apply the message record to the payload
 * 
 * @param mesrec - pointer to the message definition record
 * @param payload - pointer to a payload buffer
 * @param pl_size - the real or maximum size of payload used in preventing overflow
 * @return int negative if error was discovered, 0 for completed Ok or positive number of
 *      byte written to payload if mesrec was a getter message.
 */
int mem_mess_process(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size)
{
    if(mesrec)
    {
        // if there is an immediate function, user takes all control, just call.
        if(mesrec->immediate)
        {
            return  mesrec->immediate(mesrec, payload, pl_size);
        }

        if(mesrec->is_getter)
        {
            return mem_mess_getter_copy(mesrec, payload, pl_size);
        }
        else
        {
            return mem_mess_setter_copy(mesrec, payload, pl_size);
        }
    }
    else return -1;

}


