/** @file 4x4-keypad.c
 * 
 * @brief   4x4 keypad library header file. 
 * @author  huseyink
 * @date    03/12/2022
 * @version v1.0.0
 */


#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include "key.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define KEYPAD_COLUMN_SIZE (4U)
#define KEYPAD_ROW_SIZE    (4U)

#define KEYPAD_GPIO_SET    (1U)
#define KEYPAD_GPIO_RESET  (0U)

/**
 * @fn uint8_t (*)(void*, uint32_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 * @return
 */
typedef uint8_t (*fpGpioRead) (void* vpGpioPort, uint16_t u16GpioPin);

/**
 * @fn void (*)(void*, uint32_t, uint8_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 * @param u8GpioState
 */
typedef void (*fpGpioWrite)(void* vpGpioPort, uint16_t u16GpioPin, uint8_t u8GpioState);


/**
 * @brief IO contact type enums
 *
 */
typedef enum {
	IO_NORMALLY_CLOSE,
	IO_NORMALLY_OPEN
}EGpioContactType;

/**
 * @brief Key GPIO structure such as
 *        port, pin and contact type.
 */
typedef struct {
	void*            vpGpioPort;
	int16_t          u16GpioPin;
	EGpioContactType eGpioContactType;
}SKeyGPIO;

/**
 * @brief Keypad IO structure such as
 *        column/row ports and pins,
 *        GPIO interface read and write function.
 */
typedef struct {
	SKeyGPIO* ioColumn;
	SKeyGPIO* ioRow;
	fpGpioRead  gpioRead;
	fpGpioWrite gpioWrite;
}SKeypadIO;

/**
 * @brief Keypad main structure
 */
typedef struct {
	SKeypadIO*   tKeypadIO;
	SKey*       tKey;
	uint8_t*    u8KeyMap;
}SKeypad;

void keypad_init(SKeypad* tKeypad, SKey* tKey, SKeypadIO* tKeypadIO, uint8_t* u8KeyMap);
void keypad_scan(SKeypad* tKeypad, uint32_t msTime);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* KEYPAD_H */

/*** end of file ***/