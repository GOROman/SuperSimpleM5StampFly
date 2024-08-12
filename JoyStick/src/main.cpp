#include <Arduino.h>

// フルカラーLEDを使うためのライブラリ
#include <FastLED.h>

// Atom JoyStickのLED(G6)
#define PIN_LED_JOYSTICK 6

// Atom JoyStickのLEDの数
#define NUM_LEDS_JOYSTICK 2

// フルカラーLEDの設定
CRGB leds_joystick[NUM_LEDS_JOYSTICK];

void setup() {
    // S3 は Serial ではなく USBSerial を使う必要がある（罠）
    USBSerial.begin(115200);

    // フルカラーLEDの初期化
    FastLED.addLeds<WS2812B, PIN_LED_JOYSTICK, GRB>(leds_joystick,
                                                    NUM_LEDS_JOYSTICK);

    // フルカラーLEDの明るさを設定する
    FastLED.setBrightness(32);
}

void loop() {
    USBSerial.println("Hello, Atom JoyStick!");

    // フルカラーLEDの左を青色にする
    leds_joystick[0] = CRGB::Blue;
    leds_joystick[1] = CRGB::Black;

    // フルカラーLEDを更新する
    FastLED.show();

    // 1000ms待つ (1秒)
    delay(500);

    // フルカラーLEDの右を青色にする
    leds_joystick[0] = CRGB::Black;
    leds_joystick[1] = CRGB::Blue;

    // フルカラーLEDを更新する
    FastLED.show();

    // 1000ms待つ (1秒)
    delay(500);
}
