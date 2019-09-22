#include "solaredge.h"

bool
SolarEdgeInverter::writeToBuffer(sunspec_register_t r, char *buf, int len)
{
    int reg = get_register_offset(r);
    if (!_client->requestFrom(_id, HOLDING_REGISTERS, reg, len / 2)) {
        return false;
    }

    int i = 0;
    while (_client->available() && i < len) {
        buf[i] = _client->read();
        i += 2;
    }
    return true;
}

float
register_to_float(sunspec_register_t r, long val)
{
    register_map_t reg = register_map[r];
    float v = 0.0;

    switch (reg.type) {
    case ACC32:
    case UINT32:
        v = (uint32_t)val;
        break;
    case INT16:
        v = (int16_t)val;
        break;
    case INT32:
        v = (int32_t)val;
        break;
    case UINT16:
        v = (uint16_t)val;
        break;
    default:
        /* NOT IMPLEMENTED */
        break;
    }

    return v;
}

bool
SolarEdgeInverter::getScaledRegister(sunspec_register_t r, sunspec_register_t scaled_r, float *val)
{
    register_map_t reg = register_map[r];
    register_map_t scaled_reg = register_map[scaled_r];

    int r_off = get_register_offset(reg.reg);
    int s_off = get_register_offset(scaled_reg.reg);

    long v = -1;

    // First get the unscaled value
    if (reg.len == 1) {
        if ((v = _client->holdingRegisterRead(_id, r_off)) == -1)
            return false;
    } else {
        if (!_client->requestFrom(_id, HOLDING_REGISTERS, r_off, 2)) {
            return false;
        }

        // if it's not one register, it's two. That's a fact for this library.
        if (!_client->available())
            return false;

        v = _client->read() << 16L;
        v |= _client->read();
    }

    *val = register_to_float(reg.reg, v);

    // ...and then get the scaling factor
    if ((v = _client->holdingRegisterRead(_id, s_off)) == -1)
        return false;

    *val *= pow(10, register_to_float(scaled_reg.reg, v));

    return true;
}

SolarEdgeInverter::SolarEdgeInverter() :
    _id(0),
    _client(NULL)
{
}

void
SolarEdgeInverter::begin(ModbusClient *client, int modbus_id)
{
    _client = client;
    _id = modbus_id;
}

bool
SolarEdgeInverter::GetManufacturer(char *buf, int len)
{
    return writeToBuffer(C_MANUFACTURER, buf, len);
}

bool
SolarEdgeInverter::GetModel(char *buf, int len)
{
    return writeToBuffer(C_MODEL, buf, len);
}

bool
SolarEdgeInverter::GetVersion(char *buf, int len)
{
    return writeToBuffer(C_VERSION, buf, len);
}

bool
SolarEdgeInverter::GetSerial(char *buf, int len)
{
    return writeToBuffer(C_SERIAL, buf, len);
}

bool
SolarEdgeInverter::TotalACCurrent(float *val)
{
    return getScaledRegister(I_AC_CURRENT, I_AC_CURRENT_SF, val);
}

bool
SolarEdgeInverter::ACPower(float *val)
{
    return getScaledRegister(I_AC_POWER, I_AC_POWER_SF, val);
}

bool
SolarEdgeInverter::ACFrequency(float *val)
{
    return getScaledRegister(I_AC_FREQUENCY, I_AC_FREQUENCY_SF, val);
}

bool
SolarEdgeInverter::ApparentPower(float *val)
{
    return getScaledRegister(I_AC_VA, I_AC_VA_SF, val);
}

bool
SolarEdgeInverter::ReactivePower(float *val)
{
    return getScaledRegister(I_AC_VAR, I_AC_VAR_SF, val);
}

bool
SolarEdgeInverter::PowerFactor(float *val)
{
    return getScaledRegister(I_AC_PF, I_AC_PF_SF, val);
}

// XXX: This isn't correct! It uses two 16-bit registers
bool
SolarEdgeInverter::LifetimeEnergyProduction(float *val)
{
    return getScaledRegister(I_AC_ENERGY_WH, I_AC_ENERGY_WH_SF, val);
}

bool
SolarEdgeInverter::DCCurrent(float *val)
{
    return getScaledRegister(I_DC_CURRENT, I_DC_CURRENT_SF, val);
}

bool
SolarEdgeInverter::DCVoltage(float *val)
{
    return getScaledRegister(I_DC_VOLTAGE, I_DC_VOLTAGE_SF, val);
}

bool
SolarEdgeInverter::DCPower(float *val)
{
    return getScaledRegister(I_DC_POWER, I_DC_POWER_SF, val);
}

bool
SolarEdgeInverter::HeatSinkTemperature(float *val)
{
    if (getScaledRegister(I_TEMP_SINK, I_TEMP_SF, val)) {
        *val = (*val * 9.0 / 5.0) + 32.0;
        return true;
    }
    return false;
}

bool SolarEdgeInverter::OperatingState(inverter_status_t *status)
{
    long reg = get_register_offset(I_STATUS);
    long st = -1;

    if ((st = _client->holdingRegisterRead(_id, reg)) == -1)
        return false;

    *status = (inverter_status_t)st;
    return true;
}

const char *
SolarEdgeInverter::LastError()
{
    return _client->lastError();
}
