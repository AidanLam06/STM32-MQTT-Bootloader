idf_component_register(SRCS "main.cpp" "uart.cpp"
                    INCLUDE_DIRS "." "include" "../../shared"
                    REQUIRES esp_driver_uart esp_driver_gpio)
