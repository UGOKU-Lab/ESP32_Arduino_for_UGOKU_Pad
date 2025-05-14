#include "UGOKU_Pad_Controller.hpp"   // Include the custom UGOKU Pad Controller header

// Constructor for UGOKU_Pad_Controller
UGOKU_Pad_Controller::UGOKU_Pad_Controller(void) {
  ch = 0;            // Initialize channel variable
  val = 0;           // Initialize value variable
  cs = 0;            // Initialize checksum variable
  err_num = 0;       // Initialize error number
  return;            // Return from the constructor
}

// Setup BLE device with a given name
void UGOKU_Pad_Controller::setup(char *device_name) {
  BLEDevice::init(device_name);   // Initialize BLE with the device name
  pServer = BLEDevice::createServer();   // Create a BLE server
  pServer->setCallbacks(new MyServerCallbacks(this));  // Set callback functions for the server

  // Create a BLE service and characteristic
  pService = pServer->createService(SERVICE_UUID);  // Create a service with a defined UUID
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,  // Characteristic UUID
    BLECharacteristic::PROPERTY_READ |  // Allow read
    BLECharacteristic::PROPERTY_WRITE |   // Allow write
    BLECharacteristic::PROPERTY_WRITE_NR // Allow writing without response (for faster performance)
  );

  pService->start();  // Start the BLE service
  
  // Start BLE advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();  // Get advertising object
  pAdvertising->addServiceUUID(SERVICE_UUID);  // Add the service UUID to the advertising packet
  pAdvertising->setScanResponse(true);         // Enable scan response
  pAdvertising->setMinPreferred(0x06);         // Set minimum preferred connection interval
  pAdvertising->setMinPreferred(0x12);         // Set maximum preferred connection interval
  BLEDevice::startAdvertising();               // Start BLE advertising

  return;
}

// Set the callback function for when a device connects
void UGOKU_Pad_Controller::setOnConnectCallback(void (*callback)()) {
  onConnectCallback = callback;  // Assign the callback function for connection events
}

// Set the callback function for when a device disconnects
void UGOKU_Pad_Controller::setOnDisconnectCallback(void (*callback)()) {
  onDisconnectCallback = callback;  // Assign the callback function for disconnection events
}

// Read data from the BLE characteristic
uint8_t UGOKU_Pad_Controller::read_data(void) {
  uint8_t err_num = 0;  // Initialize the error number

  // Read the value from the characteristic (received from the BLE client)
  //String value = pCharacteristic->getValue();

  //aoki Add ↓
  std::string value = pCharacteristic->getValue();

  if (value.length() == 3) {
    ch = static_cast<uint8_t>(value[0]);
    val = static_cast<uint8_t>(value[1]);
    cs = static_cast<uint8_t>(value[2]);

    if ((ch ^ val) == cs) {
      if (ch < MAX_CHANNELS) {
        data_array[ch] = val;
      }
      err_num = no_err;
    } else {
      ch = val = cs = 0xFF;
      err_num = cs_err;
    }
  }
  //aoki add ↑

  if (value.length() == 3) {  // Check if the value has exactly 3 bytes
    // Parse the received data
    ch = value[0];  // First byte represents the channel ID
    val = value[1];  // Second byte represents the value
    cs = value[2];   // Third byte is the checksum

    // Verify the checksum (XOR of channel and value should match checksum)
    if ((ch ^ val) == cs) {
      // If the channel is valid, store the value in the array
      if (ch < MAX_CHANNELS) {
        data_array[ch] = val;  // Store the value in the data array at the channel index
      }

      err_num = no_err;  // No error encountered

    } else {
      // If checksum verification fails, reset the values and set the error
      ch = 0xFF;
      val = 0xFF;
      cs = 0xFF;

      err_num = cs_err;  // Set error code for checksum failure
    }
  }

  return err_num;  // Return the error number (0 if successful)
}

// Write data to the BLE characteristic
void UGOKU_Pad_Controller::write_data(uint8_t w_ch, uint8_t w_val) {
  uint8_t w_cs = w_ch ^ w_val;  // Calculate checksum (XOR of channel and value)
  
  // Create an array to hold the data to write
  uint8_t data[3];
  data[0] = w_ch;   // First byte: channel ID
  data[1] = w_val;  // Second byte: value
  data[2] = w_cs;   // Third byte: checksum

  // Set the characteristic value and notify BLE clients
  pCharacteristic->setValue(data, sizeof(data));  // Write data to the characteristic
  pCharacteristic->notify();  // Notify clients if notifications are enabled

  return;
}

// Get the current channel value
uint8_t UGOKU_Pad_Controller::get_ch(void) {
  return ch;  // Return the current channel ID
}

// Get the current value associated with the channel
uint8_t UGOKU_Pad_Controller::get_val(void) {
  return val;  // Return the current value
}

// Get the data stored in the array by channel
uint8_t UGOKU_Pad_Controller::getDataByChannel(uint8_t channel) {
  if (channel < MAX_CHANNELS) {
    return data_array[channel];  // Return the stored value for the specified channel
  } else {
    return 0xFF;  // Return 0xFF if the channel is out of range
  }
}
