# STM32-MQTT-Bootloader
## Description:
Custom STM32 bootloader to simulated OTA updates from an MQTT server, with an ESP32S3 acting as a receiver. The data fetched from the MQTT server by the ESP32S3 is passed to the STM32 via UART. The STM32 utilizes the dual-bank mode on the STM32L4 series boards to reliably swap between an Update partition and an App partition. The flashing sequence follows a typical stop-and-wait ARQ protocol.

## Status:
In progress
