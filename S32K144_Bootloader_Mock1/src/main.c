/*
 * main.c
 * Bootloader Application (BOOT APP)
 */
#include "S32K144.h"
#include "Driver_USART.h"
#include "Driver_GPIO.h"
#include "QUEUE.h"
#include "SREC.h"
#include <s32_core_cm4.h>
#include <string.h>
#include "FLASH.h"
#include <stdint.h>

extern ARM_DRIVER_USART Driver_USART0;
extern ARM_DRIVER_GPIO Driver_GPIO1; /* GPIO1 quản lý Port C */

/* Địa chỉ bắt đầu của USER APP (Theo đề bài) */
#define USER_APP_START_ADDR    0x0000A000UL

/* Nút Boot là chân PTC13 (Port C, Pin 13) */
#define BOOT_BUTTON_PIN        13U

/* Định nghĩa buffer */
#define APP_BUFFER_SIZE 100U
uint8_t app_buffer[APP_BUFFER_SIZE];
SREC_Record_t SREC_line_data;

typedef void (*pFunction)(void);

/**
 * @brief Thiết lập Main Stack Pointer (MSP) cho User App
 */
static inline void My_Set_MSP(uint32_t topOfMainStack)
{
    __asm volatile ("MSR msp, %0" : : "r" (topOfMainStack) : "memory");
}

