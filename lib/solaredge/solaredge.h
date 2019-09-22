#ifndef _SOLAREDGE_H_
#define _SOLAREDGE_H_

#include <ArduinoModbus.h>

#include <sunspec.h>

class SolarEdgeInverter {
public:
    SolarEdgeInverter();

    void begin(ModbusClient* client, int modbus_id);

    bool GetManufacturer(char *buf, int len);
    bool GetModel(char *buf, int len);
    bool GetVersion(char *buf, int len);
    bool GetSerial(char *buf, int len);
    bool TotalACCurrent(float *val);
    bool ACPower(float *val);
    bool ACFrequency(float *val);
    bool ApparentPower(float *val);
    bool ReactivePower(float *val);
    bool PowerFactor(float *val);
    bool LifetimeEnergyProduction(float *val);
    bool DCCurrent(float *val);
    bool DCVoltage(float *val);
    bool DCPower(float *val);
    bool HeatSinkTemperature(float *val);
    bool OperatingState(inverter_status_t *status);

    const char *LastError();

private:
    bool getScaledRegister(sunspec_register_t r, sunspec_register_t scaled_r, float *val);
    bool writeToBuffer(sunspec_register_t r, char *buf, int len);

    int _id;
    ModbusClient* _client;
};

#endif /* _SOLAREDGE_H_ */
