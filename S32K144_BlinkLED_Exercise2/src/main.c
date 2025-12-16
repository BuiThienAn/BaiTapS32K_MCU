/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-12-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "S32K144.h"
#include "Driver_Common.h"
#include "Driver_GPIO.h"


#define PIN_LED_RED 15
#define PIN_LED_GREEN 16
#define PIN_LED_BLUE 0

extern ARM_DRIVER_GPIO Driver_GPIO0;

void delay_ms(uint32_t ms)
{
uint32_t i = 0u;
uint32_t j = 0u;
for (i = 0u; i < ms; i++)
{
for (j = 0u; j < 5000u; j++)
    {
        /* Do nothing! */
    }
}
};


int main(void)
{
    Driver_GPIO0.Setup(PIN_LED_RED, NULL);
    Driver_GPIO0.Setup(PIN_LED_GREEN, NULL);
    Driver_GPIO0.Setup(PIN_LED_BLUE, NULL);
    Driver_GPIO0.SetDirection(PIN_LED_RED,ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetDirection(PIN_LED_GREEN,ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetDirection(PIN_LED_BLUE,ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(PIN_LED_RED, 1);
    Driver_GPIO0.SetOutput(PIN_LED_GREEN, 1);
    Driver_GPIO0.SetOutput(PIN_LED_BLUE, 1);

    while (1)
	{
	/* Turn on LED RED */
	Driver_GPIO0.SetOutput(PIN_LED_RED, 0);
	Driver_GPIO0.SetOutput(PIN_LED_GREEN, 1);
	Driver_GPIO0.SetOutput(PIN_LED_BLUE, 1);
	delay_ms(3000);

	/* Turn on LED GREEN */
	Driver_GPIO0.SetOutput(PIN_LED_RED, 1);
	Driver_GPIO0.SetOutput(PIN_LED_GREEN, 0);
	delay_ms(3000);

	/* Turn on LED BLUE */
	Driver_GPIO0.SetOutput(PIN_LED_GREEN, 1);
	Driver_GPIO0.SetOutput(PIN_LED_BLUE, 0);
	delay_ms(3000);

	};
return 0;
}
