#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

#define QUEUE_MAX_ELEMENTS 4U
/* Độ dài an toàn cho một dòng SREC */
#define SREC_LINE_MAX_LEN  100U

typedef struct {
    uint8_t data[QUEUE_MAX_ELEMENTS][SREC_LINE_MAX_LEN];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} SREC_Queue_t;

/* Định nghĩa Enum thay thế cho bool */
typedef enum
{
    QUEUE_OK = 0U,          /* Thay cho true (thành công) */
    QUEUE_ERROR = 1U,       /* Thay cho false (thất bại) */
    QUEUE_STATE_FULL = 2U,  /* Trạng thái đầy */
    QUEUE_STATE_NOT_FULL = 3U /* Trạng thái chưa đầy */
} Queue_Status_t;

void Queue_Init(SREC_Queue_t *q);
Queue_Status_t Queue_Push(SREC_Queue_t *q, const uint8_t *line);
Queue_Status_t Queue_Pop(SREC_Queue_t *q, uint8_t *dest);
Queue_Status_t Queue_IsFull(const SREC_Queue_t *q);

#endif