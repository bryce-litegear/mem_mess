
#ifndef MEM_MESS_MEMORY_H
#define MEM_MESS_MEMORY_H

typedef char const * str_const;

#define STR_LOG_DEF(name, size) \
str_const name##_slog[size]; \
str_const *next_##name = name##_slog; \
void add_##name(str_const str)  \
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
extern str_const name##_slog[size]; \
void add_##name(str_const str);


#endif // MEM_MESS_MEMORY_H
