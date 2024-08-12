#pragma once

// I2C を使う場合は Wire ライブラリを使う
#include <Wire.h>

// I2Cの初期化
void I2C_init() {
    // I2Cの初期化
    Wire.begin();
}

// I2Cの更新(何もしない)
void I2C_update() {}

// I2Cでデータを読み込む
void I2C_read(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len) {
    // I2Cでデータを読み込む
    Wire.beginTransmission(addr);
    // レジスタを指定
    Wire.write(reg);
    Wire.endTransmission(false);

    // データを読み込む
    Wire.requestFrom(addr, len);
    for (int i = 0; i < len; i++) {
        data[i] = Wire.read();
    }
    Wire.endTransmission();
}
