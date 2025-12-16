/*
 * HAL_GPIO.c
 *
 *  Created on: 16 thg 12, 2025
 *      Author: Admin
 */

#include "HAL_GPIO.h"
#include <stdint.h>
#include <S32K144.h>


void HAL_GPIO_ClockInit(void)
{
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
};
void HAL_GPIO_PinInit(uint32_t pinNumber)
{
    IP_PORTD->PCR[pinNumber] &= ~PORT_PCR_MUX(0b111);
    IP_PORTD->PCR[pinNumber] |= PORT_PCR_MUX(1);
};
void HAL_GPIO_PinDataDirection(uint32_t pinNumber, uint8_t value)
{
    if (value == 1)
    {
        IP_PTD->PDDR |= (1 << pinNumber);
    }
    else
    {
        IP_PTD->PDDR &= ~(1 << pinNumber);
    }
};
void HAL_GPIO_WritePin(uint32_t pinNumber, uint8_t value)
{
    if (value == 1)
    {
        IP_PTD->PSOR |= (1 << pinNumber);
    }
    else
    {
        IP_PTD->PCOR |= (1 << pinNumber);
    }
};
