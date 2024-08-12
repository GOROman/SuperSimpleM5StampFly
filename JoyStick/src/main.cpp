// はじめの十一歩
// JoyStickドライバを作成。ジョイスティックの状態を取得

#include <Arduino.h>
#include <M5GFX.h>

M5GFX gfx;

#include "driver_i2c.h"  // I2Cドライバ
#include "driver_joy.h"  // JoyStickドライバ
#include "driver_led.h"  // LEDドライバ

void setup() {
    USBSerial.begin(115200);

    // 各ドライバーの初期化
    LED_init();  // フルカラーLEDの初期化
    I2C_init();  // I2Cの初期化
    Joy_init();  // JoyStickの初期化

    // グラフィックスの初期化
    gfx.begin();
}

void loop() {
    gfx.clear();

    // I2C通信の更新
    I2C_update();

    // JoyStickの状態を更新する(I2C受信)
    Joy_update();

    // 左上のボタンが押された場合は左上LEDを点灯する
    if (Joy_isPressed(BUTTON_TRIG_L)) {
        LED_setColor(0, 255, 255, 255);
    } else {
        LED_setColor(0, 0, 0, 0);
    }

    // 右上のボタンが押された場合は右上LEDを点灯する
    if (Joy_isPressed(BUTTON_TRIG_R)) {
        LED_setColor(1, 255, 255, 255);
    } else {
        LED_setColor(1, 0, 0, 0);
    }

    gfx.setCursor(0, 0);
    gfx.printf("Button: %d %d %d %d", Joy_isPressed(BUTTON_TRIG_L),
               Joy_isPressed(BUTTON_TRIG_R), Joy_isPressed(BUTTON_STICK_L),
               Joy_isPressed(BUTTON_STICK_R));

    // JoyStickの状態(X,Y)を表示する
    gfx.setCursor(0, 30);
    uint16_t x, y;
    Joy_getAxis(STICK_L, &x, &y);  // 12bit: 0-4095
    gfx.printf("Joy1: %4d %4d\n", x, y);
    Joy_getAxis(STICK_R, &x, &y);
    gfx.printf("Joy2: %4d %4d\n", x, y);

    // フルカラーLEDの状態を更新する
    LED_update();

    delay(10);
}
