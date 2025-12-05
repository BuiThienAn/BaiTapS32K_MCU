#include "S32K144.h"

#include <stdio.h>

#define PIN_LED_RED 15
#define PIN_LED_GREEN 16
#define PIN_LED_BLUE 0

void delay_ms(int ms){
	for (int i = 0; i < ms; i++){
		for (int j = 0; j < 5000; j++){};
	}
};

void PORT_init(){
	IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;

	//RESET cau hinh MUX ban dau cho cac PIN LED
	IP_PORTD->PCR[PIN_LED_RED] &= ~PORT_PCR_MUX(0b111);
	IP_PORTD->PCR[PIN_LED_GREEN] &= ~PORT_PCR_MUX(0b111);
	IP_PORTD->PCR[PIN_LED_BLUE] &= ~PORT_PCR_MUX(0b111);

	//Tien hanh cau hinh MUX cho cac PIN LED
	IP_PORTD->PCR[PIN_LED_RED] |= PORT_PCR_MUX(1);
	IP_PORTD->PCR[PIN_LED_GREEN] |= PORT_PCR_MUX(1);
	IP_PORTD->PCR[PIN_LED_BLUE] |= PORT_PCR_MUX(1);

	//Data direction cho cac PIN LED
	IP_PTD->PDDR |= (1<<PIN_LED_RED) | (1<<PIN_LED_GREEN) | (1<<PIN_LED_BLUE);

	//Init cho cac PIN LED
	IP_PTD->PSOR |= (1<<PIN_LED_RED) | (1<<PIN_LED_GREEN) | (1<<PIN_LED_BLUE);
	/*SetOutput cho cac PIN de dau ra PIN la 5V -> Hieu dien the 0V -> LED TAT*/
};

int main(){
	PORT_init();
	while (1){
		//Turn on LED RED
		IP_PTD->PCOR |= (1<<PIN_LED_RED);
		IP_PTD->PSOR |= (1<<PIN_LED_GREEN) | (1<<PIN_LED_BLUE);
		delay_ms(3000);

		//Turn on LED GREEN
		IP_PTD->PCOR |= (1<<PIN_LED_GREEN);
		IP_PTD->PSOR |= (1<<PIN_LED_RED);
		delay_ms(3000);

		//Turn on LED BLUE
		IP_PTD->PCOR |= (1<<PIN_LED_BLUE);
		IP_PTD->PSOR |= (1<<PIN_LED_GREEN);
		delay_ms(3000);
	};
	return 0;
}

