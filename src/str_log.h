/**
 * @file str_log.h
 * @author Bryce Deary (bdeary@chauvetcinema.com)
 * @brief reporting log in memory of events represented by pointers to string constants
 * @date 2025-05-01
 * 
 * @copyright Copyright LiteGear (2025)
 * 
 */
#ifndef STR_LOG_H
#define STR_LOG_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief alias for pointer to string const
 */
typedef char const * str_const_t;

/**
 * @brief the log is just a collection of pointers to const string
 */
typedef struct str_log_t
{
    str_const_t *oldest;  // last use
    str_const_t *last;  // next location
    const uint16_t arr_len;   // 
    uint16_t lost;
    str_const_t * const log;
} str_log_t;



#define STR_LOG_DEF(name, size) \
str_const_t name ## _slog_store[size]; \
str_log_t name ## _slog = {.arr_len = size, .log = name ## _slog_store}

#define STR_LOG_DEC(name) \
extern str_log_t name ## _slog

void mem_str_log_add(str_log_t *slog, char const* str);
char const *mem_str_log_get(str_log_t *slog);


#endif // STR_LOG_H
