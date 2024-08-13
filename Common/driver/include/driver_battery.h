#pragma once
// バッテリドライバー

#include "driver_i2c.h"
#include "hardware_config.h"

// バッテリデータ
typedef struct {
    uint16_t voltage[2];
} BatteryData_t;

static BatteryData_t _batteryData;

// バッテリ初期化
void Battery_init() { memset(&_batteryData, 0, sizeof(_batteryData)); }

void Battery_update() {
    // 12bit ADC
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_BATTERY_VOLTAGE,
             (uint8_t*)&_batteryData.voltage, sizeof(_batteryData.voltage));
}

// バッテリ電圧取得
float Battery_getVoltage(uint8_t index) {
    if (index >= 2) {
        return 0.0f;
    }

    return (float)_batteryData.voltage[index] * 0.001f;
}
