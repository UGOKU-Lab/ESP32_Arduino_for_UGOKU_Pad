#pragma once  // Ensure the header file is included only once during compilation

#include <Arduino.h>  // Include the Arduino core library
#include <BLEDevice.h>  // Include the BLE device functionality
#include <BLEUtils.h>   // Include BLE utility functions
#include <BLEServer.h>  // Include BLE server functionality
#include "UGOKU_Pad_define.h"  // Include custom definitions for UGOKU Pad
#include "MyServerCallbacks.hpp"  // Include custom server callbacks

// UGOKU_Pad_Controller class definition
class UGOKU_Pad_Controller {
  public:
    UGOKU_Pad_Controller();  // Constructor for initializing the controller

    // Function to setup the BLE device with a specified name
    void setup(char *device_name);

    // Callback function pointers for connect and disconnect events
    void (*onConnectCallback)();
    void (*onDisconnectCallback)();

    // Function to set the connect callback
    void setOnConnectCallback(void (*callback)());

    // Function to set the disconnect callback
    void setOnDisconnectCallback(void (*callback)());

    // Function to read data from the BLE characteristic
    uint8_t read_data(void);

    // Function to write data to the BLE characteristic
    void write_data(uint8_t channel, uint8_t value);

    // Getter functions to retrieve the current channel and value
    uint8_t get_ch(void);  // Get current channel
    uint8_t get_val(void);  // Get current value

    // Setter functions to set the current channel and value
    void set_ch(uint8_t channel);  // Set the channel
    void set_val(uint8_t value);  // Set the value

    // Function to get data by specifying a channel
    uint8_t getDataByChannel(uint8_t channel);

  private:
    // Private members for BLE server, service, and characteristic
    BLEServer *pServer;  // BLE server object
    BLEService *pService;  // BLE service object
    BLECharacteristic *pCharacteristic;  // BLE characteristic object

    // Private member variables to hold the channel, value, checksum, and error number
    uint8_t ch;  // Channel ID
    uint8_t val;  // Value associated with the channel
    uint8_t cs;  // Checksum for validation
    uint8_t err_num;  // Error number

    // Array to store data for each channel
    uint8_t data_array[MAX_CHANNELS];  // Data array for storing values per channel
};
