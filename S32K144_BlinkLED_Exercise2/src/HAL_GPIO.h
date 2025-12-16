/*
 * HAL_GPIO.h
 *
 *  Created on: 16 thg 12, 2025
 *      Author: Admin
 */
#include <stdint.h>

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

	void HAL_GPIO_ClockInit(void);
	void HAL_GPIO_PinInit(uint32_t pinNumber);
	void HAL_GPIO_PinDataDirection(uint32_t pinNumber, uint8_t value);
	void HAL_GPIO_WritePin(uint32_t pinNumber, uint8_t value);

#endif /* HAL_GPIO_H_ */
