/*
 * HAL_GPIO.h
 *
 *  Created on: 16 thg 12, 2025
 *      Author: Admin
 */
#include <stdint.h>

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_
	/* PORTD */
	void HAL_GPIO0_ClockInit(void);
	void HAL_GPIO0_PinInit(uint32_t pinNumber);
	void HAL_GPIO0_PinDataDirection(uint32_t pinNumber, uint8_t value);
	void HAL_GPIO0_WritePin(uint32_t pinNumber, uint8_t value);
	uint8_t HAL_GPIO0_ReadPin(uint32_t pinNumber);
#endif /* HAL_GPIO_H_ */
