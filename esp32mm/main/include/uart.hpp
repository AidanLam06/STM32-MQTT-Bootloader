#ifndef UART_H
#define UART_H

#include "driver/uart.h"

class UART {
    private:
        uart_port_t uart_num;

    public:
        /**
        * @brief Initialize UART port with specified pins and baud rate
        */
        UART(uart_port_t uart_num, int tx_pin, int rx_pin, int baud_rate);

        /**
        * @brief Send a string over UART
        */
        void sendData(const uint8_t *data, size_t len);

        /**
        * @brief Read data from UART and log it
        */
        int receiveData(uint8_t* buffer, size_t size, uint32_t timeout_ms);
};

#endif // UART_H