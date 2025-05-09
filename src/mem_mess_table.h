/**
 * @file mem_mess_table.h
 * @author Bryce Deary (bdeary@chauvetcinema.com)
 * @brief 
 * @date 2025-05-03
 * 
 * Copyright Chauvet Cinema Group (2025)
 * 
 */

#ifndef MEM_MESS_TABLE_H
#define MEM_MESS_TABLE_H

#include "mem_mess_message.h"

// forward ref
typedef struct mem_mess_table_t mem_mess_table_t;

/**
 * @brief two helper functions to find a token(message number) in a set of message records,
 *      there are two versions that can be used depending on if the message records are 
 *      sorted by token.
 * 
 * @param table - pointer to the table object
 * @param token - the token to find in the table
 * @return mem_mess_record_t const* pointer to the located record or NULL if not found.
 */
mem_mess_record_t const *mem_mes_get_record(mem_mess_table_t const *table, uint16_t token);
mem_mess_record_t const *mem_mes_get_record_sorted(mem_mess_table_t const *table, uint16_t token);

// prototype of the search function if user wants other strategy. 
typedef mem_mess_record_t const *(*mem_mes_get_record_t)(mem_mess_table_t const *table, uint16_t token);

// object to collect information about a table of message definitions
typedef struct mem_mess_table_t
{
    mem_mes_get_record_t get_record;    // method pointer to record getter function
    mem_mess_record_t const *table;     // pointer to array of message definations
    uint16_t table_entries;             // members in array
} mem_mess_table_t;


// helper macros for defining mem_mess_table object depending on if array is sorted
// mem_mess_table_t my_table = BIND_MES_RECS( array_name );  or
// mem_mess_table_t my_table = BIND_SORTED_MES_RECS( array_name );
#define BIND_MESS_RECS(mes_rec_arr_name) \
{  .table=mes_rec_arr_name, \
   .table_entries = ARR_LEN(mes_rec_arr_name), \
   .get_record = mem_mes_get_record }

#define BIND_SORTED_MESS_RECS(mes_rec_arr_name) \
{  .table=mes_rec_arr_name, \
    .table_entries = ARR_LEN(mes_rec_arr_name), \
    .get_record = mem_mes_get_record_sorted }



/**
 * @brief process a payload buffer against a mess_rec, with the table
 *      provided for message linking.
 * 
 * @param mes_tbl pointer to the table
 * @param mes_rec pointer to the message to process
 * @param payload pointer to the data associated with the message
 * @param pl_size buffer limit for payload error checking 
 * @return int  0 - normal completion
 *             >0 - indicates data was returned in payload
 *             <0 - error         
 */
int mem_mess_process(mem_mess_table_t const *mes_tbl, mem_mess_record_t const *mes_rec, uint8_t *payload, uint32_t pl_size);





#endif // MEM_MESS_TABLE_H

