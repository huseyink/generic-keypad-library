/** @file keypad.h
 * 
 * @brief   generic keypad library header file.
 * @author  huseyink
 * @date    03/12/2022
 * @version v1.0.0
 */


#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

#include "../../keypad/inc/key.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef NULL
#define NULL (void*)0
#endif

#define KEYPAD_GPIO_SET    (1U)
#define KEYPAD_GPIO_RESET  (0U)


/* Forward declarations */
struct _SKeypad;

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
 * @fn void (*)(void*, uint16_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 */
typedef void (*fpGpioInput)(void* vpGpioPort, uint16_t u16GpioPin);

/**
 * @fn void (*)(void*, uint16_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 */
typedef void (*fpGpioOutput)(void* vpGpioPort, uint16_t u16GpioPin);


/**
 * @fn void (*)(SKeypad*)
 * @brief
 *
 * @param tKeypad
 */
typedef void (*fpEventCallback)(struct _SKeypad* tKeyPad, EKeyState eKeyState, uint8_t u8KeyChar);

/**
 * @brief IO contact type enums
 *
 */
typedef enum {
	IO_NORMALLY_CLOSE, /* pull-up connection */
	IO_NORMALLY_OPEN   /* pull-down connection */
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
	SKeyGPIO*    ioColumn;
	SKeyGPIO*    ioRow;
	fpGpioRead   gpioRead;
	fpGpioWrite  gpioWrite;
	fpGpioInput  gpioInput;
	fpGpioOutput gpioOutput;
}SKeypadIO;

/**
 * @brief Keypad main structure
 */
typedef struct _SKeypad{
	uint8_t         u8RowCount;
	uint8_t         u8ColumnCount;
	SKeypadIO*      tKeypadIO;
	SKey*           tKey;
	uint8_t*        u8KeyMap;
	fpEventCallback fpEventCallback;
}SKeypad;

uint8_t keypad_init(SKeypad* tKeypad, uint8_t u8RowCount, uint8_t u8ColumnCount);
uint8_t keypad_scan(SKeypad* tKeypad, uint32_t msTime);
void keypad_addEventCallback(SKeypad* tKeypad, fpEventCallback keyEventCallback);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* KEYPAD_H */

/*** end of file ***/
