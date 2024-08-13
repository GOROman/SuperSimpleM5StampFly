# SuperSimpleM5StampFly

M5Stack社が発売した StampFly と AtomJoyStick のファームウェアをゼロから書いてみよう！！という無謀な試み。飛んだら成功！飽きたら終了！

## 方針

- なるべくシンプルにする
- IMU姿勢制御して飛ばすところまでやる(ToFとかOpiticalFlowとか気圧とかは諦める)
- デバイスごとにドライバを分ける
- StampFly と AtomJoyStick で共通化できる部分は共通化する

## ToDo

- [ ] アホほどある


## 参考資料

- StampFly & Atom ジョイスティック ファームウェア書き込みガイド https://docs.m5stack.com/ja/guide/hobby_kit/stampfly/stamply_firmware

### M5 Stamp Fly関連

- オリジナルファームウェア https://github.com/m5stack/M5StampFly

|仕様|概要|
|----|----|
|M5StampS3|ESP32-S3@Xtensa LX7、8 MB-FLASH、Wi-Fi、OTG/CDC support|
|距離センサ|VL53L3CXV0DH/1 (0x52) @ 最大3m|
|オプティカルフローセンサ|PMW3901MB-TXQT|
|気圧センサ|BMP280（0x76）@ 300-1100 hPa|
|3軸磁力センサ|BMM150（0x10)|
|6軸IMUセンサ|BMI270|
|バッテリー|300 mAh 高電圧リチウムポリマーバッテリ（LiHV）|
|電流電圧計|INA3221AIRGVR（0x40）|
|ブザー|Built-in Buzzer @ 5020|
|製品サイズ|107 x 107 x 30 mm|
|製品重量|36.2 g|

### M5 Atom JoyStick関連

- オリジナルファームウェア https://github.com/m5stack/Atom-JoyStick

|仕様|概要|
|----|----|
|マイコン|STM32F030F4P6|
|RGB LED|WS2812C|
|充電IC|TP4067 4.35 V|
|バッテリ|300 mAh 高電圧リチウムポリマーバッテリ（LiHV）|
|充電電流|	5V / 500 mA|
|ブザー|	Built-in Buzzer @ 5020|
|製品サイズ|84 x 60 x 30 mm|
|製品重量|63.5g|
