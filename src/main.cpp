#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#include <solaredge.h>
#include <sunray.h>

static const int serial_tx_pin = 3;
static const int serial_rx_pin = 4;
SoftwareSerial SerialMonitor(serial_rx_pin, serial_tx_pin);

static const long modbus_baud_rate = 9600;
int inverter_id = 2;
SolarEdgeInverter inverter;

//char buf[32] = { 0 };

/*
void
print_u16_as_ascii(long val)
{
    char first = val >> 8;
    char second = val & 0xff;

    if (first > 0)
        SerialMonitor.print(first);
    if (second > 0)
        SerialMonitor.print(second);
}
*/

void
print_last_error()
{
    SerialMonitor.print(F("[E] "));
    SerialMonitor.println(inverter.LastError());
}

void
setup()
{
    SerialMonitor.begin(9600);
    while (!SerialMonitor)
        delay(100);

    // Set up Modbus.
    if (!ModbusRTUClient.begin(modbus_baud_rate)) {
        SerialMonitor.println(F("[E] Modbus client failure"));
        while(1);
    }

    // Set up the inverter.
    inverter.begin(&ModbusRTUClient, inverter_id);
}

bool
send_message(msg_t *msg)
{
    if (add_checksum(msg) == 0) {
        // Something went wrong and we should handle it.
        return false;
    }

    // Clearly everything went right.
    return true;
}

void
loop()
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    inverter_status_t status = I_UNUSED;
    float value = 0;

    if (!inverter.OperatingState(&status)) {
        print_last_error();
        goto done;
    }

    if (!inverter.DCVoltage(&value)) {
        print_last_error();
        goto done;
    }

    if (!inverter.DCCurrent(&value)) {
        print_last_error();
        goto done;
    }

    if (!inverter.DCPower(&value)) {
        print_last_error();
        goto done;
    }

    if (!inverter.HeatSinkTemperature(&value)) {
        print_last_error();
        goto done;
    }

    if (!inverter.ACPower(&value)) {
        print_last_error();
        goto done;
    }

    if (!inverter.TotalACCurrent(&value)) {
        print_last_error();
        goto done;
    }

    if (!inverter.ACFrequency(&value)) {
        print_last_error();
        goto done;
    }

    if (!inverter.LifetimeEnergyProduction(&value)) {
        print_last_error();
        goto done;
    }

done:
    delay(2500);
    SerialMonitor.println();
}
