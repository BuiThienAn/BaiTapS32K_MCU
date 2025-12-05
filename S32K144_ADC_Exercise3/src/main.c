#include <S32K144.h>

#define RED_LED_PIN   15
#define GREEN_LED_PIN 16
#define BLUE_LED_PIN  0

#define LOW_ADC_VALUE  1024
#define MID_ADC_VALUE  2048
#define HIGH_ADC_VALUE 3072 

void WDOG_disable(){
    IP_WDOG->CNT = 0xD928C520;
    IP_WDOG->TOVAL = 0xFFFF;
    IP_WDOG->CS = 0x2100; // Disable watchdog
}

void SOSC_init_8MHz(){
    IP_SCG->SOSCDIV = 0x00000101;
    IP_SCG->SOSCCFG = SCG_SOSCCFG_RANGE(2) | SCG_SOSCCFG_EREFS(1);
    while (IP_SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK);
    IP_SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN(1);
    while (!(IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));
}

void Clock_init(){
    /* Reset và cấu hình Clock cho ADC0 */
    IP_PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK; // Tắt trước
    IP_PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_PCS_MASK; // Xóa nguồn cũ
    IP_PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(1);    // Chọn SOSCDIV2
    IP_PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;  // Bật lại

    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK;
}

void Port_init(){
    IP_PORTA->PCR[6] &= ~PORT_PCR_MUX_MASK; // Analog Input
    
    /* Dùng dấu = để đảm bảo cấu hình sạch sẽ */
    IP_PORTD->PCR[RED_LED_PIN]   = PORT_PCR_MUX(1);
    IP_PORTD->PCR[GREEN_LED_PIN] = PORT_PCR_MUX(1);
    IP_PORTD->PCR[BLUE_LED_PIN]  = PORT_PCR_MUX(1);

    IP_PTD->PDDR |= (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | (1 << BLUE_LED_PIN);
    /* Tắt hết đèn ban đầu (Active Low) */
    IP_PTD->PSOR |= (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | (1 << BLUE_LED_PIN);
}

void ADC_init(){
    /* [SỬA LỖI] Dùng dấu = thay vì |= để xóa rác */
    /* MODE 12-bit, ADIV chia 1 */
    IP_ADC0->CFG1 = ADC_CFG1_ADIV(0) | ADC_CFG1_MODE(1);

    /* [QUAN TRỌNG] Tăng thời gian lấy mẫu (Sample Time) */
    /* Mặc định là 13 cycles (khoảng 0xC). Tăng lên 0xFF (255 cycles) để đọc biến trở ổn định hơn */
    IP_ADC0->CFG2 = ADC_CFG2_SMPLTS(0xFF);

    IP_ADC0->SC2 = 0x00000000;
    IP_ADC0->SC3 = 0x00000000;
}

/* Hàm đọc ADC có lọc trung bình cộng */
int Read_ADC_Average(void) {
    uint32_t sum = 0;
    int samples = 32; // Lấy mẫu 32 lần

    for (int i = 0; i < samples; i++) {
        /* 1. Kích hoạt */
        IP_ADC0->SC1[0] = ADC_SC1_ADCH(2);

        /* 2. Chờ */
        while (!(IP_ADC0->SC1[0] & ADC_SC1_COCO_MASK));

        /* 3. Cộng dồn vào biến tổng */
        sum += IP_ADC0->R[0];
    }

    /* 4. Trả về giá trị trung bình (Chia cho 16) */
    return (sum / samples);
}

int main(){
    WDOG_disable();
    SOSC_init_8MHz();
    Clock_init();
    Port_init();
    ADC_init();

    int ADC_Value = 0;

    while (1){
        ADC_Value = Read_ADC_Average();

        /* 1. Reset trạng thái đèn (Tắt hết) trước khi bật màu mới */
        IP_PTD->PSOR |= (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | (1 << BLUE_LED_PIN);

        /* 2. Bật đèn theo giá trị */
        if (ADC_Value >= HIGH_ADC_VALUE) {
            IP_PTD->PCOR |= (1 << RED_LED_PIN);
        }
        else if (ADC_Value >= MID_ADC_VALUE) {
            IP_PTD->PCOR |= (1 << GREEN_LED_PIN);            
        }
        else if (ADC_Value > LOW_ADC_VALUE) {
            IP_PTD->PCOR |= (1 << BLUE_LED_PIN);            
        }
        else {
            // Không làm gì (đã tắt hết ở trên)
        }

        /* Delay khoảng 10ms để mắt kịp nhìn */
        for(volatile int i=0; i<50000; i++);
    }
    return 0;
}
