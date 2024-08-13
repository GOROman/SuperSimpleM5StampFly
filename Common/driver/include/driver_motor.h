#pragma once

// モータードライバ

// モーター定義
enum {
    MOTOR_FL,  // [A] Front Left
    MOTOR_FR,  // [B] Front Right
    MOTOR_RL,  // [C] Rear Left
    MOTOR_RR,  // [D] Rear Right

    MOTOR_MAX
};

// モーターのGPIOピンアサイン(hardware_config.hで設定)
static const int MOTOR_PINS[] = {
    PIN_MOTOR_FL,  // [A] FL
    PIN_MOTOR_FR,  // [B] FR
    PIN_MOTOR_RL,  // [D] RL
    PIN_MOTOR_RR,  // [C] RR
};

// PWM設定
const int MOTOR_PWM_FREQ = 150000;  // 150kHz
const int MOTOR_PWM_BIT_NUM = 8;    // 8bit (PWM Reso)

// モーターの速度
static float _motorSpeed[MOTOR_MAX] = {0};

// モーター初期化
void Motor_init() {
    for (int i = 0; i < MOTOR_MAX; i++) {
        const int PWM_CH = i;

        // PWM設定
        ledcSetup(PWM_CH, MOTOR_PWM_FREQ, MOTOR_PWM_BIT_NUM);

        // PWMピン設定
        ledcAttachPin(MOTOR_PINS[i], PWM_CH);
    }
}

// モーター更新
void Motor_update() {
    for (int i = 0; i < MOTOR_MAX; i++) {
        const int PWM_CH = i;

        // PWM Duty比設定
        const uint32_t MAX = (1 << MOTOR_PWM_BIT_NUM) - 1;
        uint32_t       duty = (uint32_t)(MAX * _motorSpeed[i]);

        ledcWrite(PWM_CH, duty);
    }
}

// モーター速度設定
void Motor_setSpeed(int index, float duty) {
    if (index < 0 || index >= MOTOR_MAX) {
        return;
    }
    _motorSpeed[index] = duty;
}
