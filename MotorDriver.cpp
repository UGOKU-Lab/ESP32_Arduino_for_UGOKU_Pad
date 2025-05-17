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
static const int CH_MD1_FWD = 4;
static const int CH_MD1_REV = 5;
static const int CH_MD2_FWD = 6;
static const int CH_MD2_REV = 7;

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
    // Clamp input to -1.0 … +1.0
    dutyRatio = constrain(dutyRatio, -1.0f, 1.0f);

    // Map absolute input [0…1] → [MIN_DUTY…MAX_DUTY], keep sign
    float absIn = fabs(dutyRatio);
    float mapped = 0.0f;
    if (absIn > 0.0f) {
        mapped = MIN_DUTY + (MAX_DUTY - MIN_DUTY) * absIn;
    }
    float finalRatio = (dutyRatio >= 0.0f) ? mapped : -mapped;

    // Compute duty 0–255
    uint8_t duty = (uint8_t)(fabs(finalRatio) * 255.0f);

    // Select PWM channels
    int chFwd = (ch == MD1) ? CH_MD1_FWD : CH_MD2_FWD;
    int chRev = (ch == MD1) ? CH_MD1_REV : CH_MD2_REV;

    if (duty == 0) {
        // Brake mode: IN1=HIGH, IN2=HIGH
        digitalWrite((ch==MD1?MD1_IN1:MD2_IN1), HIGH);
        digitalWrite((ch==MD1?MD1_IN2:MD2_IN2), HIGH);
        ledcWrite(chFwd, 0);
        ledcWrite(chRev, 0);
    }
    else if (finalRatio > 0.0f) {
        // Forward: IN1=PWM, IN2=LOW
        ledcWrite(chFwd, duty);
        ledcWrite(chRev, 0);
    }
    else {
        // Reverse: IN1=LOW, IN2=PWM
        ledcWrite(chFwd, 0);
        ledcWrite(chRev, duty);
    }
}
