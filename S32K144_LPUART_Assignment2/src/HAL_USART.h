/*
 * HAL_USART.h
 *
 *  Created on: 16 thg 12, 2025
 *      Author: Admin
 */
#ifndef HAL_USART_H_
#define HAL_USART_H_
    #include <stdint.h>
    void HAL_USART_ClockInit(void);
    void HAL_USART_PinInit(void);
    void HAL_USART_LPUART_Init(void);
    void HAL_USART_sendChar(char c);
    void HAL_USART_sendString(char *string);
    char HAL_USART_receiveChar(void);
    void HAL_USART_receiveString(char* buffer);

#endif /* HAL_USART_H_ */
