#include "UGOKU_Pad_Controller.hpp"   // BLE helper（UGOKU Pad 用の制御クラス）
#include "ESP32Servo.h"               // ESP32 用サーボライブラリ

UGOKU_Pad_Controller controller;      // UGOKU Pad 向け BLE コントローラインスタンス

// 接続ピン定義
#define PIN_SERVO_1 12       // サーボ1 出力ピン
#define PIN_SERVO_2 14       // サーボ2 出力ピン
#define PIN_ANALOG_READ 26   // アナログ入力（距離センサなど）
#define PIN_LED 27           // ステータス LED

Servo servo1;
Servo servo2;

bool isConnected = false;    // BLE 接続状態フラグ

// 受信データのデフォルト値（0xFF は「未受信」を表す）
uint8_t stick_2 = 90;
uint8_t stick_3 = 90;
uint8_t btn_1 = 0xFF;    
uint8_t prev_btn_1 = 0xFF;

// 受信したチャンネルの値を一度だけ読み、有効(0xFF 以外)かつ変化があれば指定変数へ反映するヘルパー関数
static inline void updateFromChannel(uint8_t ch, uint8_t &var) {
  uint8_t v = controller.getDataByChannel(ch);
  if (v != 0xFF && v != var) var = v;
}

void setup() {
  Serial.begin(115200);
  controller.setup("My ESP32");
  controller.setOnConnectCallback(onDeviceConnect);
  controller.setOnDisconnectCallback(onDeviceDisconnect);

  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.println("Waiting for a device to connect...");
}

// デバイスが接続されたときの処理
void onDeviceConnect() {
 
  Serial.println("Device connected!");
  isConnected = true;
  servo1.attach(PIN_SERVO_1, 500, 2500);
  servo2.attach(PIN_SERVO_2, 500, 2500);
  digitalWrite(PIN_LED, HIGH);
}

// デバイスが切断されたときの処理
void onDeviceDisconnect() {
  Serial.println("Device disconnected!");
  isConnected = false;
  servo1.detach();
  servo2.detach();
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  if (!isConnected) {
    delay(50);
    return;
  }

  // 19 バイトの受信パケットを読み取り、チャンネル/値のペアをパース
  uint8_t err = controller.read_data();
  if (err == no_err) {
    uint8_t pairs = controller.getLastPairsCount();
    if (pairs > 0) {
      // 必要なチャンネルのみ抜き出してアプリ変数へ反映
      const uint8_t channels[] = { 1, 2, 3 };
      uint8_t* targets[] = { &btn_1, &stick_2, &stick_3 };
      for (uint8_t i = 0; i < sizeof(channels)/sizeof(channels[0]); ++i) {
        updateFromChannel(channels[i], *targets[i]);
      }

      // ch1: LED 制御（変化時のみトグル）。
      // ここでは btn_1==1 のとき LOW、それ以外で HIGH を出力
      if (btn_1 != 0xFF && btn_1 != prev_btn_1) {
        prev_btn_1 = btn_1;
        digitalWrite(PIN_LED, (btn_1 == 1) ? LOW : HIGH);
      }
    }
  } else if (err == cs_err) {
    Serial.println("Checksum error on incoming packet");
  } else if (err == data_err) {
    Serial.println("Incoming packet length != 19");
  }

  #if 1 //サーボ駆動
  servo1.write(stick_2);
  servo2.write(stick_3);
  #endif

  #if 0 //対向2輪を1スティックで動かす
  servo1.write(stick_2 + stick_3 - 90);
  servo2.write(stick_2 - stick_3 + 90);
  #endif

  // センサ読み取り例：アナログ値から簡易的に距離へ換算し、チャンネル 5 へ返信（値域や換算式は用途に応じて調整）
  int psd = analogRead(PIN_ANALOG_READ);
  float dist = 1 / (float)psd * 30000;
  int dist_int = (int)dist;
  controller.write_data(5, dist_int);

  delay(50);
}
