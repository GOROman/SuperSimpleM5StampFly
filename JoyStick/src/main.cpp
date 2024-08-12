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
}

int y = 0;

void loop() {
    // 画面を青色で塗りつぶす
    gfx.fillScreen(TFT_NAVY);

    // 画面に文字を表示する
    gfx.setTextColor(TFT_WHITE);
    gfx.setCursor(0, y);
    gfx.println("Hello, Atom JoyStick!");

    // 文字を表示する位置を変更する
    y++;
    if (y > gfx.height()) {
        y = 0;
    }

    leds_joystick[0] = CRGB::Blue;
    leds_joystick[1] = CRGB::Black;
    FastLED.show();
    delay(30);

    leds_joystick[0] = CRGB::Black;
    leds_joystick[1] = CRGB::Blue;
    FastLED.show();
    delay(30);
}
