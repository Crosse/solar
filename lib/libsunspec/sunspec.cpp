#include "sunspec.h"

register_map_t register_map[] = {
    /* Comomn registers */
    { C_SUNSPEC_ID,             1,  2, UINT32 },    /* "SunS"; Sunspec MODBUS Map Identifier */
    { C_SUNSPEC_DID,            3,  1, UINT16 },    /* 0x0001; SunSPecc Common Model Block Idenfifier */
    { C_SUNSPEC_LENGTH,         4,  1, UINT16 },    /* Length of block (50) */
    { C_MANUFACTURER,           5, 16, STRING },    /* "SolarEdge" */
    { C_MODEL,                 21, 16, STRING },    /* SolarEdge Model Value */
    { C_VERSION,               45,  8, STRING },    /* SolarEdge Version */
    { C_SERIAL,                53, 16, STRING },    /* SolarEdge Serial Number */
    { C_DEVICE_ADDRESS,        69,  1, UINT16 },    /* MODBUS ID of the device */

    /* Inverter registers */
    { I_SUNSPEC_DID,           70,  1, UINT16 },    /* Inverter type (single/split/three phase */
    { I_SUNSPEC_LEN,           71,  1, UINT16 },    /* length of model block */
    { I_AC_CURRENT,            72,  1, UINT16 },    /* AC Total Current */
    { I_AC_CURRENT_A,          73,  1, UINT16 },    /* AC Phase A Current */
    { I_AC_CURRENT_B,          74,  1, UINT16 },    /* AC Phase B Current */
    { I_AC_CURRENT_C,          75,  1, UINT16 },    /* AC Phase C Current */
    { I_AC_CURRENT_SF,         76,  1,  INT16 },    /* AC Current scale factor */
    { I_AC_VOLTAGE_AB,         77,  1, UINT16 },    /* AC Voltage Phase AB */
    { I_AC_VOLTAGE_BC,         78,  1, UINT16 },    /* AC Voltage Phase BC */
    { I_AC_VOLTAGE_CA,         79,  1, UINT16 },    /* AC Voltage Phase CA */
    { I_AC_VOLTAGE_AN,         80,  1, UINT16 },    /* AC Voltage Phase A to N */
    { I_AC_VOLTAGE_BN,         81,  1, UINT16 },    /* AC Voltage Phase B to N */
    { I_AC_VOLTAGE_CN,         82,  1, UINT16 },    /* AC Voltage Phase C to N */
    { I_AC_VOLTAGE_SF,         83,  1,  INT16 },    /* AC Voltage scale factor */
    { I_AC_POWER,              84,  1,  INT16 },    /* AC Power */
    { I_AC_POWER_SF,           85,  1,  INT16 },    /* AC Power scale factor */
    { I_AC_FREQUENCY,          86,  1, UINT16 },    /* AC Frequency */
    { I_AC_FREQUENCY_SF,       87,  1,  INT16 },    /* AC Frequency scale factor */
    { I_AC_VA,                 88,  1,  INT16 },    /* Apparent Power */
    { I_AC_VA_SF,              89,  1,  INT16 },    /* Apparent Power scale factor */
    { I_AC_VAR,                90,  1,  INT16 },    /* Reactive Power */
    { I_AC_VAR_SF,             91,  1,  INT16 },    /* Reactive Power scale factor */
    { I_AC_PF,                 92,  1,  INT16 },    /* Power Factor */
    { I_AC_PF_SF,              93,  1,  INT16 },    /* Power Factor scale factor */
    { I_AC_ENERGY_WH,          94,  2,  ACC32 },    /* AC Lifetime Energy production */
    { I_AC_ENERGY_WH_SF,       96,  1, UINT16 },    /* ...scale factor */
    { I_DC_CURRENT,            97,  1, UINT16 },    /* DC Current */
    { I_DC_CURRENT_SF,         98,  1,  INT16 },    /* DC Current scale factor */
    { I_DC_VOLTAGE,            99,  1, UINT16 },    /* DC Voltage */
    { I_DC_VOLTAGE_SF,        100,  1,  INT16 },    /* DC Voltage scale factor */
    { I_DC_POWER,             101,  1,  INT16 },    /* DC Power */
    { I_DC_POWER_SF,          102,  1,  INT16 },    /* DC Power scale factor */
    { I_TEMP_SINK,            104,  1,  INT16 },    /* Heat Sink Temperature */
    { I_TEMP_SF,              107,  1,  INT16 },    /* Heat Sink Temp scale factor */
    { I_STATUS,               108,  1, UINT16 },    /* Operating State */
    { I_STATUS_VENDOR,        109,  1, UINT16 }     /* Vendor-defined operating state */
};

int
get_register_offset(sunspec_register_t reg)
{
    return register_map[reg].address - 1;
}

int
get_register_address(sunspec_register_t reg)
{
    return BASE_REGISTER_COMMON_BLOCK + get_register_offset(reg);
}

struct inverter_status inverter_status_map[] = {
    { I_UNUSED,                 "Unused" },
    { I_STATUS_OFF,             "Off" },
    { I_STATUS_SLEEPING,        "Sleeping (Night mode)" },
    { I_STATUS_STARTING,        "Grid monitoring / wake-up" },
    { I_STATUS_MPPT,            "Inverter is ON and producing power" },
    { I_STATUS_THROTTLED,       "Production (throttled)" },
    { I_STATUS_SHUTTING_DOWN,   "Shutting down" },
    { I_STATUS_FAULT,           "Fault" },
    { I_STATUS_STANDBY,         "Maintenance / setup" }
};
