#include "S32K144.h"
#include "Driver_GPIO.h"
#include "Driver_Common.h"

extern ARM_DRIVER_GPIO Driver_GPIO0;
extern ARM_DRIVER_GPIO Driver_GPIO1;

#define PIN_LED_RED 15
#define PIN_LED_GREEN 16

#define BUTTON_0 12
#define BUTTON_1 13

void delay_ms(uint32_t ms)
{
    uint32_t i = 0;
    uint32_t j = 0;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 5000; j++)
        {
            /* Do nothing */
        };
    }
}

int main(void)
{
    Driver_GPIO0.Setup(PIN_LED_RED,NULL);
    Driver_GPIO0.Setup(PIN_LED_GREEN,NULL);
    Driver_GPIO1.Setup(BUTTON_0, NULL);
    Driver_GPIO1.Setup(BUTTON_1, NULL);

    Driver_GPIO0.SetDirection(PIN_LED_RED, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetDirection(PIN_LED_GREEN, ARM_GPIO_OUTPUT);
    Driver_GPIO1.SetDirection(BUTTON_0, ARM_GPIO_INPUT);
    Driver_GPIO1.SetDirection(BUTTON_1, ARM_GPIO_INPUT);

    Driver_GPIO0.SetOutput(PIN_LED_RED, 1);
    Driver_GPIO0.SetOutput(PIN_LED_GREEN, 1);

    while (1)
    {
        if (Driver_GPIO1.GetInput(BUTTON_0) == 0)
        {
            delay_ms(20);
            if (Driver_GPIO1.GetInput(BUTTON_0) == 0)
            {
                uint32_t current_state = Driver_GPIO0.GetInput(PIN_LED_RED);
                Driver_GPIO0.SetOutput(PIN_LED_RED, !current_state);
                while (Driver_GPIO1.GetInput(BUTTON_0) == 0);
            }
        }

        if (Driver_GPIO1.GetInput(BUTTON_1) == 0)
        {
            delay_ms(20);
            if (Driver_GPIO1.GetInput(BUTTON_1) == 0)
            {
                uint32_t current_state = Driver_GPIO0.GetInput(PIN_LED_GREEN);
                Driver_GPIO0.SetOutput(PIN_LED_GREEN, !current_state);
                while (Driver_GPIO1.GetInput(BUTTON_1) == 0);
            }
        }
    }
    return 0;
}
