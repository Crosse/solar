#ifndef _SUNRAY_H_
#define _SUNRAY_H_

#include <inttypes.h>

#include <sunspec.h>

typedef enum : uint8_t {
    MSG_UNKNOWN = 0,
    MSG_RESPONSE = 1,
    MSG_TIMESYNC = 2,
    MSG_STATS = 3,
} message_type_t;
static_assert(sizeof(message_type_t) == 1, "incorrect size for message_type_t");

typedef struct {
    message_type_t type;
    uint16_t len;
    uint16_t crc;
} __attribute((__packed__)) msg_hdr_t;
static_assert(sizeof(msg_hdr_t) == 5, "incorrect size for message_hdr_t");

typedef struct {
    sunspec_register_t id;
    union {
        inverter_status_t status;
        int32_t i;
        uint32_t u;
        float f;
    } value;
} __attribute((__packed__)) statistic_t;
static_assert(sizeof(statistic_t) == 5, "incorrect size for statistic_t");

typedef struct {
    msg_hdr_t hdr;
    statistic_t stats[5];
} __attribute((__packed__)) msg_t;
static_assert(sizeof(msg_t) == 30, "incorrect size for msg_t");

uint16_t add_checksum(msg_t *msg);

#endif /* _SUNRAY_H_ */
