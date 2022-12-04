/** @file 4x4-keypad.c
 * 
 * @brief   4x4 keypad library source file. 
 * @author  huseyink
 * @date    03/12/2022
 * @version v1.0.0
 */

#include "../inc/4x4-keypad.h"

#define KEY_FLAG_ONPRESS_SENT               ((uint16_t)0x0001)

#define KEY_TIME_DEBOUNCE_GET_MIN(btn)      20
#define KEY_TIME_CLICK_GET_PRESSED_MIN(btn) 20
#define KEY_TIME_CLICK_GET_PRESSED_MAX(btn) 300
#define KEY_TIME_CLICK_MAX_MULTI(btn)       400
#define KEY_TIME_KEEPALIVE_PERIOD(btn)      1000
#define KEY_CLICK_MAX_CONSECUTIVE(btn)      3

/**
 * @fn void keypad_init(SKeypad*, SKey*, SKeypadIO*, uint8_t*)
 * @brief
 *
 * @param tKeypad
 * @param tKey
 * @param tKeypadIO
 * @param u8KeyMap
 */
void keypad_init(SKeypad* tKeypad, SKey* tKey, SKeypadIO* tKeypadIO, uint8_t* u8KeyMap)
{
	tKeypad->tKey = tKey;
	tKeypad->tKeypadIO = tKeypadIO;
	tKeypad->u8KeyMap = u8KeyMap;
}

/**
 * @fn void keypad_scan(SKeypad*, uint32_t)
 * @brief
 *
 * @param tKeypad
 * @param mstime
 */
