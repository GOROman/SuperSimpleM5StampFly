// はじめの十五歩
// PWMモータードライバーを追加。モーターを回す。tone()関数がモーターと干渉するのでサウンドドライバもPWM化。

#include <Arduino.h>

// ../Common/Driver/include/...
#include "driver_i2c.h"    // I2Cドライバ
#include "driver_led.h"    // LEDドライバ
#include "driver_motor.h"  // モータードライバ
#include "driver_sound.h"  // サウンドドライバ
#include "driver_timer.h"  // Timerドライバ

void setup() {
    USBSerial.begin(115200);

    // 各ドライバーの初期化
    LED_init();         // フルカラーLEDの初期化
    I2C_init();         // I2Cの初期化
    Sound_init();       // サウンドの初期化
    Timer_init(10000);  // タイマーの初期化(10000us = 10ms)
    Motor_init();       // モーターの初期化

    Sound_play(0);
}

int frame = 0;

void loop() {
    Timer_sync();  // フレーム同期

    // I2C通信の更新
    I2C_update();

    if (frame % 200 < 20) {
        LED_setColor(0, 255, 0, 0);
        // モーターをちょっと回してみる（0.1f = 10%）
        Motor_setSpeed(0, 0.1f);
        Motor_setSpeed(1, 0.1f);
        Motor_setSpeed(2, 0.1f);
        Motor_setSpeed(3, 0.1f);
    } else {
        LED_setColor(0, 0, 0, 0);
        // モーターを止める
        Motor_setSpeed(0, 0);
        Motor_setSpeed(1, 0);
        Motor_setSpeed(2, 0);
        Motor_setSpeed(3, 0);
    }

    ++frame;

    Motor_update();  // モーターの更新
    LED_update();    // フルカラーLEDの更新
    Timer_update();  // タイマーの更新
    Sound_update();  // サウンドの更新
}
