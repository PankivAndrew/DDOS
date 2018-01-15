//#include <time.h>
#include "stm32f0xx.h"
uint32_t prescaler_ms;
uint32_t prescaler3_ms;

void SystemClock_Config(void);
void Error_Handler(void);

inline uint32_t get_us()
{

 return 1000*HAL_GetTick();// ƒ”же грубо, а що зробиш?
}
#define LOOP_FREQ (SystemCoreClock/4000000)

// „астота виконанн€ циклу (4 такти ≥ одиниц€ -- мегагерци, щоб зручн≥ше на
// м≥кросекунди множити):
inline void udelay_asm (uint32_t useconds) {
 useconds *= LOOP_FREQ;

asm volatile("   mov r0, %[useconds]    \n\t"
			 "1: subs r0, #1            \n\t"
			 "   bhi 1b                 \n\t"
			 :
			 : [useconds] "r" (useconds)
			 : "r0");
}

void InitDelayTIM6(void) {
	prescaler3_ms = SystemCoreClock / 1000000;
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->CR1 |= TIM_CR1_CEN;
	TIM6->PSC = prescaler3_ms - 1;
}


void TIM6delay_us(uint16_t value){
	TIM6->ARR = value+100;
	TIM6->CNT = 0;
	while(TIM6->CNT!=value){}
}

void rotate_servo(int x){
	for(int i = 0; i<20000; ++i)
	{
		GPIOB->ODR |= GPIO_ODR_7;
		TIM6delay_us(x);
		GPIOB->ODR &= ~GPIO_ODR_7;
		TIM6delay_us(20000 - x);
	}
}

void open_damper(){
	rotate_servo(1800);
}

void close_dumper(){
	rotate_servo(1700);
}
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	RCC ->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_MODER7_0;

	InitDelayTIM6();
//	rotate_servo(1700);// 1700 горизонтально
//	rotate_servo(1800);//800/1800/2000 вертикально
	open_damper();
	TIM6delay_us(5000000);
	close_dumper();
	while(1)
    {
    }
    return 0;
}
