/**
 * @file litenet
 * @author Bryce Deary (bdeary@chauvetcinema.com)
 * @brief 
 * @date 2025-05-06
 * 
 * Copyright Chauvet Cinema Group (2025)
 * 
 */
#include "litenet.h"
// The states of the liteNet state machine
typedef enum litenet_state_t
{
    LN_SYNC,            // 0x55
    LN_ADDRESS,         // 0 for broadcast, panel address
    LN_PROTOCOL,        // 0, or 1
    LN_MESS_NO,         // the message number
    LN_PAYLOAD,         // collect payload or skip if length is 0
    LN_PAYLOAD_SPECIAL, // was variable length with length in the early payload
    LN_CHECK_AND_G0,    // validate checksum and jump to processing function.
    LN_SNIFF_PAYLOAD,   // allow follow along, not receiving
    LN_SNIFF_PAYLOAD_SPECIAL, // allow follow along, not receiving
    LN_SNIFF_CHECK,     // allow follow along, not receiving
} litenet_state_t;

void ln_set_address(ln_process_obj_t *lnt, uint8_t address)
{
    ln->ln_address = address & 0x7f; // limit to 7 bits
}

uint8_t litenet_address; // global address or zero

// Litenet packet constants:
#define LN_PACKET_START (0x55)
#define LN_PACKET_SEED (0x93)
#define LN_PACKET_OBFUSCATOR (0xC5) // used in checksum

// rot use in checksum
static inline uint16_t short_rot_r(uint16_t in)
{
    return (in >> 1) | (in << 15);
}

static inline uint16_t short_rot_l(uint16_t in)
{
    return (in >> 15) | (in << 1);
}

static inline uint8_t byte_rot_r(uint8_t in)
{
    return (in >> 1) | (in << 7);
}

static inline uint8_t byte_rot_l(uint8_t in)
{
    return (in >> 7) | (in << 1);
}

uint8_t add_to_checksum8(uint8_t checksum, uint8_t data)
{
    checksum += data;
    checksum = byte_rot_r(checksum);
    return checksum;
}

uint16_t add_to_checksum16(uint16_t checksum, uint16_t data)
{
    checksum += data;
    checksum = short_rot_r(checksum);
    return checksum;
}

ln_check_t add_to_liteNet_checksum(ln_check_t acc, uint8_t new_byte)
{
    return (ln_check_t)add_to_checksum8(acc, new_byte ^ LN_PACKET_OBFUSCATOR);
}

// used in receive function to maintain ongoing checksum
static inline void cycle_checksum(litenet_t *ln, uint8_t nbyte)
{
    ln->checksum = add_to_liteNet_checksum(ln->checksum, nbyte);
}

static void reprocess_ln(litenet_t *ln)
{
    uint8_t *next = ln->rec_buffer + 1;               // point at word 1 (skip 0)
    uint8_t *end =  ln->rec_buffer + ln->next_idx;    // point at next empty
    ln->state = LN_SYNC;   // restart
    ln->next_idx = 0;      // point at word 0
    while(next < end) ln_process(ln, *next++);  // reprocess the data
}

void litenet_process(litenet_t *ln, uint16_t ch)
{
    uint8_t new_byte = ch & 0xff;

    // if bus error, do reset
    if(ch != new_byte) ln->state = LN_SYNC;
    ln->rec_buffer[ln->next_idx++] = new_byte; // accumulate the message

    switch (ln->state)
    {
        case LN_SYNC:
        {
            if (new_byte == 0x55) 
            {
                ln->next_idx = 1;
                ln->checksum = LN_PACKET_SEED;
                ln->rec_buffer[0] = new_byte;
                cycle_checksum(ln, new_byte);
                ln->state = LN_ADDRESS;
            }            
            else
            {
                ln->next_idx = 0;
            }
        }
        break;

        case LN_ADDRESS:
        {
            cycle_checksum(ln, new_byte);
            ln->mes_rec = NULL;
            ln->state = LN_PROTOCOL;
        }
        break;        

        case LN_PROTOCOL:
        {
            cycle_checksum(ln, new_byte);
            ln->state = LN_MESS_NO;
        }
        break;

        case LN_MESS_NO:
        {
            // do protocol and parity verification of mes->header.       
            if (ln->mes_buffer->header.protocol == ln->protocol &&
                ln->mes_buffer->header.s1 == ln->mes_buffer->header.s3 &&
                ln->mes_buffer->header.s1 != ln->mes_buffer->header.s2_not)
            {
                // see if message exists in the mes table
                ln->mes_rec = ln->mes_table->get_record(ln->mes_table, ln->mes_buffer->header.message_no);
                if (ln->mes_rec)
                {
                    cycle_checksum(ln, new_byte);
                    uint16_t payload_len = ln->mes_rec->mem_instance_len;
                    // for non zero payloads, setup for anything special
                    if (payload_len)
                    {
                        if(ln->mes_rec->len_in_pl) // special flag
                        {
                            ln->state = LN_PAYLOAD_SPECIAL;
                        }
                        else
                        {
                            ln->state = LN_PAYLOAD;
                        }
                    }
                    else if(ln->mes_rec->is_string)
                    {
                        payload_len = ln->buffer_len - sizeof ln->mes->header - sizeof(ln_check_t);
                    }
                    else
                    {
                        ln->state = LN_CHECK_AND_G0;
                    }
                    // set target byte to inspect for checksum or extend based on embedded length
                    
                    ln->check_byte_index = ln->next_idx + payload_len;
    
                    // don't allow overflow
                    if(ln->check_byte_index >= ln->buffer_len) 
                    {
                        // too big to process so assume bad header
                        reprocess_ln(lnt);
                    }
                }
                else // unknown message reprocess the mes->header.           
                {
                    reprocess_ln(lnt);
                }
            }
            else
            { // header direction parity or protocol failed, reprocess the mes->header.           
                reprocess_ln(lnt);
            }
        }
        break;        

    }






}

