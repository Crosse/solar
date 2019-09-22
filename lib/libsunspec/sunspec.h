#ifndef _LIBSUNSPEC_H_
#define _LIBSUNSPEC_H_

#include <inttypes.h>

const uint16_t BASE_REGISTER_COMMON_BLOCK = 40000;

typedef enum : uint8_t {
    ACC32 = 0,
    INT16,
    INT32,
    STRING,
    UINT16,
    UINT32
} register_type_t;
static_assert(sizeof(register_type_t) == 1, "incorrect size for register_type_t");

typedef enum : uint8_t {
    /* Common registers */
    C_SUNSPEC_ID = 0,
    C_SUNSPEC_DID,
    C_SUNSPEC_LENGTH,
    C_MANUFACTURER,
    C_MODEL,
    C_VERSION,
    C_SERIAL,
    C_DEVICE_ADDRESS,

    /* Inverter registers */
    I_SUNSPEC_DID,
    I_SUNSPEC_LEN,
    I_AC_CURRENT,
    I_AC_CURRENT_A,
    I_AC_CURRENT_B,
    I_AC_CURRENT_C,
    I_AC_CURRENT_SF,
    I_AC_VOLTAGE_AB,
    I_AC_VOLTAGE_BC,
    I_AC_VOLTAGE_CA,
    I_AC_VOLTAGE_AN,
    I_AC_VOLTAGE_BN,
    I_AC_VOLTAGE_CN,
    I_AC_VOLTAGE_SF,
    I_AC_POWER,
    I_AC_POWER_SF,
    I_AC_FREQUENCY,
    I_AC_FREQUENCY_SF,
    I_AC_VA,
    I_AC_VA_SF,
    I_AC_VAR,
    I_AC_VAR_SF,
    I_AC_PF,
    I_AC_PF_SF,
    I_AC_ENERGY_WH,
    I_AC_ENERGY_WH_SF,
    I_DC_CURRENT,
    I_DC_CURRENT_SF,
    I_DC_VOLTAGE,
    I_DC_VOLTAGE_SF,
    I_DC_POWER,
    I_DC_POWER_SF,
    I_TEMP_SINK,
    I_TEMP_SF,
    I_STATUS,
    I_STATUS_VENDOR
} sunspec_register_t;
static_assert(sizeof(sunspec_register_t) == 1, "incorrect size for sunspec_register_t");

typedef struct {
    sunspec_register_t reg;
    uint16_t address;
    uint8_t len;
    uint8_t type;
} register_map_t;

extern register_map_t register_map[];

typedef enum : uint8_t {
    I_UNUSED = 0,
    I_STATUS_OFF,
    I_STATUS_SLEEPING,
    I_STATUS_STARTING,
    I_STATUS_MPPT,
    I_STATUS_THROTTLED,
    I_STATUS_SHUTTING_DOWN,
    I_STATUS_FAULT,
    I_STATUS_STANDBY
} inverter_status_t;
static_assert(sizeof(inverter_status_t) == 1, "incorrect size for inverter_status_t");

struct inverter_status {
    inverter_status_t value;
    const char *description;
};

extern struct inverter_status inverter_status_map[];

int get_register_offset(sunspec_register_t reg);
int get_register_address(sunspec_register_t reg);

#endif /* _LIBSUNSPEC_H_ */
