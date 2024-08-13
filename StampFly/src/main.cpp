// はじめの十六歩
// BLEでネットワーク通信をしてモーターを連動させてみる

#include <Arduino.h>
// BLEライブラリ
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

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
static uint32_t ble_data = -1;

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
            ble_data = param->data;
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

    int y1 = ble_data;

    if (is_connected) {
        // ジョイスティックのY値でモーターをオンオフする
        if ((y1 > 0) && (y1 < 1024)) {
            float pow = (1024 - y1) / 1024.0f;  // 0.0f ~ 1.0f

            int w = pow * 255;
            LED_setColor(0, w, w, w);

            // モーターをちょっと回してみる
            Motor_setSpeed(0, pow * 0.9f);
            Motor_setSpeed(1, pow * 0.9f);
            Motor_setSpeed(2, pow * 0.9f);
            Motor_setSpeed(3, pow * 0.9f);
        } else {
            LED_setColor(0, 0, 0, 100);

            // モーターを止める
            Motor_setSpeed(0, 0);
            Motor_setSpeed(1, 0);
            Motor_setSpeed(2, 0);
            Motor_setSpeed(3, 0);
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
