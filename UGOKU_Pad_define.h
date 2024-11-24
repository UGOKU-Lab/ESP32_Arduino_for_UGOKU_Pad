#pragma once  // Ensures the header file is included only once during compilation

// UUIDs for the BLE service and characteristic
// You can generate new UUIDs using: https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"  // UUID for the BLE service
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"  // UUID for the BLE characteristic

// Error codes
#define no_err 0xE0   // No error
#define cs_err 0xE1   // Checksum error

// Define the maximum number of channels
#define MAX_CHANNELS 255  // Maximum number of channels supported
