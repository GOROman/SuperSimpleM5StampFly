#pragma once

#include <FastLED.h>

// Atom JoyStickのLED(G6)
#define PIN_LED 6

// Atom JoyStickのLEDの数
#define NUM_LEDS 2

// フルカラーLEDの設定
CRGB leds[NUM_LEDS];

// フルカラーLEDの初期化
void LED_init() {
    FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);

    // フルカラーLEDの明るさを設定する
    FastLED.setBrightness(32);
}

// フルカラーLEDの状態を更新する
void LED_update() { FastLED.show(); }

// フルカラーLEDの色を設定する
void LED_setColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= NUM_LEDS) {
        return;
    }
    leds[index] = CRGB(r, g, b);
}
