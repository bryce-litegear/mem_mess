
#ifndef MEM_MESS_TABLE_H
#define MEM_MESS_TABLE_H

#include "mem_mess_message.h"

typedef struct mem_mess_table_t
{
    mem_mess_record_t const *table;
    uint16_t table_entries;
} mem_mess_table_t;

#endif // MEM_MESS_TABLE_H

mem_mess_record_t const *mem_mes_get_record(mem_mess_table_t const *table, uint16_t token);
