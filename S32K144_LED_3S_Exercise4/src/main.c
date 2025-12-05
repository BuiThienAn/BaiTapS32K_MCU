#include <stdio.h>
#include "S32K144.h"
#include <s32_core_cm4.h>

#define RED_LED_PIN 15
#define NVIC_ISER1 (*((volatile uint32_t *)0xE000E104u)) //Dinh nghia tu file NVIC cua ARM
#define LPIT_count 23999999
//Chọn SOSCDIV2 (8MHz) làm src: 3/(1/8MHz)-1=23999999

void SOSC_init_8MHz(){
    IP_SCG->SOSCDIV = 0x00000101; //SOSCDIV1=1 và SOSCDIV2=1
    IP_SCG->SOSCCFG = SCG_SOSCCFG_RANGE(0b10) //Cấp điện áp cho thạch anh 1-8MHz
                    | SCG_SOSCCFG_HGO(0) //xtal osc low power
                    | SCG_SOSCCFG_EREFS(1); //EREF=1 Internal SOSC
    while (IP_SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK){};
    IP_SCG->SOSCCSR |= SCG_SOSCCSR_SOSCEN(1);
    while (!(IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)){};
};

void SPLL_init_160MHz(){
    IP_SCG->SPLLDIV = 0x00000302; //SPLLDIV2=4 SPLLDIV1=2
    IP_SCG->SPLLCFG = SCG_SPLLCFG_MULT(0b11000) //MULT=24
                     |SCG_SPLLCFG_PREDIV(0); //PREDIV=0 ->8/(0+1)*(24+16)/2=160MHz
    while (IP_SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK){};
    IP_SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN(1); //Enable System PLL
    while (!(IP_SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)){};
};

void NormalRunMode_Init(){
    IP_SCG->RCCR = SCG_RCCR_SCS(0b110)
                  |SCG_RCCR_DIVCORE(0b01) //CORE 160/2=80MHz
                  |SCG_RCCR_DIVBUS(0b0011)  //BUS 160/4=40MHz
                  |SCG_RCCR_DIVSLOW(0b0101); //FLASH 160/6=26.67 MHz
    while (((IP_SCG->CSR & SCG_CSR_SCS_MASK)>>SCG_CSR_SCS_SHIFT) != 6){};
}

void Clock_init(){
    IP_PCC->PCCn[PCC_LPIT_INDEX] &= ~PCC_PCCn_PCS_MASK;
    IP_PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_PCS(0b001); //Chọn SOSCDIV2=8MHz làm src

    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
};

void Port_init(){
    IP_PORTD->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);

    IP_PTD->PDDR |= (1 << RED_LED_PIN);
    IP_PTD->PSOR |= (1 << RED_LED_PIN);
};

void LPIT_init(){
    IP_LPIT0->MCR |= LPIT_MCR_M_CEN_MASK; //Bat module
    IP_LPIT0->MIER |= LPIT_MIER_TIE0_MASK; //Cho phep ngat o chanel 0
    IP_LPIT0->TMR[0].TVAL = LPIT_count;
    IP_LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_MODE_MASK;
    IP_LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
    //NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
    NVIC_ISER1 = (1<<16);
};

void LPIT0_Ch0_IRQHandler(){
    if(IP_LPIT0->MSR & LPIT_MSR_TIF0_MASK){
        IP_LPIT0->MSR = LPIT_MSR_TIF0_MASK;
        IP_PTD->PTOR |= (1<<RED_LED_PIN);
    }
};

void WDOG_disable(){
    IP_WDOG->CNT = 0xD928C520; //unlock watchdog
    IP_WDOG->CNT = 0xB480A602; // refresh watchdog
    IP_WDOG->CS &= ~WDOG_CS_EN_MASK; //disable watchdog
};

int main(){
    WDOG_disable();
    SOSC_init_8MHz();
    SPLL_init_160MHz();
    NormalRunMode_Init();
    Clock_init();
    Port_init();
    LPIT_init();
    ENABLE_INTERRUPTS(); //Cho phép ngắt toàn cục
    while (1){/*do something*/};
    return 0;
}
