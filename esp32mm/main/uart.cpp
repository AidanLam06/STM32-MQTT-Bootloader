#include <cstring>
#include "esp_log.h"
#include "uart.hpp"

static const char *TAG = "UART_LOG";

// Constructor implementation
// Note: We use the ClassName::MethodName syntax here
UART::UART(uart_port_t uart_num, int tx_pin, int rx_pin, int baud_rate) : uart_num(uart_num) {
    uart_config_t uart_config = {};
    uart_config.baud_rate = baud_rate;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT; // Good practice to define the clock source

    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    // Install driver with a 2KB RX buffer
    ESP_ERROR_CHECK(uart_driver_install(uart_num, 1024 * 2, 0, 0, NULL, 0));
}

// Send data implementation
void UART::sendData(const uint8_t *data, size_t len) {
    if (data == nullptr || len == 0) return;

    int sent = uart_write_bytes(uart_num, data, len);
    
    if (len > 0) {
        ESP_LOGI(TAG, "Sent %d bytes", sent);
        ESP_LOG_BUFFER_HEX(TAG, data, sent); // This is vital for protocol debugging
    }
}

// Receive data implementation
int UART::receiveData(uint8_t* buffer, size_t size, uint32_t timeout_ms) {
    // We use the provided buffer instead of a local one
    int length = uart_read_bytes(uart_num, buffer, size, pdMS_TO_TICKS(timeout_ms));
    
    if (length > 0) {
        // Optional: still log it if you want to see the raw hex
        ESP_LOG_BUFFER_HEX(TAG, buffer, length); 
    }
    
    return length; // Return how many bytes we actually got
}