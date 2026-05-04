#ifndef INC_FLASH_LAYOUT_H_
#define INC_FLASH_LAYOUT_H_

#define BL_START_ADDR   (0x08000000) // start of flash memory, 32KB for bootloader

#define APP_HEADER_ADDR (0x08008000) // 256B app header
#define APP_START_ADDR  (0x08008100) // 199.75KB for app

#define METADATA        (0x0806C200) // 32KB for metadata

#endif /* INC_FLASH_LAYOUT_H_ */
