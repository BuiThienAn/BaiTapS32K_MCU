#include "Queue.h"
#include <stddef.h>

/* Hàm nội bộ để copy chuỗi uint8_t thay thế cho strncpy */
static void Queue_CopyString(uint8_t *dest, const uint8_t *src, uint8_t max_len)
{
    uint8_t i = 0U;

    if ((dest != NULL) && (src != NULL))
    {
        /* Copy cho đến khi gặp ký tự kết thúc hoặc chạm giới hạn max_len - 1 */
        while ((i < (max_len - 1U)) && (src[i] != (uint8_t)'\0'))
        {
            dest[i] = src[i];
            i++;
        }
        /* Đảm bảo chuỗi luôn có ký tự kết thúc null */
        dest[i] = (uint8_t)'\0';
    }
}

void Queue_Init(SREC_Queue_t *q)
{
    if (q != NULL)
    {
        q->head = 0U;
        q->tail = 0U;
        q->count = 0U;
    }
}

Queue_Status_t Queue_IsFull(const SREC_Queue_t *q)
{
    Queue_Status_t full_status = QUEUE_STATE_NOT_FULL;

    if (q != NULL)
    {
        if (q->count == QUEUE_MAX_ELEMENTS)
        {
            full_status = QUEUE_STATE_FULL;
        }
    }

    return full_status;
}

Queue_Status_t Queue_Push(SREC_Queue_t *q, const uint8_t *line)
{
    Queue_Status_t push_status = QUEUE_ERROR;

    /* Kiểm tra con trỏ và kiểm tra hàng đợi đầy */
    if ((q != NULL) && (line != NULL))
    {
        if (q->count < QUEUE_MAX_ELEMENTS)
        {
            Queue_CopyString(q->data[q->tail], line, SREC_LINE_MAX_LEN);
            /* Tính toán tail mới với ép kiểu tường minh */
            q->tail = (uint8_t)((q->tail + 1U) % QUEUE_MAX_ELEMENTS);
            q->count++;
            push_status = QUEUE_OK;
        }
    }

    return push_status;
}

Queue_Status_t Queue_Pop(SREC_Queue_t *q, uint8_t *dest)
{
    Queue_Status_t pop_status = QUEUE_ERROR;

    if ((q != NULL) && (dest != NULL))
    {
        if (q->count > 0U)
        {
            /* Copy từ hàng đợi ra ngoài */
            Queue_CopyString(dest, q->data[q->head], SREC_LINE_MAX_LEN);
            q->head = (uint8_t)((q->head + 1U) % QUEUE_MAX_ELEMENTS);
            q->count--;
            pop_status = QUEUE_OK;
        }
    }
    return pop_status;
}