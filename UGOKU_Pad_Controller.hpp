#pragma once  // Ensure the header file is included only once during compilation

#include <Arduino.h>  // Include the Arduino core library
#include <BLEDevice.h>  // Include the BLE device functionality
#include <BLEUtils.h>   // Include BLE utility functions
#include <BLEServer.h>  // Include BLE server functionality
#include <BLE2902.h>  // For iOS
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

    // Read exactly 19 bytes from the characteristic:
    //   [ (ch0, val0), (ch1, val1), … (ch8, val8) ] + [ checksum ]
    // Returns last error code (no_err, cs_err, data_err).
    uint8_t read_data(void);

    // Write exactly 19 bytes back to central: 9 pairs + 1 XOR-checksum
    // Usage: supply two uint8_t arrays of length 9 (channels[9], values[9]).
    // If you want to send fewer than nine (channel,value) pairs, zero out the unused slots.
    void write_data(const uint8_t channels[9], const uint8_t values[9]);

    // Convenience overload: send a single (channel, value) pair, all other slots = 0.
    void write_data(uint8_t channel, uint8_t value);

    // Retrieve the last stored value for any channel index
    uint8_t getDataByChannel(uint8_t channel);

    // How many valid (channel,value) pairs were parsed in the last read_data() call
    uint8_t getLastPairsCount(void);

  private:
    // Private members for BLE server, service, and characteristic
    BLEServer *pServer;  // BLE server object
    BLEService *pService;  // BLE service object
    BLECharacteristic *pCharacteristic;  // BLE characteristic object
    
    // Array to store data for each channel
    uint8_t data_array[MAX_CHANNELS];  // Data array for storing values per channel
    uint8_t err_num;                  // Last error code (no_err, cs_err, data_err)
    uint8_t last_pairs_count;         // Number of valid pairs in last read_data()
};
