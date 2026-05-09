#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PACKET_TYPE_START,
    PACKET_TYPE_DATA,
    PACKET_TYPE_COMPLETE
} PacketType_t;

typedef struct __attribute__((packed)){
    uint8_t start_byte; // always 0xAA
    uint8_t type;
    uint32_t block_num;
    uint32_t len;
    uint8_t  payload[512];
    uint32_t crc;
} OTA_Packet_t;

typedef struct {
    uint32_t magic_number;  // e.g., 0xDEADBEEF to verify it's a valid update
    uint32_t image_size;    // Total size in bytes
    uint32_t image_crc;     // CRC32 of the application data
    char version[16];       // "v1.0.2"
} AppHeader_t;

#define CMD_WRITE_FLASH 0x01
#define CMD_REBOOT      0x05

#ifdef __cplusplus
}
#endif

#endif
