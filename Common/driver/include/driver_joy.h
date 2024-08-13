#pragma once

#include "driver_i2c.h"
#include "hardware_config.h"

enum {
    STICK_L,
    STICK_R,

    STICK_MAX
};

enum {
    BUTTON_TRIG_L,
    BUTTON_TRIG_R,
    BUTTON_STICK_L,
    BUTTON_STICK_R,

    BUTTON_MAX
};

// ジョイスティック
typedef struct {
    // ジョイスティックの生データ
    struct {
        uint16_t x;  // 12bit: 0-4095
        uint16_t y;
    } joy1;
    struct {
        uint16_t x;
        uint16_t y;
    } joy2;
    uint8_t button[BUTTON_MAX];  // 負論理

    // ボタンの状態管理用
    bool buttonState[BUTTON_MAX];
    bool lastButtonState[BUTTON_MAX];
} JoyData_t;

// ジョイスティックのデータ
static JoyData_t joyData;

// JoyStickの初期化
void Joy_init() { memset(&joyData, 0, sizeof(joyData)); }

// JoyStickの状態を更新する(I2C受信)
void Joy_update() {
    // I2Cから生のデータを取得する
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY1_ADC_VALUE_12BITS,
             (uint8_t*)&joyData.joy1, sizeof(joyData.joy1));
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY2_ADC_VALUE_12BITS,
             (uint8_t*)&joyData.joy2, sizeof(joyData.joy2));
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_BUTTON, joyData.button,
             sizeof(joyData.button));

    // ボタンの状態を更新する
    for (int i = 0; i < BUTTON_MAX; i++) {
        // 前回の状態を保存
        joyData.lastButtonState[i] = joyData.buttonState[i];

        // 現在の状態を保存(負論理を正論理に変換)
        joyData.buttonState[i] = joyData.button[i] == LOW;
    }
}

// JoyStickのファームウェアバージョンを取得する
uint8_t Joy_getFirmwareVersion() {
    uint8_t ver;
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_FIRMWARE_VERSION, &ver,
             sizeof(ver));
    return ver;
}

// JoyStickの状態(X,Y)を取得する
void Joy_getAxis(uint8_t index, uint16_t* x, uint16_t* y) {
    if (index == 0) {
        *x = joyData.joy1.x;
        *y = joyData.joy1.y;
    } else {
        *x = joyData.joy2.x;
        *y = joyData.joy2.y;
    }
}

// ボタンが押されているかどうかを取得する
bool Joy_isPressed(uint8_t index) {
    if (index >= BUTTON_MAX) {
        return false;
    }
    return joyData.buttonState[index];
}

// ボタンが押されたかどうかを取得する
bool Joy_wasPressed(uint8_t index) {
    if (index >= BUTTON_MAX) {
        return false;
    }
    return joyData.buttonState[index] && !joyData.lastButtonState[index];
}

// ボタンが離されたかどうかを取得する
bool Joy_wasReleased(uint8_t index) {
    if (index >= BUTTON_MAX) {
        return false;
    }
    return !joyData.buttonState[index] && joyData.lastButtonState[index];
}
