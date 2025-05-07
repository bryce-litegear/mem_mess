
#ifndef LITENET_H
#define LITENET_H
#include <stdint.h>
#include "mem_mess.h"

// this is a legacy size long enough to send one row of firmware image as a payload
#define REC_BUFFER_SIZE (sizeof(ln_message_header_t)+ 1 + 4 + 4 + 256 + 1)

typedef uint8_t ln_check_t; // currently only 8 bits

// the full 4byte overlay
typedef struct ln_message_header_t
{
    uint8_t sync;               // usually 0x55

    uint8_t address :7;         // the panel address 1-127 with 0 reserved for broadcast
    uint8_t s1      :1;         // direction bit, set on panel generated messages, clear from dimmer

    uint8_t protocol :7;        // the protocol 0-31 with higher reserved
    uint8_t s2_not   :1;        // the inverted direction bit

    uint8_t message_no :7;      // the message number 0-127
    uint8_t s3      :1;         // the direction bit, must match s1 and not match s2_not
} ln_message_header_t;

// message overlay
typedef struct ln_message_t
{
    ln_message_header_t header;
    uint8_t payload[];
} ln_message_t;


typedef struct litenet_t
{
    uint8_t * const rec_buffer;     // working buffer
    ln_message_t * const mes_buffer;// working buffer
    uint8_t* const rec_buffer_limit;// limit of rec_buffer
    uint16_t next_idx;              // index into rec_buffer
    uint16_t check_byte_index;      // estimate of checksum index in payload
    ln_check_t checksum;            // running checksum
    uint8_t state;                  // decoding state
    uint8_t address;                // address of this object or 0 
    const uint8_t protocol;         // protocol of this object
    mem_mess_table_t const * const mes_table;   // the message record table
    mem_mess_record_t *mes_rec;     // current message or NULL
} litenet_t;

void litenet_process(litenet_t *ln, uint16_t ch);




#endif // LITENET_H
