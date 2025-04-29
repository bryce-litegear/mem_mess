
#ifndef MEM_MESS_MEMORY_H
#define MEM_MESS_MEMORY_H
#include <stdint.h>

typedef char const * str_const_t;
typedef struct str_log_t
{
    str_const_t *last;  // last use
    str_const_t *next;  // next location
    uint16_t arr_len;   // 
    str_const_t log[];
} str_log_t;

/**
 * @brief Add a new (different) string to the log
 * 
 * @param log pointer to the log structure
 * @param str the string const pointer to add
 */
void mem_log_str(str_log_t *log, str_const_t str);

/**
 * @brief pop the oldest string from the log
 * 
 * @param log pointer to the log structure
 * @return str_const_t the pointer, removed from the log
 */
str_const_t mem_log_str_get(str_log_t *log);

#define STR_LOG_DEF(name, size) \
str_const_t name##_slog[size]; \
str_const_t *next_##name = name##_slog; \
void add_##name(str_const_t str)  \
{   \
    if(*next_##name == NULL) *next_##name = str; \
    /* not a repeat, write it */ \
    else if(*next_##name != str) \
    {   \
        next_##name++;  \
        if(next_##name >= &name##_slog) next_##name = name##_slog; \
        *next_##name = str; \
    } \
}


#define STR_LOG_DEC(name, size) \
extern str_const_t name##_slog[size]; \
void add_##name(str_const_t str);


#endif // MEM_MESS_MEMORY_H
