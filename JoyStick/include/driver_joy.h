#pragma once

// Atom JoyStickのI2Cアドレス
#define I2C_ADDR_ATOM_JOYSTICK 0x59

// Atom JoyStickのレジスタ
#define I2C_REG_JOY1_ADC_VALUE_12BITS 0x00
#define I2C_REG_JOY2_ADC_VALUE_12BITS 0x20
#define I2C_REG_BUTTON                0x70

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

// ジョイスティックの生データ
typedef struct {
    struct {
        uint16_t x;  // 12bit: 0-4095
        uint16_t y;
    } joy1;
    struct {
        uint16_t x;
        uint16_t y;
    } joy2;
    uint8_t button[BUTTON_MAX];
} JoyData_t;

// ジョイスティックのデータ
JoyData_t joyData;

// JoyStickの初期化
void Joy_init() { memset(&joyData, 0, sizeof(joyData)); }

// JoyStickの状態を更新する(I2C受信)
void Joy_update() {
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY1_ADC_VALUE_12BITS,
             (uint8_t*)&joyData.joy1, sizeof(joyData.joy1));
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_JOY2_ADC_VALUE_12BITS,
             (uint8_t*)&joyData.joy2, sizeof(joyData.joy2));
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_REG_BUTTON, joyData.button,
             sizeof(joyData.button));
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
    return joyData.button[index] == LOW;
}
