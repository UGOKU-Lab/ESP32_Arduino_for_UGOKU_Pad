# ESP32_Arduino_for_UGOKU_Pad
ESP32をUGOKU Padで動かすArduinoのサンプルプログラムです。

### UGOKU Pad
UGOKU Pad α版: https://play.google.com/store/apps/details?id=com.ugoku_lab.ugoku_console

Console内の「ESP32 Arduino Sample」を使用

<img src="https://github.com/user-attachments/assets/a0c7ed43-5082-4802-9647-cbb8cc861142" width="200">
<img src="https://github.com/user-attachments/assets/578605c3-9ea8-434b-b564-59bf12aa8233" width="200">

### できること
- デジタル出力の操作
- RCサーボの操作
- ローテーションサーボの操作
- PSD測距モジュール(GP2Y0A21YK)による距離表示（アナログ入力）

### ピン配置
| 機能 | ピン |
| ------------- | ------------- |
| デジタル出力  | 27 |
| 測距モジュール | 26 |
| RCサーボ | 14 |
| ローテーションサーボ | 12 |

### 使用ライブラリ
- Arduino標準ライブラリ
- ESP32servo

### 動作確認
ESP32-WROOM-32E、ESP32-WROVER-Eで動作確認済み


