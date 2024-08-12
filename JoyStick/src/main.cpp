#include <Arduino.h>

// フルカラーLEDを使うためのライブラリ
#include <FastLED.h>

// M5Stack用のグラフィックスライブラリ(https://github.com/m5stack/M5GFX)
#include <M5GFX.h>

// Atom JoyStickのLED(G6)
#define PIN_LED_JOYSTICK 6

// Atom JoyStickのLEDの数
#define NUM_LEDS_JOYSTICK 2

// Atom JoyStickのブザー(G5)
#define PIN_BUZZER 5

// AtomS3本体のボタン(G41)
// AtomS3のボタンは画面自体がボタンになっている
#define PIN_BUTTON 41

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

    // ブザー用のピンを出力に設定する
    pinMode(PIN_BUZZER, OUTPUT);

    // ボタン用のピンを入力に設定する
    pinMode(PIN_BUTTON, INPUT);
}

void loop() {
    // ボタンの状態を取得する(押されている場合はLOW)
    bool button = digitalRead(PIN_BUTTON);

    if (button == LOW) {
        // 画面(ボタン)を押している間は白色にする
        gfx.fillScreen(TFT_WHITE);
        leds_joystick[0] = CRGB::White;
        leds_joystick[1] = CRGB::White;
        FastLED.show();
    } else {
        gfx.fillScreen(TFT_NAVY);
        leds_joystick[0] = CRGB::Black;
        leds_joystick[1] = CRGB::Black;
        FastLED.show();
    }
    delay(10);
}
