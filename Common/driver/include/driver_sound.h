#pragma once

// サウンドパラメータ
typedef struct {
    int16_t freq;      // 周波数(-1 で終了)
    int16_t duration;  // 持続時間(ms)
} SoundParams_t;

static TaskHandle_t _soundTaskHandle = NULL;  // タスクハンドル

// タスク用の関数
static void _Sound_Task(void* param) {
    SoundParams_t* paramsArray = (SoundParams_t*)param;

    // 番兵は-1
    while (paramsArray->freq >= 0) {
        // 音を出す
        tone(PIN_BEEP, paramsArray->freq, paramsArray->duration);

        // 指定時間待つ
        vTaskDelay(paramsArray->duration / portTICK_PERIOD_MS);

        paramsArray++;
    }
    // 音を停止する
    tone(PIN_BEEP, 0, 0);

    // タスク終了後、タスクハンドルをリセット
    _soundTaskHandle = NULL;
    vTaskDelete(NULL);
}

static void _create_sound_task(SoundParams_t* paramsArray) {
    // 再生中は無視
    if (_soundTaskHandle != NULL) {
        return;
    }

    // サウンドタスクを作成し、コア1で実行
    xTaskCreatePinnedToCore(_Sound_Task, "Sound Task", 2048, paramsArray, 1,
                            &_soundTaskHandle, 1);
}

// サウンドの初期化
void Sound_init() {}

// サウンドの更新
void Sound_update() {}

// プリセット音の再生
void Sound_play(uint8_t index) {
    // プリセット音
    static SoundParams_t paramsArray[] = {
        {440, 100}, {400, 100}, {800, 100}, {880, 200}, {-1, -1},
    };

    // サウンドタスクを作成
    _create_sound_task(paramsArray);
}

void Sound_beep(uint16_t freq, uint32_t duration) {
    static SoundParams_t paramsArray[] = {
        {freq, duration},
        {-1, -1},
    };

    // サウンドタスクを作成
    _create_sound_task(paramsArray);
}
