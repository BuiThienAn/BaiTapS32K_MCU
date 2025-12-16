/*
 * HAL_USART.c
 *
 *  Created on: 16 thg 12, 2025
 *      Author: Admin
 */

#include "HAL_USART.h"
#include "S32K144.h"

#define PIN_TXD 7U
#define PIN_RXD 6U
#define BUFFER_SIZE 50

void HAL_USART_ClockInit(void)
{
    IP_PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_PCS_MASK;
    /*     Chọn SOSCDIV2=8MHz làm src */
    IP_PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(0b001);
    IP_PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
};
void HAL_USART_PinInit(void)
{
    IP_PORTC->PCR[PIN_TXD] |= PORT_PCR_MUX(0b010);
    IP_PORTC->PCR[PIN_RXD] |= PORT_PCR_MUX(0b010);
};
void HAL_USART_LPUART_Init(void)
{
    IP_LPUART1->CTRL &= ~LPUART_CTRL_M_MASK;
    IP_LPUART1->CTRL &= ~LPUART_CTRL_M7_MASK;
    IP_LPUART1->CTRL &= ~LPUART_CTRL_PE_MASK;
    /* LSB */
    IP_LPUART1->STAT &= ~LPUART_STAT_MSBF_MASK;

    IP_LPUART1->BAUD &= ~LPUART_BAUD_SBR_MASK;

    /* Baud rate is approximately 9600 */
    IP_LPUART1->BAUD = LPUART_BAUD_OSR(15) | LPUART_BAUD_SBR(52);

    /* Enable LPUART */
    IP_LPUART1->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
};
void HAL_USART_sendChar(char c)
{
    while (!(IP_LPUART1->STAT & LPUART_STAT_TDRE_MASK));
    IP_LPUART1->DATA = c;
};
void HAL_USART_sendString(char *string)
{
    while (*string != '\0')
    {
        HAL_USART_sendChar(*string);
        string++;
    }
};
char HAL_USART_receiveChar(void)
{
    while(!(IP_LPUART1->STAT & LPUART_STAT_RDRF_MASK));
    return (char)IP_LPUART1->DATA;
};
void HAL_USART_receiveString(char* buffer)
{
    uint8_t count = 0;
    char receieve_character;
    uint8_t finish_flag = 0; // Đổi tên cho dễ hiểu

    while (finish_flag == 0)
    {
        receieve_character = HAL_USART_receiveChar();

        /* Chỉ xử lý nếu KHÔNG PHẢI là ký tự xuống dòng (\n) */
        if (receieve_character != '\n')
        {
            /* 1. Echo lại ký tự để hiện lên màn hình */
            HAL_USART_sendChar(receieve_character);

            /* 2. Kiểm tra kết thúc lệnh (\r) */
            if (receieve_character == '\r')
            {
                HAL_USART_sendChar('\n'); // Gửi thêm \n cho đẹp
                buffer[count] = '\0';     // Kết thúc chuỗi
                finish_flag = 1;          // Thoát vòng lặp lớn
            }
            /* 3. Lưu vào buffer */
            else if (count < BUFFER_SIZE - 1)
            {
                buffer[count] = receieve_character;
                count++;
            }
        }
    }
}