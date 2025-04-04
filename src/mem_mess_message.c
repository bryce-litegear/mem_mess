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

/**
 * @brief main copy function, called by default if no user immediate is provided 
 *      for non-getter messages.
 * 
 * @param mesrec - pointer to const 
 * @param payload 
 * @return int 
 */
int mem_mess_setter_copy(mem_mess_record_t const *mesrec, uint8_t *payload)
{
    if(mesrec)
    {   
        bool changed = false;
        uint8_t*mem_start = mesrec->mem_pnt;
        uint32_t index = 0;
        uint32_t pl_start = 0;
        uint32_t pl_len = mesrec->mem_struct_len;
        if(payload)
        {
            if(mesrec->word_indexed)
            {
                memcpy(&index, &payload[mesrec->index_offset], 4);
                pl_start = (uint32_t)mesrec->index_offset + 4;
                pl_len -= 4;
            }
            else if(mesrec->byte_indexed)
            {
                memcpy(&index, &payload[mesrec->index_offset], 1);
                pl_start = (uint32_t)mesrec->index_offset + 1;
                pl_len -= 1;
            }

            if(mesrec->word_length)
            {
                pl_len = 0;
                memcpy(&pl_len, &payload[mesrec->len_offset], 4);
                if(mesrec->len_offset + 4 > pl_start) pl_start = mesrec->len_offset + 4;
            }
            else if(mesrec->byte_length)
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

int mem_mess_getter_copy(mem_mess_record_t conat *mesrec, uint8_t *payload)
{

}
