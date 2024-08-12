// はじめの九歩
// I2C(Wire)でJoyStick基板と通信してみる
// I2Cを使うことで、JoyStick基板のボタン状態やスティック状態を取得することができる

#include <Arduino.h>
#include <Wire.h>  // I2C を使う場合は Wire ライブラリを使う

// フルカラーLEDを使うためのライブラリ
#include <FastLED.h>

// M5Stack用のグラフィックスライブラリ(https://github.com/m5stack/M5GFX)
#include <M5GFX.h>

// Atom JoyStickのLED(G6)
#define PIN_LED_JOYSTICK 6

// Atom JoyStickのLEDの数
#define NUM_LEDS_JOYSTICK 2

// フルカラーLEDの設定
CRGB leds_joystick[NUM_LEDS_JOYSTICK];

// M5Stack用のグラフィックスライブラリ
M5GFX gfx;

void setup() {
    // S3 は Serial ではなく USBSerial を使う必要がある（罠）
    USBSerial.begin(115200);

    // グラフィックスの初期化
    gfx.begin();

    // フルカラーLEDの初期化
    FastLED.addLeds<WS2812B, PIN_LED_JOYSTICK, GRB>(leds_joystick,
                                                    NUM_LEDS_JOYSTICK);

    // フルカラーLEDの明るさを設定する
    FastLED.setBrightness(32);

    gfx.clear();
    gfx.setCursor(0, 0);
    gfx.println("Press L/R Button");

    Wire.begin();
}

// Atom JoyStickのI2Cアドレス
#define I2C_ADDR_ATOM_JOYSTICK 0x59

// Atom JoyStickのボタン状態取得レジスタ
//   0x70 〜 0x73 までのレジスタにボタン状態が格納されている
#define I2C_BUTTON_REG 0x70

void loop() {
    uint8_t data[4] = {0};

    // I2CでAtom JoyStickのボタン状態を取得する
    {
        // ボタン状態取得レジスタにアクセスする
        Wire.beginTransmission(I2C_ADDR_ATOM_JOYSTICK);
        Wire.write(I2C_BUTTON_REG);
        Wire.endTransmission(false);
        // ボタン状態を取得する
        Wire.requestFrom(I2C_ADDR_ATOM_JOYSTICK, 4);
        // 4バイト読み込む
        for (int i = 0; i < 4; i++) {
            data[i] = Wire.read();
        }
        Wire.endTransmission();
    }

    // data[0] が左上のボタンの状態(LOW: 押されている)
    // data[1] が右上のボタンの状態(LOW: 押されている)

    // 左上のボタンが押された場合は左上LEDを点灯する
    if (data[0] == LOW) {
        // 押されている
        leds_joystick[0] = CRGB::White;
    } else {
        // 押されていない
        leds_joystick[0] = CRGB::Black;
    }

    // 右上のボタンが押された場合は右上LEDを点灯する
    if (data[1] == LOW) {
        leds_joystick[1] = CRGB::White;
    } else {
        leds_joystick[1] = CRGB::Black;
    }
    // フルカラーLEDの状態を更新する
    FastLED.show();

    delay(10);
}
