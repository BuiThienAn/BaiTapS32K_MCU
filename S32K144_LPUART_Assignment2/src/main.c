/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-12-12
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "S32K144.h"
#include "Driver_GPIO.h"
#include "Driver_Common.h"
#include "Driver_USART.h"

#define RED_LED_PIN 15
#define GREEN_LED_PIN 16
#define BLUE_LED_PIN 0
#define BUFFER_SIZE 50

extern ARM_DRIVER_GPIO Driver_GPIO0;
extern ARM_DRIVER_USART Driver_USART0;

int strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1==*str2))
    {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

void SOSC_init_8MHz(void)
{
    /* SOSCDIV1=1 và SOSCDIV2=1 */
    IP_SCG->SOSCDIV = 0x00000101;
    /* Cấp điện áp cho thạch anh 1-8MHz */
    /* xtal osc low power */
    /* EREF=1 External SOSC */
    IP_SCG->SOSCCFG = SCG_SOSCCFG_RANGE(0b10)
                    | SCG_SOSCCFG_HGO(0b0)
                    | SCG_SOSCCFG_EREFS(0b1);
    while (IP_SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK)
    {
        /* Do nothing */
    }
    IP_SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN(0b1);
    while (!(IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK))
    {
        /* Do nothing */
    };
};

void SPLL_init_160MHz()
{
    /* SPLLDIV2=4 SPLLDIV1=2 */
    IP_SCG->SPLLDIV = 0x00000302;

    /* MULT=24 */
    /* PREDIV=0 ->8/(0+1)*(24+16)/2=160MHz */
    IP_SCG->SPLLCFG = SCG_SPLLCFG_MULT(0b11000)
                     |SCG_SPLLCFG_PREDIV(0b0);
    while (IP_SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK){};

    /* Enable System PLL */
    IP_SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN(0b1);
    while (!(IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)){};
};

void NormalRunMode_Init(){
    /* CORE 160/2=80MHz */
    /* BUS 160/4=40MHz */
    /* FLASH 160/6=26.67 MHz */
    IP_SCG->RCCR = SCG_RCCR_SCS(0b110)
                  |SCG_RCCR_DIVCORE(0b01)
                  |SCG_RCCR_DIVBUS(0b0011)
                  |SCG_RCCR_DIVSLOW(0b0101);
    while (((IP_SCG->CSR & SCG_CSR_SCS_MASK)>>SCG_CSR_SCS_SHIFT) != 6){};
}

typedef enum{
    CMD_UNKNOWN = 0,
    CMD_LED_STATUS,
    CMD_RED_ON,
    CMD_RED_OFF,
    CMD_GREEN_ON,
    CMD_GREEN_OFF,
    CMD_BLUE_ON,
    CMD_BLUE_OFF,
    CMD_HELP
} CommandID;

typedef struct {
    char* cmd_str;
    CommandID cmd_id;
} CommandMap;

const CommandMap command_list[] = {
    {"LED STATUS", CMD_LED_STATUS},
    {"RED ON",     CMD_RED_ON},
    {"RED OFF",    CMD_RED_OFF},
    {"GREEN ON",   CMD_GREEN_ON},
    {"GREEN OFF",  CMD_GREEN_OFF},
    {"BLUE ON",    CMD_BLUE_ON},
    {"BLUE OFF",   CMD_BLUE_OFF},
    {"HELP",       CMD_HELP}
};

CommandID getCommandID(char* str)
{
    uint8_t CommandMap_size = sizeof(command_list) / sizeof(command_list[0]);
    uint8_t i = 0;
    uint8_t flag = 0;
    CommandID result;
    while ((i< CommandMap_size) && (flag == 0))
    {
        if (strcmp(str,command_list[i].cmd_str)==0)
        {
            result = command_list[i].cmd_id;
            flag = 1;
        }
        i++;
    }
    if (flag == 0)
    {
        result = CMD_UNKNOWN;
    }
    return result;
}


void execute_command(char* str)
{
    uint8_t id = getCommandID(str);
    switch (id)
    {
        case CMD_LED_STATUS:
            if (Driver_GPIO0.GetInput(RED_LED_PIN) == 1)
            {
                Driver_USART0.Send("RED LED IS OFF",NULL);
            }
            else
            {
                Driver_USART0.Send("RED LED IS ON",NULL);
            };
            Driver_USART0.Send("\n",NULL);
            if (Driver_GPIO0.GetInput(GREEN_LED_PIN) == 1)
            {
                Driver_USART0.Send("GREEN LED IS OFF",NULL);
            }
            else
            {
                Driver_USART0.Send("GREEN LED IS ON",NULL);
            };
            Driver_USART0.Send("\n",NULL);
            if (Driver_GPIO0.GetInput(BLUE_LED_PIN) == 1)
            {
                Driver_USART0.Send("BLUE LED IS OFF",NULL);
            }
            else
            {
                Driver_USART0.Send("BLUE LED IS ON",NULL);
            };
            Driver_USART0.Send("\n",NULL);
            break;
        case CMD_RED_ON:
            Driver_GPIO0.SetOutput(RED_LED_PIN,0);
            break;
        case CMD_RED_OFF:
            Driver_GPIO0.SetOutput(RED_LED_PIN,1);
            break;
        case CMD_GREEN_ON:
            Driver_GPIO0.SetOutput(GREEN_LED_PIN,0);
            break;
        case CMD_GREEN_OFF:
            Driver_GPIO0.SetOutput(GREEN_LED_PIN,1);
            break;
        case CMD_BLUE_ON:
            Driver_GPIO0.SetOutput(BLUE_LED_PIN,0);
            break;
        case CMD_BLUE_OFF:
            Driver_GPIO0.SetOutput(BLUE_LED_PIN,1);
            break;
        case CMD_HELP:
            Driver_USART0.Send("Type in one of these keywords: \n",NULL);
            Driver_USART0.Send("1. Type LED STATUS to receive led status\n",NULL);
            Driver_USART0.Send("2. Type RED ON to turn on RED LED\n",NULL);
            Driver_USART0.Send("3. Type RED OFF to turn off RED LED\n",NULL);
            Driver_USART0.Send("4. Type GREEN ON to turn on RED LED\n",NULL);
            Driver_USART0.Send("5. Type GREEN OFF to turn off RED LED\n",NULL);
            Driver_USART0.Send("6. Type BLUE ON to turn on RED LED\n",NULL);
            Driver_USART0.Send("7. Type BLUE OFF to turn off RED LED\n",NULL);
            Driver_USART0.Send("8. Type HELP to see the guidelines again\n",NULL);
            break;
        default:
            Driver_USART0.Send("Command not available\n",NULL);
            break;
    }
}

int main(){
    SOSC_init_8MHz();
    SPLL_init_160MHz();
    NormalRunMode_Init();
    Driver_GPIO0.Setup(RED_LED_PIN, NULL);
    Driver_GPIO0.Setup(GREEN_LED_PIN, NULL);
    Driver_GPIO0.Setup(BLUE_LED_PIN, NULL);
    Driver_GPIO0.SetDirection(RED_LED_PIN,ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetDirection(GREEN_LED_PIN,ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetDirection(BLUE_LED_PIN,ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(RED_LED_PIN, 1);
    Driver_GPIO0.SetOutput(GREEN_LED_PIN, 1);
    Driver_GPIO0.SetOutput(BLUE_LED_PIN, 1);
    Driver_USART0.Initialize(NULL);
    char string_receive[BUFFER_SIZE];
    Driver_USART0.Send("If you're new, type HELP to see commands: \n",NULL);
    while (1)
    {
        Driver_USART0.Receive(string_receive,NULL);
        Driver_USART0.Send(" -> Board sent back: ",NULL);
        Driver_USART0.Send(string_receive, NULL);
        Driver_USART0.Send("\n",NULL);

        execute_command(string_receive);
        Driver_USART0.Send("\n",NULL);
    };
    return 0;
}
