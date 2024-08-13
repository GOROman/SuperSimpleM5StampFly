// はじめの二十歩
// ジョイスティックに押された瞬間(wasPressed)と離された瞬間(wasReleased)を取る処理を追加

#include <Arduino.h>
#include <M5GFX.h>

// BLEライブラリ
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define BLE_CENTRAL 1

// ../Common/Driver/include/...
#include "driver_battery.h"  // バッテリドライバ
#include "driver_ble.h"      // BLEドライバ
#include "driver_button.h"   // ボタンドライバ
#include "driver_i2c.h"      // I2Cドライバ
#include "driver_joy.h"      // JoyStickドライバ
#include "driver_led.h"      // LEDドライバ
#include "driver_sound.h"    // サウンドドライバ
#include "driver_timer.h"    // Timerドライバ

M5GFX    gfx;
M5Canvas canvas(&gfx);  // キャンバス

void setup() {
    USBSerial.begin(115200);

    // 各ドライバーの初期化
    LED_init();         // フルカラーLEDの初期化
    I2C_init();         // I2Cの初期化
    Joy_init();         // JoyStickの初期化
    Sound_init();       // サウンドの初期化
    Button_init();      // ボタンの初期化
    Battery_init();     // バッテリの初期化
    Timer_init(16667);  // タイマーの初期化(16667us = 16ms)

    // グラフィックスの初期化
    gfx.begin();

    // キャンバスの作成
    canvas.createSprite(gfx.width(), gfx.height());

    delay(100);
    Sound_play(SOUND_PRESET_BOOT);

    gfx.setCursor(0, 10);
    gfx.setFont(&fonts::Font2);
    gfx.printf("Atom JoyStick\n");
    delay(500);

    // ファームウェアバージョンの取得と表示
    uint8_t fw_ver = Joy_getFirmwareVersion();
    gfx.printf("Firmware ver.% d\n\n ", fw_ver);
    gfx.setTextSize(2, 2);
    delay(500);

    BLE_init();  // BLEの初期化

    BLE_connect();  // BLE接続

    do {
        gfx.setTextColor(TFT_BLUE);
        gfx.setCursor(4, 48);
        gfx.printf("BLE Scan");
        delay(200);

        gfx.setTextColor(TFT_WHITE);
        gfx.setCursor(4, 48);
        gfx.printf("BLE Scan");
        delay(100);

        // ボタン押下で強制スキャン終了
        Button_update();
        if (Button_isPressed()) {
            break;
        }
    } while (!BLE_isConnected());

    // 接続したら音を鳴らす
    Sound_play(SOUND_PRESET_CONNECTED);
}

int64_t elapsedTime = 0;

void loop() {
    Timer_sync();  // フレーム同期

    // I2C通信の更新
    I2C_update();

    // ボタンの状態を更新する
    Button_update();

    // JoyStickの状態を更新する(I2C受信)
    Joy_update();

    // バッテリの状態を更新する
    Battery_update();

    // 左上のボタンが押された場合は左上LEDを点灯する
    if (Joy_wasPressed(BUTTON_TRIG_L)) {
        Sound_beep(1000, 100);
        LED_setColor(0, 255, 255, 255);
    } else {
        LED_setColor(0, 0, 0, 0);
    }

    // 右上のボタンが押された場合は右上LEDを点灯する
    if (Joy_wasPressed(BUTTON_TRIG_R)) {
        Sound_beep(1000, 100);
        LED_setColor(1, 255, 255, 255);
    } else {
        LED_setColor(1, 0, 0, 0);
    }

    const int W = gfx.width();
    const int H = gfx.height();

    canvas.clear();
    canvas.setCursor(0, 0);
    canvas.printf("%06d %4.4fms\nButton: %d%d%d%d\nBat1:%4.2fV Bat2:%4.2fV ",
                  Timer_getFrameCount(), (float)elapsedTime / 1000.0f,
                  Joy_isPressed(BUTTON_TRIG_L), Joy_isPressed(BUTTON_TRIG_R),
                  Joy_isPressed(BUTTON_STICK_L), Joy_isPressed(BUTTON_STICK_R),
                  Battery_getVoltage(0), Battery_getVoltage(1));

    canvas.drawFastHLine(0, H / 2, W, TFT_WHITE);
    canvas.drawFastVLine(W / 2, 0, H, TFT_WHITE);

    // JoyStickの状態(X,Y)を表示する
    canvas.setCursor(0, 30);
    uint16_t x1, y1, x2, y2;
    Joy_getAxis(STICK_L, &x1, &y1);  // 12bit: 0-4095
    Joy_getAxis(STICK_R, &x2, &y2);

    // JoyStickの生データを表示する
    canvas.printf("Joy1: X:%4d Y:%4d\n", x1, y1);
    canvas.printf("Joy2: X:%4d Y:%4d\n", x2, y2);

    // JoyStickの位置を描画する
    canvas.fillCircle(map(x1, 0, 4095, 0, W - 1), map(y1, 0, 4095, 0, H - 1), 5,
                      TFT_RED);
    canvas.fillCircle(map(x2, 0, 4095, 0, W - 1), map(y2, 0, 4095, 0, H - 1), 5,
                      TFT_BLUE);

    // 画面を押した瞬間だけ音を鳴らすテスト
    if (Button_wasPressed()) {
        Sound_beep(4000, 50);
    }

    // BLEの更新
    BLE_update(y1);  // y1 の値をテストで送信

    LED_update();    // フルカラーLEDの更新
    Timer_update();  // タイマーの更新
    Sound_update();  // サウンドの更新

    // キャンバスの描画(ちらつき防止)
    gfx.startWrite();
    canvas.pushSprite(0, 0);
    gfx.endWrite();

    // 処理負荷計測
    elapsedTime = Timer_getElapsedTime();
}
