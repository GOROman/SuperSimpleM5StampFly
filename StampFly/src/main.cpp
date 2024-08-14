// はじめの二十一歩
// BLEで受信したデータを使ってモーターを制御する

#include <Arduino.h>

// BLEライブラリ
#include <NimBLEDevice.h>

#define BLE_PERIPHERAL 1

// ../Common/Driver/include/...
#include "driver_ble.h"    // BLEドライバ
#include "driver_i2c.h"    // I2Cドライバ
#include "driver_led.h"    // LEDドライバ
#include "driver_motor.h"  // モータードライバ
#include "driver_sound.h"  // サウンドドライバ
#include "driver_timer.h"  // Timerドライバ

// フラグ
static bool is_connected = false;
// BLEデータ
static CommData_t commdata;

// BLEイベントコールバック
static void _ble_event_callback(BLEEventParam_t* param) {
    switch (param->event) {
        // 接続時に呼ばれる
        case BLE_EVENT_CONNECTED:
            USBSerial.println("BLE_EVENT_CONNECTED");
            Sound_play(SOUND_PRESET_CONNECTED);
            is_connected = true;
            break;
        // 切断時に呼ばれる
        case BLE_EVENT_DISCONNECTED:
            USBSerial.println("BLE_EVENT_DISCONNECTED");
            Sound_play(SOUND_PRESET_DISCONNECTED);
            is_connected = false;
            break;
        // データ受信時に呼ばれる
        case BLE_EVENT_RECEIVED:
            memcpy(&commdata, &param->data, sizeof(CommData_t));
            break;
    }
}

void setup() {
    USBSerial.begin(115200);

    // 各ドライバーの初期化
    LED_init();                     // フルカラーLEDの初期化
    I2C_init();                     // I2Cの初期化
    Sound_init();                   // サウンドの初期化
    Timer_init(10000);              // タイマーの初期化(10000us = 10ms)
    Motor_init();                   // モーターの初期化
    BLE_init(_ble_event_callback);  // BLEの初期化
    BLE_connect();                  // BLEの接続

    Sound_play(SOUND_PRESET_BOOT);
}

int frame = 0;

void loop() {
    Timer_sync();  // フレーム同期

    I2C_update();  // I2C通信の更新
    BLE_update();  // BLE通信の更新

    int y1 = commdata.y1;

    if (is_connected) {
        float pow = (2048 - y1) / 2048.0f;  // 0.0f ~ 1.0f
        if (pow < 0.0f) {
            pow = 0.0f;
        }
        // 0-4095 を -1 to +1
        float x = commdata.x2 / 4095.0f * 2.0f - 1.0f;
        float y = commdata.y2 / 4095.0f * 2.0f - 1.0f;

        x *= 0.08f;
        y *= 0.08f;

        // モーターをちょっと回してみる（0.1f = 10%）
        float fl = pow + x + y;
        float fr = pow - x + y;
        float rl = pow + x - y;
        float rr = pow - x - y;
        // 0 - 1 の範囲にクランプ
        fl = constrain(fl, 0.0f, 1.0f);
        fr = constrain(fr, 0.0f, 1.0f);
        rl = constrain(rl, 0.0f, 1.0f);
        rr = constrain(rr, 0.0f, 1.0f);

        if (Timer_getFrameCount() % 5 == 0) {
            USBSerial.printf("X:%4.2f Y:%4.2f - %+4.2f %+4.2f %+4.2f %+4.2f\n",
                             x, y, fl, fr, rl, rr);
        }
        // ジョイスティックのY値でモーターをオンオフする
        if ((y1 > 0) && (y1 < 1024)) {
            int w = pow * 255;
            LED_setColor(0, w, w, w);

            Motor_setSpeed(MOTOR_FL, fl * 0.8f);
            Motor_setSpeed(MOTOR_FR, fr * 0.8f);
            Motor_setSpeed(MOTOR_RL, rl * 0.8f);
            Motor_setSpeed(MOTOR_RR, rr * 0.8f);
        } else {
            LED_setColor(0, 0, 0, 100);

            // モーターを止める
            Motor_setSpeed(MOTOR_FL, 0);
            Motor_setSpeed(MOTOR_FR, 0);
            Motor_setSpeed(MOTOR_RL, 0);
            Motor_setSpeed(MOTOR_RR, 0);
        }
    } else {
        // BLE未接続時はLEDを点滅させる
        LED_setColor(0, 0, 0, (frame % 200 < 100) ? 100 : 0);
    }

    frame++;

    Motor_update();  // モーターの更新
    LED_update();    // フルカラーLEDの更新
    Timer_update();  // タイマーの更新
    Sound_update();  // サウンドの更新
}
