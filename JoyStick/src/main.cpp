// はじめの十歩
// I2CとLEDの制御をドライバーとして別ファイルにしてみる

#include <Arduino.h>

// M5Stack用のグラフィックスライブラリ(https://github.com/m5stack/M5GFX)
#include <M5GFX.h>

// M5Stack用のグラフィックスライブラリ
M5GFX gfx;

// I2Cドライバ(別ファイルに分離)
#include "driver_i2c.h"

// LEDドライバ(別ファイルに分離)
#include "driver_led.h"

void setup() {
    // S3 は Serial ではなく USBSerial を使う必要がある（罠）
    USBSerial.begin(115200);

    // フルカラーLEDの初期化
    LED_init();

    // I2Cの初期化
    I2C_init();

    // グラフィックスの初期化
    gfx.begin();
}

void loop() {
    gfx.clear();

    uint8_t data[4] = {0};

    // I2CでAtom JoyStickのボタン状態を取得する
    I2C_read(I2C_ADDR_ATOM_JOYSTICK, I2C_BUTTON_REG, data, sizeof(data));

    // 左上のボタンが押された場合は左上LEDを点灯する
    if (data[0] == LOW) {
        // 押されている
        LED_setColor(0, 255, 255, 255);
    } else {
        // 押されていない
        LED_setColor(0, 0, 0, 0);
    }

    // 右上のボタンが押された場合は右上LEDを点灯する
    if (data[1] == LOW) {
        LED_setColor(1, 255, 255, 255);
    } else {
        LED_setColor(1, 0, 0, 0);
    }

    // フルカラーLEDの状態を更新する
    LED_update();
    gfx.setCursor(0, 0);
    gfx.printf("Button: %d %d %d %d", data[0], data[1], data[2], data[3]);

    delay(10);
}
