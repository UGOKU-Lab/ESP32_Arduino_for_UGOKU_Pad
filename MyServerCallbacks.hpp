#pragma once  // Ensures the header file is included only once during compilation

#include <BLEServer.h>  // Include BLE server functionality

// Forward declaration of UGOKU_Pad_Controller class to avoid circular dependency
class UGOKU_Pad_Controller;

// MyServerCallbacks class inherits from BLEServerCallbacks to handle BLE connection events
class MyServerCallbacks : public BLEServerCallbacks {
  UGOKU_Pad_Controller* controller;  // Pointer to the UGOKU_Pad_Controller instance

  public:
    // Constructor that accepts a UGOKU_Pad_Controller pointer
    MyServerCallbacks(UGOKU_Pad_Controller* ctrl);

    // Overridden method that handles BLE connection events
    void onConnect(BLEServer* pServer) override;

    // Overridden method that handles BLE disconnection events
    void onDisconnect(BLEServer* pServer) override;
};
