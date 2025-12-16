/*
 * HAL_GPIO.c
 *
 *  Created on: 16 thg 12, 2025
 *      Author: Admin
 */

#include "HAL_GPIO.h"
#include <stdint.h>
#include <S32K144.h>

/* =========================================================================*/
/* INSTANCE 0 (GPIO0): Dành cho PORT D */
/* =========================================================================*/

void HAL_GPIO0_ClockInit(void)
{
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
};
void HAL_GPIO0_PinInit(uint32_t pinNumber)
{
    IP_PORTD->PCR[pinNumber] &= ~PORT_PCR_MUX(0b111);
    IP_PORTD->PCR[pinNumber] |= PORT_PCR_MUX(1);
};
void HAL_GPIO0_PinDataDirection(uint32_t pinNumber, uint8_t value)
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
void HAL_GPIO0_WritePin(uint32_t pinNumber, uint8_t value)
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
uint8_t HAL_GPIO0_ReadPin(uint32_t pinNumber)
{
    return (IP_PTD->PDIR & (1<<pinNumber)) ? 1 : 0;
};