/* --- System Clocks --- */
void SOSC_init_8MHz(void)
{
    IP_SCG->SOSCDIV = 0x00000101;
    IP_SCG->SOSCCFG = SCG_SOSCCFG_RANGE(0b10) | SCG_SOSCCFG_HGO(0b0) | SCG_SOSCCFG_EREFS(0b1);
    while (IP_SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK);
    IP_SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN(0b1);
    while (!(IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));
}
void SPLL_init_160MHz(void)
{
    IP_SCG->SPLLDIV = 0x00000302;
    IP_SCG->SPLLCFG = SCG_SPLLCFG_MULT(0b11000) | SCG_SPLLCFG_PREDIV(0b0);
    while (IP_SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK);
    IP_SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN(0b1);
    while (!(IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));
}
void NormalRunMode_Init(void)
{
    IP_SCG->RCCR = SCG_RCCR_SCS(0b110) | SCG_RCCR_DIVCORE(0b01) | SCG_RCCR_DIVBUS(0b0011) | SCG_RCCR_DIVSLOW(0b0101);
    while (((IP_SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 6) {};
}



/* Hàm nhảy sang User App */
void JumpToUserApp(uint32_t start_addr)
{
    /* 1. Lấy địa chỉ Reset Handler từ Vector Table của User App */
    uint32_t jump_address = Read_FlashAddress(start_addr + 4U);
    pFunction jump_to_app = (pFunction)jump_address;

    /* 2. Tắt hết ngắt để đảm bảo quá trình nhảy an toàn */
    DISABLE_INTERRUPTS();

    /* 3. Thiết lập MSP (Main Stack Pointer) mới */
    My_Set_MSP(Read_FlashAddress(start_addr));

    /* 4. Nhảy! */
    jump_to_app();
}

/* Hàm tiện ích để báo lỗi SREC cụ thể */
void SREC_ReportError(SREC_Status_t error_code)
{
    const uint8_t *msg = (const uint8_t *)"Unknown Error";

    switch (error_code)
    {
        case SREC_ERROR_NULL_PTR:
            msg = (const uint8_t *)"ERROR: Null Pointer detected!";
            break;
        case SREC_ERROR_TYPE:
            msg = (const uint8_t *)"ERROR: Invalid S-Record Type (Start char not 'S')!";
            break;
        case SREC_ERROR_FORMAT:
            msg = (const uint8_t *)"ERROR: Invalid Format (Length mismatch)!";
            break;
        case SREC_ERROR_CHECKSUM:
            msg = (const uint8_t *)"ERROR: Checksum Mismatch!";
            break;
        default:
            break;
    }

    Driver_USART0.Send((const uint8_t *)"\r\n>> FATAL ERROR: ", 0U);
    Driver_USART0.Send(msg, 0U);
    Driver_USART0.Send((const uint8_t *)"\r\n>> System Halted. Please reset board.\r\n", 0U);
}

int main(void) {
    /* 1. Khởi tạo Clock hệ thống */
    SOSC_init_8MHz();
    SPLL_init_160MHz();
    NormalRunMode_Init();

    /* 2. QUAN TRỌNG: Copy hàm thực thi Flash sang RAM */
    /* Hàm này nằm trong FLASH.c, cần được gọi trước bất kỳ lệnh ghi/xóa nào */
    Mem_43_INFLS_IPW_LoadAc();

    /* 3. Bật ngắt toàn cục (để GPIO và UART hoạt động) */
    ENABLE_INTERRUPTS();

    /* 4. Kiểm tra nút Boot (PTC13) */
    Driver_GPIO1.Setup(BOOT_BUTTON_PIN, NULL);
    Driver_GPIO1.SetDirection(BOOT_BUTTON_PIN, ARM_GPIO_INPUT);

    /* Logic:
       - Giả sử có trở kéo lên (Pull-up).
       - Không nhấn = Logic 1 (High).
       - Nhấn = Logic 0 (Low).
    */

    /* Nếu KHÔNG NHẤN (Input == 1) -> Kiểm tra và nhảy sang User App */
    if (Driver_GPIO1.GetInput(BOOT_BUTTON_PIN) != 0U)
    {
        /* Kiểm tra xem địa chỉ ứng dụng có dữ liệu hợp lệ không (Khác 0xFFFFFFFF) */
        /* Chỉ kiểm tra MSP (4 byte đầu), nếu cần kỹ hơn có thể check checksum */
        if (Read_FlashAddress(USER_APP_START_ADDR) != 0xFFFFFFFFUL)
        {
            JumpToUserApp(USER_APP_START_ADDR);
        }
        /* Nếu User App trống, chương trình sẽ tự trôi xuống dưới để chạy Bootloader */
    }

    /* --- BOOTLOADER MODE (Do nhấn nút hoặc App trống) --- */

    Driver_USART0.Initialize(NULL);
    Driver_USART0.Send((const uint8_t *)"\r\n=== BOOT APP MODE ===\r\n", 0U);
    Driver_USART0.Send((const uint8_t *)"Request: Please send SREC file now...\r\n", 0U);

    /* 5. Xóa vùng nhớ User App trước khi ghi mới */
    Driver_USART0.Send((const uint8_t *)"Erasing Flash... ", 0U);

    /* Tắt ngắt khi xóa Flash để tránh treo CPU (do mã ISR nằm trên Flash) */
    DISABLE_INTERRUPTS();
    Erase_Multi_Sector(USER_APP_START_ADDR, 118U); /* Xóa từ 0xA000 đến hết */
    ENABLE_INTERRUPTS();

    Driver_USART0.Send((const uint8_t *)"Done.\r\n", 0U);

    while (1) {
            /* Nhận dữ liệu từ Queue */
            int32_t status = Driver_USART0.Receive(app_buffer, APP_BUFFER_SIZE);

            if (status == (int32_t)ARM_DRIVER_OK)
            {
                /* 1. Gọi hàm Parse và lưu kết quả trả về */
                SREC_Status_t parse_status = SREC_ParseLine(app_buffer, &SREC_line_data);

                /* 2. Kiểm tra kết quả */
                if (parse_status == SREC_OK)
                {
                    /* --- TRƯỜNG HỢP THÀNH CÔNG (Code cũ) --- */

                    /* Ghi dữ liệu (S1, S2, S3) */
                    if ((SREC_line_data.type >= 1U) && (SREC_line_data.type <= 3U))
                    {
                        uint8_t i;
                        for (i = 0; i < SREC_line_data.data_length; i += 8)
                        {
                            DISABLE_INTERRUPTS();
                            Program_LongWord_8B(SREC_line_data.address + i, &SREC_line_data.data[i]);
                            ENABLE_INTERRUPTS();
                        }
                    }

                    /* Kết thúc (S7, S8, S9) */
                    if ((SREC_line_data.type >= 7U) && (SREC_line_data.type <= 9U))
                    {
                        Driver_USART0.Send((const uint8_t *)"\r\nDownload Complete!\r\n", 0U);
                        Driver_USART0.Send((const uint8_t *)"Please RESET board to run new App.\r\n", 0U);
                    }
                }
                else
                {
                    /* --- TRƯỜNG HỢP CÓ LỖI (Mới thêm) --- */

                    /* Báo lỗi cụ thể ra màn hình */
                    SREC_ReportError(parse_status);

                    /* Dừng ngay chương trình (Infinite Loop) */
                    /* Lúc này người dùng phải ấn nút Reset trên mạch mới chạy lại được */
                    while(1)
                    {
                        /* Có thể thêm code nháy đèn LED đỏ ở đây để báo hiệu */
                    }
                }
            }
        }
    return 0;
}
