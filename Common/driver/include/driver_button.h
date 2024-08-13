#pragma once
// ボタンドライバー

#include "hardware_config.h"

static bool _Button_lastState = false;
static bool _Button_isPressed = false;

// ボタン初期化
void Button_init() { pinMode(PIN_BUTTON, INPUT); }

// ボタン更新
void Button_update() {
    _Button_lastState = _Button_isPressed;
    _Button_isPressed = !digitalRead(PIN_BUTTON);
}

// ボタン押下判定
bool Button_isPressed(uint8_t index = 0) {
    if (index >= NUM_BUTTONS) {
        return false;
    }

    return _Button_isPressed;
}

// ボタン押下判定(押下時のみ)
bool Button_wasPressed(uint8_t index = 0) {
    if (index >= NUM_BUTTONS) {
        return false;
    }

    return _Button_isPressed && !_Button_lastState;
}

// ボタン押下判定(離した時のみ)
bool Button_wasReleased(uint8_t index = 0) {
    if (index >= NUM_BUTTONS) {
        return false;
    }

    return !_Button_isPressed && _Button_lastState;
}
