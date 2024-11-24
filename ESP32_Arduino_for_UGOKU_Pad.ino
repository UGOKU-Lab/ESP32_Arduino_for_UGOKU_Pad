#include "UGOKU_Pad_Controller.hpp"   // Include the custom controller header for BLE handling
#include "ESP32Servo.h"               // Include the ESP32 Servo library

UGOKU_Pad_Controller controller;      // Instantiate the UGOKU Pad Controller object
uint8_t CH;                           // Variable to store the channel number
uint8_t VAL;                          // Variable to store the value for the servo control

bool isConnected = false;             // Boolean flag to track BLE connection status

// Define the pins
#define PIN_SERVO_1 12                  // Pin number for the servo
#define PIN_SERVO_2 14                  // Pin number for the servo
#define PIN_MIC_ANALOG 26             // Pin number for the analog microphone input
#define PIN_MIC_DIGITAL 27            // Pin number for the digital microphone input

Servo servo1;                         // Create a Servo object
Servo servo2;                         // Create a Servo object


void setup() {
  Serial.begin(115200);               // Initialize the serial communication with a baud rate of 115200

  // Setup the BLE connection
  controller.setup("BLE TEST");       // Set the BLE device name to "BLE TEST"

  // Set callback functions for when a device connects and disconnects
  controller.setOnConnectCallback(onDeviceConnect);   // Function called on device connection
  controller.setOnDisconnectCallback(onDeviceDisconnect);  // Function called on device disconnection

  Serial.println("Waiting for a device to connect...");  // Print waiting message

  // Setup the servo
  servo1.setPeriodHertz(50);          // Set the servo PWM frequency to 50Hz (typical for servos)

  // Setup the microphone pins
  pinMode(PIN_MIC_ANALOG, INPUT);     // Set the analog microphone pin as input
  pinMode(PIN_MIC_DIGITAL, OUTPUT);    // Set the digital microphone pin as input
}

// Function called when a BLE device connects
void onDeviceConnect() {
  Serial.println("Device connected!");  // Print connection message

  isConnected = true;                   // Set the connection flag to true

  // Attach the servo to the defined pin, with pulse range between 500 to 2500 microseconds
  servo1.attach(PIN_SERVO_1, 500, 2500);  
  servo2.attach(PIN_SERVO_2, 500, 2500); 
  digitalWrite(27, HIGH);
}

// Function called when a BLE device disconnects
void onDeviceDisconnect() {
  Serial.println("Device disconnected!");  // Print disconnection message

  isConnected = false;                     // Set the connection flag to false

  servo1.detach();                       // Detach the servo to stop controlling it
}

void loop() {
  // Only run if a device is connected via BLE
  if (isConnected) {
    controller.read_data();             // Read data from the BLE device
    CH = controller.get_ch();           // Get the channel number from the controller
    VAL = controller.get_val();         // Get the value (servo position or other data)

    // Print channel and value to the Serial Monitor for debugging
    Serial.print("Channel : ");
    Serial.println(CH);
    Serial.print("Value : ");
    Serial.println(VAL);
  
    // Control the servo based on the received channel
    switch(CH){
      case 1:
        if(VAL == 1){
          digitalWrite(27, LOW);
        }else{
          digitalWrite(27, HIGH);
        }
        break;
      case 2:
        servo1.write(VAL);              // Move the servo to the position specified by VAL
        break;
      case 3:
        servo2.write(VAL);
        break;
    }
    /*
    // Read the analog value from the microphone and send it back to the BLE device
    int analogVal = analogRead(PIN_MIC_ANALOG);   // Read analog microphone data
    controller.write_data(5, analogVal);          // Send the microphone data over BLE
    */
    int psd = analogRead(26);
    float dist = 1 / (float)psd * 30000;
    int dist_int = (int)dist;
    controller.write_data(5,dist_int);

  }

  delay(50);  // Add a small delay to reduce the loop frequency
}
