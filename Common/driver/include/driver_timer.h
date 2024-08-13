#pragma once

// タイマードライバー

#define TIMER_DIVIDER    80        // 80分周
#define PERIPHERAL_CLOCK 80000000  // 80MHz
#define TIMER_ID         0         // 0 〜 3

static hw_timer_t*       _timer = NULL;  // タイマー
static SemaphoreHandle_t _semaphore;     // セマフォ
static uint32_t          _framecount;    // カウンタ
static int64_t           _baseTime;      // 基準時間

// タイマー割り込み関数
static void IRAM_ATTR _onTimer() {
    // セマフォを解放
    xSemaphoreGiveFromISR(_semaphore, NULL);
}

// タイマーの初期化
void Timer_init(uint32_t us) {
    _framecount = 0;

    // セマフォの作成
    _semaphore = xSemaphoreCreateBinary();

    // ハードウェアタイマーの設定
    // 80MHz / 80分周 = 1μs, アップカウンター
    _timer = timerBegin(TIMER_ID, TIMER_DIVIDER, true);

    // タイマー割り込み関数を設定
    timerAttachInterrupt(_timer, &_onTimer, true);

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
    _baseTime = esp_timer_get_time();  // us
    _framecount++;
}

// フレームカウント取得
uint32_t Timer_getFrameCount() { return _framecount; }

// sync後の経過時間(us)取得
int64_t Timer_getElapsedTime() { return esp_timer_get_time() - _baseTime; }
