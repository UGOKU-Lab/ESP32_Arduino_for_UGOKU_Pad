# ESP32_Arduino_for_UGOKU_Pad
ESP32をUGOKU Padで動かすArduinoのサンプルプログラムです。  
## このサンプルプログラムできること
- デジタル出力の操作
- RCサーボの操作
- ローテーションサーボの操作
- PSD測距モジュール(GP2Y0A21YK)による距離表示（アナログ入力）
<img src="https://github.com/user-attachments/assets/b2da444f-e0e3-46c4-aa92-2031e2f38083" width="600">

## 使用方法
#### Arduino IDE での準備
- BOARD MANAGER で ESP32 を検索して **Arduino ESP32 Boarsds** と **esp32** をインストールする  
<img src="https://github.com/user-attachments/assets/34e671e7-9068-47e4-8431-86b137ea8c13" width="350">

　
- LIBRARY MANAGER で **ESP32Servo** を検索してインストールする  
<img src="https://github.com/user-attachments/assets/cae88e9d-53ed-4a1c-bf38-bd7de3d8b462" width="350">  

　
- Select Other Board and Port で **ESP32 Dev Module** を検索して選択する 
<img src="https://github.com/user-attachments/assets/670f19c9-2996-4bbd-9af5-0fa0ecb96540" width="350">  


## UGOKU Pad の設定
UGOKU Padをインストール

[<img src="https://github.com/user-attachments/assets/73952bbe-7f89-46e9-9a6e-cdc7eea8e7c8" alt="Get it on Google Play" height="60">](https://play.google.com/store/apps/details?id=com.ugoku_lab.ugoku_console)　[<img src="https://github.com/user-attachments/assets/e27e5d09-63d0-4a2e-9e14-0bb05dabd487" alt="Get it on Google Play" height="60">](https://apps.apple.com/jp/app/ugoku-pad/id6739496098)


Console内の「ESP32 Arduino Sample」を使用

<img src="https://github.com/user-attachments/assets/a0c7ed43-5082-4802-9647-cbb8cc861142" width="200">
<img src="https://github.com/user-attachments/assets/578605c3-9ea8-434b-b564-59bf12aa8233" width="200">


## ピン配置
| 機能 | ピン |
| ------------- | ------------- |
| デジタル出力  | 27 |
| 測距モジュール | 26 |
| RCサーボ | 14 |
| ローテーションサーボ | 12 |


## 動作確認
ESP32-WROOM-32E、ESP32-WROVER-32Eで動作確認済み


