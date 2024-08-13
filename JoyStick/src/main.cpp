// はじめの十五歩
// Atom JoyStickの内蔵ファームウェアバージョンをI2Cで読み出してみる

#include <Arduino.h>
#include <M5GFX.h>

M5GFX    gfx;
M5Canvas canvas(&gfx);  // キャンバス

// ../Common/Driver/include/...
#include "driver_i2c.h"    // I2Cドライバ
#include "driver_joy.h"    // JoyStickドライバ
#include "driver_led.h"    // LEDドライバ
#include "driver_sound.h"  // サウンドドライバ
#include "driver_timer.h"  // Timerドライバ

void setup() {
    USBSerial.begin(115200);

    // 各ドライバーの初期化
    LED_init();         // フルカラーLEDの初期化
    I2C_init();         // I2Cの初期化
    Joy_init();         // JoyStickの初期化
    Sound_init();       // サウンドの初期化
    Timer_init(10000);  // タイマーの初期化(10000us = 10ms)

    // グラフィックスの初期化
    gfx.begin();

    // キャンバスの作成
    canvas.createSprite(gfx.width(), gfx.height());

    delay(100);
    uint8_t fw_ver =
        Joy_getFirmwareVersion();  // ファームウェアバージョンの取得

    // プリセット音 再生
    Sound_play(0);
    gfx.setCursor(0, 10);
    gfx.setFont(&fonts::Font2);
    gfx.printf("Atom JoyStick\nFirmware ver.%d\n\n", fw_ver);
    gfx.setCursor(48, 48);
    gfx.setTextSize(4, 4);
    gfx.setTextColor(TFT_SKYBLUE);
    gfx.printf("%c", fw_ver);
    delay(1000);  // 1秒待つ
}

void loop() {
    Timer_sync();  // フレーム同期

    canvas.fillScreen(0);

    // I2C通信の更新
    I2C_update();

    // JoyStickの状態を更新する(I2C受信)
    Joy_update();

    // 左上のボタンが押された場合は左上LEDを点灯する
    if (Joy_isPressed(BUTTON_TRIG_L)) {
        Sound_beep(1000, 100);
        LED_setColor(0, 255, 255, 255);
    } else {
        LED_setColor(0, 0, 0, 0);
    }

    // 右上のボタンが押された場合は右上LEDを点灯する
    if (Joy_isPressed(BUTTON_TRIG_R)) {
        Sound_beep(1000, 100);
        LED_setColor(1, 255, 255, 255);
    } else {
        LED_setColor(1, 0, 0, 0);
    }

    const int W = gfx.width();
    const int H = gfx.height();

    canvas.clear();
    canvas.setCursor(0, 0);
    canvas.printf("Button: %d %d %d %d", Joy_isPressed(BUTTON_TRIG_L),
                  Joy_isPressed(BUTTON_TRIG_R), Joy_isPressed(BUTTON_STICK_L),
                  Joy_isPressed(BUTTON_STICK_R));

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

    LED_update();    // フルカラーLEDの更新
    Timer_update();  // タイマーの更新
    Sound_update();  // サウンドの更新

    // キャンバスの描画(ちらつき防止)
    gfx.startWrite();
    canvas.pushSprite(0, 0);
    gfx.endWrite();
}
