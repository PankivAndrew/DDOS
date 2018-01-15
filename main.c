#include "stm32f0xx.h"
#include <string.h>
#include <stm32f0xx_gpio_init.h>
#include <stm32f0xx_usart.h>
#include "APS.h"

volatile char buff[1024] = "";
volatile int idx = 0;


GPIO_InitTypeDef G;

//Timekeeping functions
volatile uint32_t MSec = 0;

void SysTick_Handler(void){
	MSec++;
}

//Delay function (milliseconds)
void Delay(uint32_t T){
	uint32_t MSS = MSec;
	while((MSec-MSS)<T) __NOP;
}
void rotate_simultaniously() {
//	APS_SetPositionDegree(GPIOA, GPIO_Pin_1, -90);
//
//	//Wait for cycle to end
//	APS_WaitForUpdate();
//
	APS_SetPositionDegree(GPIOA, GPIO_Pin_1, -90);

	//Wait for cycle to end
	APS_WaitForUpdate();

//	Delay(2000);
//
//	APS_SetPositionDegree(GPIOA, GPIO_Pin_0, -30);
//
//	//Wait for cycle to end
//	APS_WaitForUpdate();

}
void start_position() {
	APS_SetPositionDegree(GPIOA, GPIO_Pin_1, -90);

	//Wait for cycle to end
	APS_WaitForUpdate();
}
void rotate_food(uint32_t delay) {
	APS_SetPositionDegree(GPIOA, GPIO_Pin_1, 90);

	//Wait for cycle to end
	APS_WaitForUpdate();

	//Pointless delay...
	Delay(delay);

	APS_SetPositionDegree(GPIOA, GPIO_Pin_1, -90);

	//Wait for cycle to end
	APS_WaitForUpdate();
}
void rotate_water(uint32_t delay) {
	APS_SetPositionDegree(GPIOA, GPIO_Pin_1, -30);

	//Wait for cycle to end
	APS_WaitForUpdate();

	//Pointless delay...
	Delay(delay);

	APS_SetPositionDegree(GPIOA, GPIO_Pin_1, -90);

	//Wait for cycle to end
	APS_WaitForUpdate();
}





void UART_Init(void) {
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_StructInit(&USART_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  gpio_pinSetup_AF(GPIOA, GPIO_Pin_9, GPIO_AF_1, GPIO_OType_PP, GPIO_PuPd_UP,
      GPIO_Speed_50MHz);

  gpio_pinSetup_AF(GPIOA, GPIO_Pin_10, GPIO_AF_1, GPIO_OType_PP, GPIO_PuPd_UP,
      GPIO_Speed_50MHz);

  USART_InitStructure.USART_BaudRate = 9600;

  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


  USART_Init(USART1, &USART_InitStructure);
  NVIC_EnableIRQ(USART1_IRQn);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

  NVIC_InitStructure.NVIC_IRQChannelPriority = 1; //SET PRIORITY!!!

  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  USART_Cmd(USART1, ENABLE);
}


void USART1_IRQHandler(void){
  // TTL Handler
   char chartoreceive = 0;
  if((USART1->ISR & USART_ISR_RXNE) == USART_ISR_RXNE) {
	  chartoreceive = (char)(USART1->RDR);
	  buff[idx++] = chartoreceive;
	  if (chartoreceive == '\n' || chartoreceive == '\r'){
		puts(buff);
        idx = 0;
        memset(buff, 0, sizeof buff);
	  }
  }
  else {
    NVIC_DisableIRQ(USART1_IRQn);
  }
}

int main(void)
{
	SystemCoreClockUpdate();
	SystemInit();
	UART_Init();
    while(1)
    {

    }
}
