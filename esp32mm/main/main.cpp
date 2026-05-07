#include <cstdio>

#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "UART_LOG";

class UART {
private:
    uart_port_t uart_num;
public:
    // Constructor: Initialize UART port with specified pins and baud rate
    UART(uart_port_t uart_num, int tx_pin, int rx_pin, int baud_rate) : uart_num(uart_num) {
        uart_config_t uart_config = {};
        uart_config.baud_rate = baud_rate;
        uart_config.data_bits = UART_DATA_8_BITS;
        uart_config.parity = UART_PARITY_DISABLE;
        uart_config.stop_bits = UART_STOP_BITS_1;
        uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;

        ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(uart_num, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_ERROR_CHECK(uart_driver_install(uart_num, 1024 * 2, 0, 0, NULL, 0));
    }

    void sendData(const char *data) {
        int len = uart_write_bytes(uart_num, data, strlen(data));
        ESP_LOGI(TAG, "Sent %d bytes: %s", len, data);
    }

    void receiveData() {
        uint8_t data[256];
        int length = uart_read_bytes(uart_num, data, sizeof(data) - 1, 100 / portTICK_PERIOD_MS);
        if (length > 0) {
            data[length] = '\0';
            ESP_LOGI(TAG, "Received: %s", data);
        }
    }
};

extern "C" void app_main(void) {
    // Initialize UART1 on GPIO 17 (TX) and GPIO 16 (RX) at 115200 baud
    UART uart(UART_NUM_1, GPIO_NUM_17, GPIO_NUM_16, 115200);

    while (true) {
        uart.sendData("Hello, UART!\n");
        uart.receiveData();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
