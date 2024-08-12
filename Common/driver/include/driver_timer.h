#pragma once

hw_timer_t*       _timer = NULL;
SemaphoreHandle_t _semaphore;

// ペリフェラルクロック
#define TIMER_DIVIDER    80        // 80分周
#define PERIPHERAL_CLOCK 80000000  // 80MHz
#define TIMER_ID         0         // 0 〜 3

// タイマー割り込み関数
void IRAM_ATTR onTimer() {
    // セマフォを解放
    xSemaphoreGiveFromISR(_semaphore, NULL);
}

// タイマーの初期化
void Timer_init(uint32_t us) {
    // セマフォの作成
    _semaphore = xSemaphoreCreateBinary();

    // ハードウェアタイマーの設定
    // 80MHz / 80分周 = 1μs, アップカウンター
    _timer = timerBegin(TIMER_ID, TIMER_DIVIDER, true);

    // タイマー割り込み関数を設定
    timerAttachInterrupt(_timer, &onTimer, true);

    // 指定時間ごとに割り込みを発生させる
    timerAlarmWrite(_timer, us, true);

    // タイマーを有効化
    timerAlarmEnable(_timer);
}

// タイマーの更新
void Timer_update() {}

// タイマーの同期
void Timer_sync() {
    // タイマー割込 待機
    xSemaphoreTake(_semaphore, portMAX_DELAY);
}