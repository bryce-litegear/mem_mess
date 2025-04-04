/**
 * @file mem_mess_table.c
 * @author Bryce Deary (bryce.deary@litegear.com)
 * @brief mem to message table module to define a message table and find a message record
 *  based on the search token.
 * @date 2025-04-04
 * 
 * @copyright Copyright LiteGear (2025)
 * 
 */

#include "mem_mess_table.h"
#include <stddef.h>

// if user defines the table already sorted by token we can use bsearch to find the record
#ifdef MEM_REC_IS_SORTED
#include <stdlib.h>

// bsearch compare function key in obj
// returns neg, 0, or pos
STATIC int comp_tokin_no(const void* key, const void* obj)
{
    uint16_t const *token = key;
    mem_mess_record_t const *mr = obj;
    return (int)*token - (int)mr->token;
}

mem_mess_record_t const *mem_mes_get_record(mem_mess_table_t const *table, uint16_t token)
{
    return bsearch(&token, table->table,  table->table_entries, sizeof table->table[0], comp_tokin_no);
}
    
#else // if not sorted just do linear search for token.

mem_mess_record_t const *mem_mes_get_record(mem_mess_table_t const *table, uint16_t token)
{
    for(int rec = 0; rec < table->table_entries; rec++)
    {
        if(token == table->table[rec].token) return &table->table[rec];
    }
    return NULL;
}

#endif
