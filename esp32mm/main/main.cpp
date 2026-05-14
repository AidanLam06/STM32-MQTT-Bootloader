#include <cstdio>
#include <cstring>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "hal/gpio_types.h"
#include "shared_protocol.h"
#include "uart.hpp"

static const char *TAG = "OTA_TEST";

// A simple CRC32 implementation to match the STM32 side
uint32_t calculate_crc32(const uint8_t *data, size_t len) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }
    }
    return ~crc;
}
/*
extern "C" void app_main(void) {
    // 1. Initialize our UART Class
    // GPIO 17 (TX), GPIO 16 (RX)
    UART uart(UART_NUM_1, 17, 16, 115200);

    ESP_LOGI(TAG, "Starting UART Protocol Test...");

    // 2. Prepare a Dummy Data Packet
    OTA_Packet_t test_packet;
    memset(&test_packet, 0, sizeof(OTA_Packet_t));

    test_packet.start_byte = 0xAA;
    test_packet.type = PACKET_TYPE_DATA;
    test_packet.block_num = 1;
    test_packet.len = 512;

    // Fill payload with a recognizable pattern (0, 1, 2, 3...)
    for (int i = 0; i < 512; i++) {
        test_packet.payload[i] = (uint8_t)(i % 256);
    }

    // 3. Calculate CRC (everything in the struct EXCEPT the crc field itself)
    test_packet.crc = calculate_crc32((uint8_t*)&test_packet, offsetof(OTA_Packet_t, crc));

    while (true) {
        ESP_LOGI(TAG, "Sending Packet Block %lu (Size: %u bytes)...", 
                 test_packet.block_num, sizeof(OTA_Packet_t));

        // 4. Send the raw bytes of the struct
        uart.sendData((uint8_t*)&test_packet, sizeof(OTA_Packet_t));

        // 5. Wait for Response (ACK = 0x79, NACK = 0x1F)
        uint8_t response = 0;
        int rx_len = uart.receiveData(&response, 1, 2000); // 2 second timeout for Flash write

        if (rx_len > 0) {
            if (response == 0x79) {
                ESP_LOGI(TAG, "SUCCESS: STM32 sent ACK! Flash write verified.");
            } else if (response == 0x1F) {
                ESP_LOGE(TAG, "FAILED: STM32 sent NACK! Checksum or Start Byte mismatch.");
            } else {
                ESP_LOGW(TAG, "UNKNOWN: Received unexpected byte: 0x%02X", response);
            }
        } else {
            ESP_LOGE(TAG, "TIMEOUT: No response from STM32 within 2 seconds.");
        }

        // Wait 5 seconds before the next test loop
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
*/


extern "C" void app_main(void) {
    // 1. Initialize UART (Keep your existing config)
    UART uart(UART_NUM_1, 17, 16, 115200);

    gpio_set_pull_mode((gpio_num_t)16, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode((gpio_num_t)17, GPIO_PULLUP_ONLY);

    ESP_LOGI(TAG, "Starting UART Handshake Test...");

    while (true) {
        ESP_LOGI(TAG, "Sending Sync Byte (0xAA)...");

        // 2. Send ONLY the start byte
        uint8_t sync_byte = 0xAA;
        uart.sendData(&sync_byte, 1);

        // 3. Wait for ACK (0x79) from STM32
        uint8_t response = 0;
        // 5-second timeout gives you plenty of time to hit the STM32 reset button
        int rx_len = uart.receiveData(&response, 1, 5000); 

        if (rx_len > 0) {
            if (response == 0x79) {
                ESP_LOGI(TAG, "SUCCESS: STM32 ack");
                break;
                
                // 4. Stop and stay here so the log doesn't scroll away
                /*
                while(1) {
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
                */
                
            } else {
                ESP_LOGW(TAG, "Received unexpected response: 0x%02X", response);
            }
        } else {
            ESP_LOGE(TAG, "TIMEOUT: No 0x79 received. Retrying in 2 seconds...");
        }

        // Wait before the next attempt
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}