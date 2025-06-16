#include "UGOKU_Pad_Controller.hpp"   // Include the custom controller header for BLE handling
#include "ESP32Servo.h"               // Include the ESP32 Servo library
#include "MotorDriver.h"

UGOKU_Pad_Controller controller;      // Instantiate the UGOKU Pad Controller object

<<<<<<< HEAD:main.cpp
Servo servo1;                         // Create a Servo object
Servo servo2;                         // Create a Servo object

// Store previous values to detect changes
int lastCH = -1;
int lastVAL = -1;

float duty = 0.0f;
=======
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
>>>>>>> remotes/origin/for_UGOKU-Pad_2:ESP32_Arduino_for_UGOKU_Pad.ino

// Function called when a BLE device connects
void onDeviceConnect() {
  Serial.println("Device connected!");  // Print connection message

  isConnected = true;                   // Set the connection flag to true

  // Attach the servo to the defined pin, with pulse range between 500 to 2500 microseconds
<<<<<<< HEAD:main.cpp
  servo1.attach(14, 500, 2500);  
  servo2.attach(27, 500, 2500); 
      
  digitalWrite(2, HIGH);
=======
  servo1.attach(PIN_SERVO_1, 500, 2500);  
  servo2.attach(PIN_SERVO_2, 500, 2500); 

  digitalWrite(PIN_LED, HIGH);
>>>>>>> remotes/origin/for_UGOKU-Pad_2:ESP32_Arduino_for_UGOKU_Pad.ino
}

// Function called when a BLE device disconnects
void onDeviceDisconnect() {
  Serial.println("Device disconnected!");  // Print disconnection message

  isConnected = false;                     // Set the connection flag to false

  servo1.detach();                       // Detach the servo to stop controlling it
  servo2.detach(); 
<<<<<<< HEAD:main.cpp
=======

  digitalWrite(PIN_LED, LOW);
>>>>>>> remotes/origin/for_UGOKU-Pad_2:ESP32_Arduino_for_UGOKU_Pad.ino
}

void setup() {
  Serial.begin(115200);               // Initialize the serial communication with a baud rate of 115200

  // Setup the BLE connection
  controller.setup("UGOKU One");       // Set the BLE device name to "My ESP32"

  // Set callback functions for when a device connects and disconnects
  controller.setOnConnectCallback(onDeviceConnect);   // Function called on device connection
  controller.setOnDisconnectCallback(onDeviceDisconnect);  // Function called on device disconnection

  Serial.println("Waiting for a device to connect...");  // Print waiting message

  MotorDriver_begin();

  // Setup the servo
  servo1.setPeriodHertz(50);          // Set the servo PWM frequency to 50Hz (typical for servos)
  servo2.setPeriodHertz(50);

  // Setup the I/O pins
  pinMode(2, OUTPUT);  
  pinMode(4, OUTPUT);
  pinMode(23, OUTPUT);      
}



void loop() {
  // Only run if a device is connected via BLE
  
  if (isConnected) {

<<<<<<< HEAD:main.cpp
    // Only process if the value has changed
    if (CH != lastCH || VAL != lastVAL) {
      lastCH = CH;
      lastVAL = VAL;

      // Debug output (only prints if changed)
      Serial.print("Channel: ");
      Serial.print(CH);
      Serial.print("  Value: ");
      Serial.println(VAL);

      digitalWrite(4, 1);
      digitalWrite(23, 1);
      

      // Control devices based on the received channel
      switch (CH) {
        /*
        case 1: // LED control
          digitalWrite(2, (VAL == 1) ? LOW : HIGH);
          break;
        case 2: // Servo 1
          servo1.write(VAL);
          break;
        */
        case 3: // Servo 2
          servo2.write(VAL);
          break;
        case 4: // Motor 1
          duty = (VAL / 127.5f) - 1.0f;
          MotorDriver_setSpeed(MD1, duty);
          break;
        /*
          case 5: // Motor 2
          duty = (VAL / 127.5f) - 1.0f;
          MotorDriver_setSpeed(MD2, duty);
          break;
        */
      }
      printf("%f\n\r",duty);

    }

    /*
    int psd = analogRead(26);
=======
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
>>>>>>> remotes/origin/for_UGOKU-Pad_2:ESP32_Arduino_for_UGOKU_Pad.ino
    float dist = 1 / (float)psd * 30000;  // Conversion of analogue values to cm
    int dist_int = (int)dist;
    //Serial.print("dist_int = ");
    //Serial.println(dist_int);
    controller.write_data(5,dist_int);
<<<<<<< HEAD:main.cpp
    */

=======
>>>>>>> remotes/origin/for_UGOKU-Pad_2:ESP32_Arduino_for_UGOKU_Pad.ino
  }
  delay(50);  // Add a small delay to reduce the loop frequency
}
