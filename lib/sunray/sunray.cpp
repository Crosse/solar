#include <inttypes.h>
#include <stddef.h>

#include "sunray.h"

static uint16_t
crc16_ccitt(uint8_t *buf, size_t len)
{
    uint8_t x;
    uint16_t crc = 0xFFFF;

    while (len--){
        x = crc >> 8 ^ *buf++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
    }
    return crc;
}

uint16_t
add_checksum(msg_t *msg)
{
    if (msg == NULL)
        return 0;

    if (msg->hdr.len > sizeof(msg_t))
        return 0;

    // Ensure that the CRC field in the message is zeroed out.
    msg->hdr.crc = 0;

    uint16_t crc = crc16_ccitt((uint8_t *)msg, msg->hdr.len);
    msg->hdr.crc = crc;

    return crc;
}
