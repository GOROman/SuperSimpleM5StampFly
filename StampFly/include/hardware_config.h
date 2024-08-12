#pragma once

#define STAMPFLY (1)

// ----------------------------------------------------------------
#if STAMPFLY

// ブザー関連 ----------------------------------
#define PIN_BEEP 40  // StampFlyのブザー(G40)

// LED関連 -------------------------------------
#define PIN_LED  39  // StampFlyのLED(G39)
#define NUM_LEDS 2   // StampFlyのLEDの数

// モーター関連 ---------------------------------
#define PIN_MOTOR_FL 5   // StampFlyの前左モーター(G5)
#define PIN_MOTOR_FR 42  // StampFlyの前右モーター(G42)
#define PIN_MOTOR_RL 10  // StampFlyの後左モーター(G10)
#define PIN_MOTOR_RR 41  // StampFlyの後右モーター(G41)

#endif
