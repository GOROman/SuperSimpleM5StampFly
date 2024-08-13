#pragma once

#define ATOMJOYSTICK (1)

// ----------------------------------------------------------------
#if ATOMJOYSTICK

// ブザー関連 ----------------------------------
#define PIN_BEEP 5  // Atom JoyStickのブザー(G5)

// LED関連 -------------------------------------
#define PIN_LED  6  // Atom JoyStickのLED(G6)
#define NUM_LEDS 2  // Atom JoyStickのLEDの数

// JoyStick関連 --------------------------------
// Atom JoyStickのI2Cアドレス
#define I2C_ADDR_ATOM_JOYSTICK 0x59

// Atom JoyStickのレジスタ
#define I2C_REG_JOY1_ADC_VALUE_12BITS 0x00
#define I2C_REG_JOY2_ADC_VALUE_12BITS 0x20
#define I2C_REG_BUTTON                0x70
#define I2C_REG_FIRMWARE_VERSION      0xFE

#endif
