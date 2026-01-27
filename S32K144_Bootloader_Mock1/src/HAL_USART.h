#ifndef HAL_USART_H_
#define HAL_USART_H_

#include <stdint.h>
#include <stdbool.h>
#include "QUEUE.h"

#define BUFFER_SIZE 100


void HAL_USART_ClockInit(void);
void HAL_USART_PinInit(void);
void HAL_USART_LPUART_Init(void);
void HAL_USART_sendChar(uint8_t c);
void HAL_USART_sendString(const uint8_t *string);
/* Hàm mới để lấy dữ liệu an toàn */
Queue_Status_t HAL_USART_GetLineFromQueue(uint8_t *buffer);

#endif /* HAL_USART_H_ */
