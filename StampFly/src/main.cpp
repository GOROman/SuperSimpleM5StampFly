#include <Arduino.h>

// フルカラーLEDを使うためのライブラリ
#include <FastLED.h>

// M5StampS3のLED(G21)
#define PIN_LED_STAMPS3 21
// M5StampFlyのLED(G39)
#define PIN_LED_STAMPFLY 39

// M5StampS3のLEDの数
#define NUM_LEDS_STAMPS3 1
// M5StampFlyのLEDの数
#define NUM_LEDS_STAMPFLY 2

// M5StampFlyのブザーのピン(G40)
#define PIN_BUZZER 40

// フルカラーLEDの設定
CRGB leds_stamps3[NUM_LEDS_STAMPS3];
CRGB leds_stampfly[NUM_LEDS_STAMPFLY];

void setup() {
    // S3 は Serial ではなく USBSerial を使う必要がある（罠）
    USBSerial.begin(115200);

    // フルカラーLEDの初期化
    FastLED.addLeds<WS2812B, PIN_LED_STAMPS3, GRB>(leds_stamps3,
                                                   NUM_LEDS_STAMPS3);
    FastLED.addLeds<WS2812B, PIN_LED_STAMPFLY, GRB>(leds_stampfly,
                                                    NUM_LEDS_STAMPFLY);
    // ブザー用のピンを出力に設定する
    pinMode(PIN_BUZZER, OUTPUT);

    // ブザーを鳴らす(200ms)
    tone(PIN_BUZZER, 262, 200);  // ドの音
    tone(PIN_BUZZER, 294, 200);  // レの音
    tone(PIN_BUZZER, 330, 200);  // ミの音

    delay(1000);
}

void loop() {
    USBSerial.println("Hello, StampFly!");

    // 本体のLEDを点灯する
    leds_stamps3[0] = CRGB::White;
    leds_stampfly[0] = CRGB::Black;
    leds_stampfly[1] = CRGB::Black;

    // フルカラーLEDを更新する
    FastLED.show();

    // ブザーを鳴らす(200ms)
    tone(PIN_BUZZER, 440, 100);

    // 1000ms待つ (1秒)
    delay(500);

    // フルカラーLEDを青色にする
    leds_stamps3[0] = CRGB::Black;
    leds_stampfly[0] = CRGB::Red;
    leds_stampfly[1] = CRGB::Blue;

    // フルカラーLEDを更新する
    FastLED.show();

    // ブザーを鳴らす(200ms)
    tone(PIN_BUZZER, 880, 100);

    // 1000ms待つ (1秒)
    delay(500);
}
