#include "UGOKU_Pad_Controller.hpp"   // Include the custom UGOKU Pad Controller header

// Constructor for UGOKU_Pad_Controller
UGOKU_Pad_Controller::UGOKU_Pad_Controller(void) {
  for (int i = 0; i < MAX_CHANNELS; i++) {
    data_array[i] = 0xFF;
  }
  err_num = no_err;
  last_pairs_count = 0;
}

// Setup BLE device with a given name
void UGOKU_Pad_Controller::setup(char *device_name) {
  BLEDevice::init(device_name);   // Initialize BLE with the device name
  pServer = BLEDevice::createServer();   // Create a BLE server
  pServer->setCallbacks(new MyServerCallbacks(this));  // Set callback functions for the server

  // Create a BLE service and characteristic
  pService = pServer->createService(SERVICE_UUID);  // Create a service with a defined UUID
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,                    // Characteristic UUID
                                         BLECharacteristic::PROPERTY_READ |      // Allow read
                                         BLECharacteristic::PROPERTY_WRITE |     // Allow write
                                         BLECharacteristic::PROPERTY_WRITE_NR |  // Allow writing without response (for faster performance)
                                         BLECharacteristic::PROPERTY_NOTIFY      // Allow notify
                                         );

  pCharacteristic->addDescriptor(new BLE2902()); // For IOS

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

// Read exactly 19 bytes from the characteristic.
// Packet layout: [ch0,val0, ch1,val1, ... ch8,val8] (18 bytes) + [checksum] (1 byte)
uint8_t UGOKU_Pad_Controller::read_data(void) {
  String raw = pCharacteristic->getValue();  // Expecting 19 bytes
  size_t len = raw.length();

  if (len != 19) {
    // Packet length is wrong
    err_num = data_err;
    last_pairs_count = 0;
    return err_num;
  }

  uint8_t packet[19];
  for (size_t i = 0; i < 19; i++) {
    packet[i] = (uint8_t)raw[i];
  }

  // Compute XOR checksum over the first 18 bytes
  uint8_t computed_cs = 0;
  for (int i = 0; i < 18; i++) {
    computed_cs ^= packet[i];
  }

  uint8_t received_cs = packet[18];
  if (computed_cs != received_cs) {
    // Checksum mismatch
    err_num = cs_err;
    last_pairs_count = 0;
    return err_num;
  }

  // Valid packet: parse up to 9 (channel,value) pairs
  last_pairs_count = 0;
  for (int i = 0; i < 9; i++) {
    uint8_t ch  = packet[2 * i];     // indices: 0,2,4,...,16
    uint8_t val = packet[2 * i + 1]; // indices: 1,3,5,...,17

    if (ch < MAX_CHANNELS) {
      data_array[ch] = val;
      last_pairs_count++;
    }
    // If ch >= MAX_CHANNELS, ignore that pair
  }

  err_num = no_err;
  return err_num;
}

// Write exactly 19 bytes: 9 pairs (channels[0..8], values[0..8]) + XOR checksum
void UGOKU_Pad_Controller::write_data(const uint8_t channels[9], const uint8_t values[9]) {
  uint8_t packet[19];

  // Copy channel/value pairs into packet[0..17]
  for (int i = 0; i < 9; i++) {
    packet[2 * i]     = channels[i];
    packet[2 * i + 1] = values[i];
  }

  // Compute XOR checksum on the first 18 bytes
  uint8_t cs = 0;
  for (int i = 0; i < 18; i++) {
    cs ^= packet[i];
  }
  packet[18] = cs;

  // Store into characteristic. No notify—central must read explicitly if needed.
  pCharacteristic->setValue(packet, 19);
  pCharacteristic->notify(); 
}

// except index 0, which gets (channel,value). Then call the full‐packet write_data(...).
void UGOKU_Pad_Controller::write_data(uint8_t channel, uint8_t value) {
  // Create local buffers
  uint8_t chArr[9];
  uint8_t valArr[9];

  // Fill all slots with “no data” (0xFF for channel, 0 for value)
  for (int i = 0; i < 9; i++) {
    chArr[i]  = 0xFF;
    valArr[i] = 0;
  }

  // Put your real channel/value in slot #0
  chArr[0]  = channel;
  valArr[0] = value;

  // Call the main write_data that sends all 9 pairs + checksum
  write_data(chArr, valArr);
}

// Return the most recently stored value for a given channel index
uint8_t UGOKU_Pad_Controller::getDataByChannel(uint8_t channel) {
  if (channel < MAX_CHANNELS) {
    return data_array[channel]; // 0xFF means “no data”
  }
  return 0xFF;
}

// Return how many valid (channel,value) pairs were parsed last time read_data() was called
uint8_t UGOKU_Pad_Controller::getLastPairsCount(void) {
  return last_pairs_count;
}
