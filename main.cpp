#include "UGOKU_Pad_Controller.hpp"   // Include the custom controller header for BLE handling
#include "ESP32Servo.h"               // Include the ESP32 Servo library
#include "MotorDriver.h"

UGOKU_Pad_Controller controller;      // Instantiate the UGOKU Pad Controller object
uint8_t CH;                           // Variable to store the channel number
uint8_t VAL;                          // Variable to store the value for the servo control

bool isConnected = false;             // Boolean flag to track BLE connection status

Servo servo1;                         // Create a Servo object
Servo servo2;                         // Create a Servo object

// Store previous values to detect changes
int lastCH = -1;
int lastVAL = -1;

float duty = 0;

// Function called when a BLE device connects
void onDeviceConnect() {
  Serial.println("Device connected!");  // Print connection message

  isConnected = true;                   // Set the connection flag to true

  // Attach the servo to the defined pin, with pulse range between 500 to 2500 microseconds
  servo1.attach(14, 500, 2500);  
  servo2.attach(27, 500, 2500); 
      
  digitalWrite(2, HIGH);
}

// Function called when a BLE device disconnects
void onDeviceDisconnect() {
  Serial.println("Device disconnected!");  // Print disconnection message

  isConnected = false;                     // Set the connection flag to false

  servo1.detach();                       // Detach the servo to stop controlling it
  servo2.detach(); 
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
    controller.read_data();             // Read data from the BLE device
    CH = controller.get_ch();           // Get the channel number from the controller
    VAL = controller.get_val();         // Get the value (servo position or other data)

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
        case 1: // LED control
          digitalWrite(2, (VAL == 1) ? LOW : HIGH);
          break;
        case 2: // Servo 1
          servo1.write(VAL);
          break;
        case 3: // Servo 2
          servo2.write(VAL);
          break;
        case 4: // Motor 1
          duty = (VAL / 127.5f) - 1.0f;
          MotorDriver_setSpeed(MD1, duty);
          break;
        case 5: // Motor 2
          duty = (VAL / 127.5f) - 1.0f;
          MotorDriver_setSpeed(MD2, duty);
          break;
      }
      printf("%f\n\r",duty);

    }

    /*
    int psd = analogRead(26);
    float dist = 1 / (float)psd * 30000;  // Conversion of analogue values to cm
    int dist_int = (int)dist;
    controller.write_data(5,dist_int);
    */

  }
  delay(50);  // Add a small delay to reduce the loop frequency
}
