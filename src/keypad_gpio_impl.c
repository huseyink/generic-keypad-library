/** @file keypad_gpio_impl.c
 *
 * @brief   platform specific GPIO implementation.
 * @author  huseyink
 * @date    03/12/2022
 * @version v0.0.1
 */

/* include platform specific GPIO files */
#include "gpio.h"
#include "keypad_gpio_impl.h"

/**
 * @fn void gpio_write(void*, uint16_t, uint8_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 * @param u8GpioState
 */
void gpio_write(void* vpGpioPort, uint16_t u16GpioPin, uint8_t u8GpioState)
{
	HAL_GPIO_WritePin((GPIO_TypeDef*)vpGpioPort, u16GpioPin, (GPIO_PinState)u8GpioState);
}

/**
 * @fn void gpio_setPinOutput(void*, uint16_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 */
void gpio_setPinOutput(void* vpGpioPort, uint16_t u16GpioPin)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = u16GpioPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init((GPIO_TypeDef*)vpGpioPort, &GPIO_InitStruct);
}

/**
 * @fn void gpio_setPinInput(void*, uint16_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 */
void gpio_setPinInput(void* vpGpioPort, uint16_t u16GpioPin)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = u16GpioPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init((GPIO_TypeDef*)vpGpioPort, &GPIO_InitStruct);
}

/**
 * @fn uint8_t gpio_read(void*, uint16_t_t)
 * @brief
 *
 * @param vpGpioPort
 * @param u16GpioPin
 * @return
 */
uint8_t gpio_read(void* vpGpioPort, uint16_t u16GpioPin)
{
	return (uint8_t)HAL_GPIO_ReadPin((GPIO_TypeDef*)vpGpioPort, u16GpioPin);
}

/**
 * @fn void gpio_init(void)
 * @brief
 *
 */
void gpio_init(void)
{
	/* init handler */
}

/**
 * @fn void gpio_deInit(void)
 * @brief
 *
 */
void gpio_deInit(void)
{
	/* deinit handler */
}