void keypad_scan(SKeypad* tKeypad, uint32_t mstime)
{
	SKey* tKey;
	uint8_t u8NewState = 0;

	for(uint8_t u8Row = 0; u8Row < KEYPAD_ROW_SIZE; ++u8Row)
	{
		uint8_t u8GpioState = (tKeypad->tKeypadIO->ioRow->eGpioContactType == IO_NORMALLY_CLOSE) ? KEYPAD_GPIO_SET
				                                                                                 : KEYPAD_GPIO_RESET;
		tKeypad->tKeypadIO->gpioWrite(tKeypad->tKeypadIO->ioRow[u8Row].vpGpioPort,
						              tKeypad->tKeypadIO->ioRow[u8Row].u16GpioPin,
						              u8GpioState);

		for(uint8_t u8Column = 0; u8Column < KEYPAD_COLUMN_SIZE; ++u8Column)
		{
			uint8_t u8KeyIndex = (u8Row * KEYPAD_ROW_SIZE) + u8Column;
	        tKey = &tKeypad->tKey[u8KeyIndex];

	        /* Get button state */
	        u8NewState = tKeypad->tKeypadIO->gpioRead(tKeypad->tKeypadIO->ioColumn[u8Column].vpGpioPort,
					                                 tKeypad->tKeypadIO->ioColumn[u8Column].u16GpioPin);

	        /*
	         * Button state has changed
	         */
	        if (u8NewState != tKey->tKeyStatus.u8OldState) {
	            /*
	             * Button just became inactive
	             *
	             * - Handle on-release event
	             * - Handle on-click event
	             */
	            if (!u8NewState) {
	                /*
	                 * We only need to react if on-press event has even been started.
	                 *
	                 * Do nothing if that was not the case
	                 */
	                if (tKey->tKeyStatus.u16Flags & KEY_FLAG_ONPRESS_SENT) {
	                    /* Handle on-release event */
	                    tKey->tKeyStatus.u16Flags &= ~KEY_FLAG_ONPRESS_SENT;
	                    tKey->tKeyState = KEY_RELEASED;

	                    /* Check time validity for click event */
	                    if ((mstime - tKey->tKeyStatus.u32TimeChange) >= KEY_TIME_CLICK_GET_PRESSED_MIN(tKey)
	                        && (mstime - tKey->tKeyStatus.u32TimeChange) <= KEY_TIME_CLICK_GET_PRESSED_MAX(tKey)) {

	                        /*
	                         * Increase consecutive clicks if max not reached yet
	                         * and if time between two clicks is not too long
	                         *
	                         * Otherwise we consider click as fresh one
	                         */
	                        if (tKey->tKeyStatus.tClick.u16Counter > 0 && tKey->tKeyStatus.tClick.u16Counter < KEY_CLICK_MAX_CONSECUTIVE(tKey)
	                            && (mstime - tKey->tKeyStatus.tClick.u32LastTime) < KEY_TIME_CLICK_MAX_MULTI(tKey)) {
	                            ++tKey->tKeyStatus.tClick.u16Counter;
	                        } else {
	                            tKey->tKeyStatus.tClick.u16Counter = 1;
	                        }
	                        tKey->tKeyStatus.tClick.u32LastTime = mstime;
	                    } else {
	                        /*
	                         * There was an on-release event, but timing
	                         * for click event detection is outside allowed window.
	                         *
	                         * If user has some consecutive clicks from previous clicks,
	                         * these will be sent after the timeout window (and after the on-release event)
	                         */
	                    }
	                }
	            }

	            /*
	             * Button just pressed
	             */
	            else {
	                /* Do nothing - things are handled after debounce period */
	            }
	            tKey->tKeyStatus.u32TimeChange = mstime;
	            tKey->tKeyStatus.tHold.u32LastTime = mstime;
	            tKey->tKeyStatus.tHold.u16Counter = 0;
	        }

	        /*
	         * Button is still pressed
	         */
	        else if (u8NewState) {
	            /*
	             * Handle debounce and send on-press event
	             *
	             * This is when we detect valid press
	             */
	            if (!(tKey->tKeyStatus.u16Flags & KEY_FLAG_ONPRESS_SENT)) {
	                /* Check minimum stable time */
	                if ((mstime - tKey->tKeyStatus.u32TimeChange) >= KEY_TIME_DEBOUNCE_GET_MIN(tKey)) {
	                    /* Start with new on-press */
	                    tKey->tKeyStatus.u16Flags |= KEY_FLAG_ONPRESS_SENT;
	                    tKey->tKeyState = KEY_PRESSED;

	                    /* Set keep alive time */
	                    tKey->tKeyStatus.tHold.u32LastTime = mstime;
	                }
	            }

	            /*
	             * Handle keep alive, but only if on-press event has been sent
	             *
	             * Keep alive is sent when valid press is being detected
	             */
	            else {
	                if ((mstime - tKey->tKeyStatus.tHold.u32LastTime) >= KEY_TIME_KEEPALIVE_PERIOD(tKey)) {
	                    tKey->tKeyStatus.tHold.u32LastTime += KEY_TIME_KEEPALIVE_PERIOD(tKey);
	                    tKey->tKeyStatus.tHold.u16Counter = 0;
	                    tKey->tKeyState = KEY_HOLD;
	                }
	            }
	        }

	        /*
	         * Button is still released
	         */
	        else {
	            /*
	             * Based on the configuration, this part of the code
	             * will send on-click event after certain timeout.
	             *
	             * This feature is useful if users prefers multi-click feature
	             * that is reported only after last click event happened,
	             * including number of clicks made by user
	             */
	            if (tKey->tKeyStatus.tClick.u16Counter > 0) {
	                if ((mstime - tKey->tKeyStatus.tClick.u32LastTime) >= KEY_TIME_CLICK_MAX_MULTI(tKey)) {
	                	tKey->tKeyState = KEY_CLICKED;
	                    tKey->tKeyStatus.tClick.u16Counter = 0;
	                }
	            }
	        }

	        tKey->tKeyStatus.u8OldState = u8NewState;
		}

		tKeypad->tKeypadIO->gpioWrite(tKeypad->tKeypadIO->ioRow[u8Row].vpGpioPort,
						              tKeypad->tKeypadIO->ioRow[u8Row].u16GpioPin,
						              !u8GpioState);
	}
}

/*** end of file ***/
