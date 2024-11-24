#include "MyServerCallbacks.hpp"
#include "UGOKU_Pad_Controller.hpp"

// Constructor for MyServerCallbacks class, links the UGOKU_Pad_Controller instance
MyServerCallbacks::MyServerCallbacks(UGOKU_Pad_Controller* ctrl) {
  controller = ctrl;  // Store the reference to the controller object
}

// Callback function that gets called when a BLE device connects
void MyServerCallbacks::onConnect(BLEServer* pServer) {
  if (controller->onConnectCallback) {
    controller->onConnectCallback();  // Trigger the onConnect callback if it's been set
  }
}

// Callback function that gets called when a BLE device disconnects
void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
  if (controller->onDisconnectCallback) {
    controller->onDisconnectCallback();  // Trigger the onDisconnect callback if it's been set
  }
  
  // Restart BLE advertising so that other devices can connect
  BLEDevice::startAdvertising();
}
