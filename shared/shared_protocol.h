#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_WRITE_FLASH 0x01
#define CMD_REBOOT      0x05
#define CHUNK_SIZE      512
#define PACKET_SIZE     524
#define ACK_BYTE        0x79
#define NACK_BYTE       0x15

typedef enum {
    PACKET_TYPE_START,
    PACKET_TYPE_DATA,
    PACKET_TYPE_COMPLETE
} PacketType_t;

typedef struct __attribute__((packed)){
    uint8_t start_byte; // always 0xAA
    uint8_t type;
    uint8_t reserved[2] // padding for HAL_CRC_Caluclate which operates in 4 byte chunks
    uint32_t block_num;
    uint32_t len;
    uint8_t  payload[CHUNK_SIZE];
    uint32_t crc;
} OTA_Packet_t; // 522 bytes total

typedef struct {
    uint32_t magic_number;
    uint32_t image_size;
    uint32_t image_crc;
    char version[16];
} AppHeader_t;

#ifdef __cplusplus
}
#endif

#endif
