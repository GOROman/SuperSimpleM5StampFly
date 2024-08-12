#include <Arduino.h>

// フルカラーLEDを使うためのライブラリ
#include <FastLED.h>

// M5StampS3のLED(G21)
#define PIN_LED 21
// M5StampS3のLEDの数
#define NUM_LEDS 1

// フルカラーLEDの設定
CRGB leds[NUM_LEDS];

void setup() {
    // S3 は Serial ではなく USBSerial を使う必要がある（罠）
    USBSerial.begin(115200);

    // フルカラーLEDの初期化
    FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);
}

void loop() {
    USBSerial.println("Hello, StampFly!");

    // フルカラーLEDを赤色にする
    leds[0] = CRGB::Red;

    // フルカラーLEDを更新する
    FastLED.show();

    // 1000ms待つ (1秒)
    delay(500);

    // フルカラーLEDを青色にする
    leds[0] = CRGB::Blue;

    // フルカラーLEDを更新する
    FastLED.show();

    // 1000ms待つ (1秒)
    delay(500);
}
