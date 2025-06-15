#include "UGOKU_Pad_Controller.hpp"   // Include the custom controller header for BLE handling
#include "ESP32Servo.h"               // Include the ESP32 Servo library

UGOKU_Pad_Controller controller;      // Instantiate the UGOKU Pad Controller object

// Define the pins
#define PIN_SERVO_1 12                  // Pin number for the servo
#define PIN_SERVO_2 14                  // Pin number for the servo
#define PIN_ANALOG_READ 26             // Pin number for the analog input
#define PIN_LED 27                     // Pin number for the LED

Servo servo1;                         // Create a Servo object
Servo servo2;                         // Create a Servo object

bool isConnected = false;             // Boolean flag to track BLE connection status

// Track last read channel/value to avoid flooding Serial output
uint8_t lastPrintedCh  = 255;
uint8_t lastPrintedVal = 255;

// Default “center” position for joystick
uint8_t stick_2 = 90;
uint8_t stick_3 = 90;

void setup() {
  Serial.begin(115200);               // Initialize the serial communication with a baud rate of 115200

  // Setup the BLE connection
  controller.setup("My ESP32");       // Set the BLE device name to "My ESP32"

  // Set callback functions for when a device connects and disconnects
  controller.setOnConnectCallback(onDeviceConnect);   // Function called on device connection
  controller.setOnDisconnectCallback(onDeviceDisconnect);  // Function called on device disconnection

  // Setup the servo
  servo1.setPeriodHertz(50);          // Set the servo PWM frequency to 50Hz (typical for servos)
  servo2.setPeriodHertz(50);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.println("Waiting for a device to connect...");  // Print waiting message
}

// Function called when a BLE device connects
void onDeviceConnect() {
  Serial.println("Device connected!");  // Print connection message

  isConnected = true;                   // Set the connection flag to true

  // Attach the servo to the defined pin, with pulse range between 500 to 2500 microseconds
  servo1.attach(PIN_SERVO_1, 500, 2500);  
  servo2.attach(PIN_SERVO_2, 500, 2500); 

  digitalWrite(PIN_LED, HIGH);
}

// Function called when a BLE device disconnects
void onDeviceDisconnect() {
  Serial.println("Device disconnected!");  // Print disconnection message

  isConnected = false;                     // Set the connection flag to false

  servo1.detach();                       // Detach the servo to stop controlling it
  servo2.detach(); 

  digitalWrite(PIN_LED, LOW);
}

void loop() {
  // Only run if a device is connected via BLE
  if (isConnected) {

    uint8_t err = controller.read_data();

    if (err == no_err) {
      uint8_t pairs = controller.getLastPairsCount();

      // If there is at least one pair, find out which channels changed
      if (pairs > 0) {

        uint8_t ch1Val = controller.getDataByChannel(1);
        if (ch1Val != 0xFF && ch1Val != lastPrintedVal) {
          lastPrintedVal = ch1Val;
          // Channel 1 toggles LED
          digitalWrite(PIN_LED, (ch1Val == 1) ? LOW : HIGH);
          //Serial.print("LED control (channel 1): ");
          //Serial.println((ch1Val == 1) ? "OFF" : "ON");
        }

        uint8_t ch2Val = controller.getDataByChannel(2);
        if (ch2Val != 0xFF && ch2Val != stick_2) {
          stick_2 = ch2Val;
          //Serial.print("stick_2 updated: ");
          //Serial.println(stick_2);
        }

        uint8_t ch3Val = controller.getDataByChannel(3);
        if (ch3Val != 0xFF && ch3Val != stick_3) {
          stick_3 = ch3Val;
          //Serial.print("stick_3 updated: ");
          //Serial.println(stick_3);
        }
      }
    } else if (err == cs_err) {
      Serial.println("Checksum error on incoming packet");
    } else if (err == data_err) {
      Serial.println("Incoming packet length != 19");
    }

    servo1.write(stick_2);
    servo2.write(stick_3);

    int psd = analogRead(PIN_ANALOG_READ);
    float dist = 1 / (float)psd * 30000;  // Conversion of analogue values to cm
    int dist_int = (int)dist;
    //Serial.print("dist_int = ");
    //Serial.println(dist_int);
    controller.write_data(5,dist_int);
  }

  delay(50);  // Add a small delay to reduce the loop frequency
}
