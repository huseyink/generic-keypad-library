/** @file key.h
 * 
 * @brief
 * @author  huseyinkoc
 * @date    03/12/2022
 * @version v1.0.0
 */

#ifndef KEY_H_
#define KEY_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief key state enums
 *
 */
typedef enum {
	KEY_IDLE,
	KEY_PRESSED,
	KEY_HOLD,
	KEY_RELEASED,
	KEY_CLICKED,
}EKeyState;

/**
 * @brief  Button/input structure
 */
typedef struct {
    uint16_t u16Flags;       /*!< Private button flags management */
    uint8_t  u8OldState;    /*!< Old button state - `1` means active, `0` means inactive */
    uint32_t u32TimeChange; /*!< Time in ms when button state got changed last time */

    struct {
        uint32_t u32LastTime; /*!< Time in ms of last send keep alive event */
        uint16_t u16Counter;       /*!< Number of keep alive events sent after successful on-press detection.
                                    Value is reset after on-release */
    }tHold;            /*!< Keep alive structure */

    struct {
        uint32_t u32LastTime; /*!< Time in ms of last successfully detected (not sent!) click event */
        uint8_t  u16Counter;        /*!< Number of consecutive clicks detected, respecting maximum timeout between clicks */
    }tClick;                 /*!< Click event structure */
}SKeyStatus;

/**
 * @brief Key structure
 *
 */
typedef struct {
	EKeyState    tKeyState;
	SKeyStatus	 tKeyStatus;
	uint8_t      u8KeyChar;
}SKey;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INC_KEY_H_ */
