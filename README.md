<p><span style="color:red"><strong>注意:</strong> このリポジトリは旧バージョンのため非推奨です。<br>
 <a href="https://github.com/UGOKU-Lab/UGOKU-Pad_Arduino">最新のArduinoライブラリバージョン</a> を使用してください。</span></p>

# ESP32_Arduino_for_UGOKU_Pad
ESP32をUGOKU Padで動かすArduinoのサンプルプログラムです。  

## このサンプルプログラムできること
<img src="https://github.com/user-attachments/assets/b2da444f-e0e3-46c4-aa92-2031e2f38083" width="600">

## 使用方法
#### Arduino IDE での準備
- BOARD MANAGER で ESP32 を検索して **Arduino ESP32 Boards** と **esp32** をインストールする  
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


## 新しいチャンネルの追加方法
ここでは「UGOKU Padから新しい値を受け取ってGPIOを操作する（入力: UGOKU Pad → ESP32）」と、「ESP32から新しいデータを送る（出力: ESP32 → UGOKU Pad）」の2パターンを、実際の `.ino` コードを引用しながら説明します。

事前知識
- チャンネル番号 chX ごとに 0..255 の値が送られてきます（受信時に 0xFF は「未更新」を意味）。
- 本スケッチの受信は、配列でチャネル → 変数へまとめて反映しています。

### 1) 入力（UGOKU Pad → ESP32）にチャンネルを1つ追加する例

例: ch4 で新しいLEDを GPIO25 に接続してON/OFFしたい。

① 変数を用意（グローバル）

```cpp
// 既存
uint8_t btn_1 = 0xFF;    // ch1
uint8_t prev_btn_1 = 0xFF;

// 追加（ch4用）
uint8_t btn_4 = 0xFF;    // ch4
uint8_t prev_btn_4 = 0xFF;
```

② ピン初期化を追加（setup 内）

```cpp
const int PIN_LED2 = 25;   // 追加LED

void setup() {
	// ...既存...
	pinMode(PIN_LED2, OUTPUT);
	digitalWrite(PIN_LED2, LOW);
}
```

③ 受信マッピング配列に ch4 を追加（loop 内）

```cpp
// 既存
const uint8_t channels[] = { 1, 2, 3 };
uint8_t* targets[] = { &btn_1, &stick_2, &stick_3 };

// 追加後（ch4を末尾に加える）
const uint8_t channels[] = { 1, 2, 3, 4 };
uint8_t* targets[] = { &btn_1, &stick_2, &stick_3, &btn_4 };
```
| 順序 | チャンネル | 対応変数 |
|---|---|---|
| 1 | ch1 | btn_1 |
| 2 | ch2 | stick_2 |
| 3 | ch3 | stick_3 |
| 4 | ch4 | btn_4 |


④ 変化時のみの処理を追加（loop 内、既存の ch1 LED と同じ書き方）

```cpp
// ch4: 変化があったときだけGPIO25を切り替え
if (btn_4 != 0xFF && btn_4 != prev_btn_4) {
	prev_btn_4 = btn_4;
	digitalWrite(PIN_LED2, (btn_4 == 1) ? LOW : HIGH);
}
```

### 2) 出力（ESP32 → UGOKU Pad）にチャンネルを1つ追加する例

例: 可変抵抗（ポテンショメータ）を ADC1 の PIN34 に接続し、値を ch6 で送る。

① ピン初期化（setup 内）

```cpp
const int PIN_POT = 34;   // ADC1
```

② 値を読む → 0..255へスケール → 送信（loop 内）

```cpp
int raw = analogRead(PIN_POT);      // 0..4095 程度
uint8_t v = map(raw, 0, 4095, 0, 255);
controller.write_data(6, v);        // ch6 に送信
```

