#include "HAL_USART.h"
#include "S32K144.h"
#include "QUEUE.h" /* HAL sở hữu Queue */

#define PIN_TXD 7U
#define PIN_RXD 6U
#define NVIC_ISER1 (*((volatile uint32_t *)0xE000E104u))

static SREC_Queue_t s_rx_queue;

/* Buffer tạm thời chỉ dùng nội bộ trong ISR để gom ký tự thành dòng */
static uint8_t s_isr_temp_buffer[BUFFER_SIZE];
static uint8_t s_isr_index = 0U;

/* --- Các hàm Init và Send giữ nguyên như cũ --- */
void HAL_USART_ClockInit(void)
{
    IP_PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_PCS_MASK;
    IP_PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_PCS(0b001);
    IP_PCC->PCCn[PCC_LPUART1_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
}
void HAL_USART_PinInit(void)
{
    IP_PORTC->PCR[PIN_TXD] |= PORT_PCR_MUX(0b010U);
    IP_PORTC->PCR[PIN_RXD] |= PORT_PCR_MUX(0b010U);
}
void HAL_USART_LPUART_Init(void)
{
    IP_LPUART1->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
    IP_LPUART1->CTRL &= ~LPUART_CTRL_M_MASK;
    IP_LPUART1->CTRL &= ~LPUART_CTRL_M7_MASK;
    IP_LPUART1->CTRL &= ~LPUART_CTRL_PE_MASK;
    IP_LPUART1->STAT &= ~LPUART_STAT_MSBF_MASK;
    IP_LPUART1->BAUD = LPUART_BAUD_OSR(15U) | LPUART_BAUD_SBR(52U);
    IP_LPUART1->CTRL |= LPUART_CTRL_RIE_MASK;
    NVIC_ISER1 = (uint32_t)(1u << 1u);
    IP_LPUART1->CTRL |= LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK;
    Queue_Init(&s_rx_queue);
}
void HAL_USART_sendChar(uint8_t c)
{
    while (!(IP_LPUART1->STAT & LPUART_STAT_TDRE_MASK));
    IP_LPUART1->DATA = (uint8_t)c;
}
void HAL_USART_sendString(const uint8_t *string) { /* Giữ nguyên */
    while (*string != (uint8_t)'\0') {
        HAL_USART_sendChar(*string);
        string++;
    }
}

Queue_Status_t HAL_USART_GetLineFromQueue(uint8_t *buffer) {
    /* Tầng trên gọi hàm này để lấy dữ liệu,
       chứ không được đụng trực tiếp vào s_rx_queue */
    return Queue_Pop(&s_rx_queue, buffer);
}

/* --- ISR ĐÃ ĐƯỢC TỐI ƯU --- */
void LPUART1_RxTx_IRQHandler(void)
{
    uint8_t received_char;

    /* Kiểm tra cờ ngắt nhận */
    if (IP_LPUART1->STAT & LPUART_STAT_RDRF_MASK)
    {
        received_char = (uint8_t)IP_LPUART1->DATA;

        /* Bỏ qua ký tự Line Feed (\n) */
        if (received_char != (uint8_t)'\n')
        {
            /* Nếu gặp ký tự kết thúc dòng (\r) */
            if (received_char == (uint8_t)'\r')
            {
                s_isr_temp_buffer[s_isr_index] = (uint8_t)'\0'; /* Kết thúc chuỗi */

                /* Đẩy chuỗi vừa nhận được vào hàng đợi */
                /* Hàm này copy data từ s_isr_temp_buffer vào Queue */
                Queue_Push(&s_rx_queue, s_isr_temp_buffer);

                /* Reset index để nhận dòng mới */
                s_isr_index = 0U;
            }
            else
            {
                /* Lưu vào buffer tạm */
                if (s_isr_index < (uint8_t)BUFFER_SIZE - 1U)
                {
                    s_isr_temp_buffer[s_isr_index] = received_char;
                    s_isr_index++;
                }
            }
        }
    }
}
