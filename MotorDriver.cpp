#include "MotorDriver.h"

// GPIO pins for motor inputs
// MD1: IN1 = GPIO32, IN2 = GPIO33
// MD2: IN1 = GPIO5,  IN2 = GPIO13
static const int MD1_IN1 = 32;
static const int MD1_IN2 = 33;
static const int MD2_IN1 = 5;
static const int MD2_IN2 = 13;

// PWM configuration
static const int PWM_FREQ = 20000; // 20 kHz
static const int PWM_RES  = 8;     // 8-bit resolution (0–255)

// LEDC channels
static const int CH_MD1_FWD = 0;
static const int CH_MD1_REV = 1;
static const int CH_MD2_FWD = 2;
static const int CH_MD2_REV = 3;

void MotorDriver_begin() {
    pinMode(MD1_IN1, OUTPUT);
    pinMode(MD1_IN2, OUTPUT);
    pinMode(MD2_IN1, OUTPUT);
    pinMode(MD2_IN2, OUTPUT);
    digitalWrite(MD1_IN1, LOW);
    digitalWrite(MD1_IN2, LOW);
    digitalWrite(MD2_IN1, LOW);
    digitalWrite(MD2_IN2, LOW);

    ledcSetup(CH_MD1_FWD, PWM_FREQ, PWM_RES);
    ledcSetup(CH_MD1_REV, PWM_FREQ, PWM_RES);
    ledcSetup(CH_MD2_FWD, PWM_FREQ, PWM_RES);
    ledcSetup(CH_MD2_REV, PWM_FREQ, PWM_RES);

    ledcAttachPin(MD1_IN1, CH_MD1_FWD);
    ledcAttachPin(MD1_IN2, CH_MD1_REV);
    ledcAttachPin(MD2_IN1, CH_MD2_FWD);
    ledcAttachPin(MD2_IN2, CH_MD2_REV);
}

void MotorDriver_setSpeed(MotorCh ch, float dutyRatio) {
    // Clamp dutyRatio to -1.0 … +1.0
    if (dutyRatio >  1.0f) dutyRatio =  1.0f;
    if (dutyRatio < -1.0f) dutyRatio = -1.0f;

    // Compute duty cycle 0–255
    uint8_t duty = (uint8_t)(MAX_DUTY * fabs(dutyRatio) * 255.0f);

    // Select channels
    int chFwd = (ch == MD1) ? CH_MD1_FWD : CH_MD2_FWD;
    int chRev = (ch == MD1) ? CH_MD1_REV : CH_MD2_REV;

    if (dutyRatio > 0.0f) {
        // Forward: apply PWM to IN1, IN2 low
        ledcWrite(chFwd, duty);
        ledcWrite(chRev, 0);
    } else if (dutyRatio < 0.0f) {
        // Reverse: IN1 low, PWM to IN2
        ledcWrite(chFwd, 0);
        ledcWrite(chRev, duty);
    } else {
        // Stop: both low
        ledcWrite(chFwd, 0);
        ledcWrite(chRev, 0);
    }
}
