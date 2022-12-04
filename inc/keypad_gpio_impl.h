/** @file keypad_gpio_impl.h
 *
 * @brief   platform specific GPIO implementation.
 * @author  huseyink
 * @date    03/12/2022
 * @version v0.0.1
 */

#ifndef KEYPAD_GPIO_IMPL_H_
#define KEYPAD_GPIO_IMPL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void    gpio_write(void* vpGpioPort, uint16_t u16GpioPin, uint8_t u8GpioState);
uint8_t gpio_read (void* vpGpioPort, uint16_t u16GpioPin);
void gpio_setPinOutput(void* vpGpioPort, uint16_t u16GpioPin);
void gpio_setPinInput(void* vpGpioPort, uint16_t u16GpioPin);
void gpio_init(void);
void gpio_deInit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INC_KEYPAD_GPIO_IMPL_H_ */
