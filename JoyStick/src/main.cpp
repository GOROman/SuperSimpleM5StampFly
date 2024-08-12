#include <Arduino.h>

void setup() {
    // AtomS3 は Serial ではなく USBSerial を使う必要がある（罠）
    USBSerial.begin(115200);
}

void loop() {
    USBSerial.println("Hello, Atom JoyStick!");

    // 1000ms待つ (1秒)
    delay(1000);
}
