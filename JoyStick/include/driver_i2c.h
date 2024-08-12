#pragma once

// I2C を使う場合は Wire ライブラリを使う
#include <Wire.h>

// Atom JoyStickのI2Cアドレス
#define I2C_ADDR_ATOM_JOYSTICK 0x59

// Atom JoyStickのボタン状態取得レジスタ
//   0x70 〜 0x73 までのレジスタにボタン状態が格納されている
#define I2C_BUTTON_REG 0x70

// I2Cの初期化
void I2C_init() {
    // I2Cの初期化
    Wire.begin();
}

// I2Cでデータを読み込む
void I2C_read(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len) {
    // I2Cでデータを読み込む
    Wire.beginTransmission(addr);
    // レジスタを指定
    Wire.write(reg);
    Wire.endTransmission(false);

    // データを読み込む
    Wire.requestFrom(I2C_ADDR_ATOM_JOYSTICK, len);
    for (int i = 0; i < len; i++) {
        data[i] = Wire.read();
    }
    Wire.endTransmission();
}
