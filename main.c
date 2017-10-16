#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "misc.h"

#define BAUDRATE	9600

#define USART1_GPIO	GPIOA		// USART1 (PA9 / PA10)
#define USART1_TX	GPIO_Pin_9
#define USART1_RX	GPIO_Pin_10

#define BUFFOR_LENGTH 12
volatile char received_string[BUFFOR_LENGTH + 1];

/*
 * The delay function;
 * 1ms seed;
 */
void Delay(__IO uint32_t ms)
{
	while(ms--);
}

/*
 * Function that initializes USART1;
 */
void Init_USART1(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	USART_InitTypeDef	USART_InitStruct;
	NVIC_InitTypeDef	NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = USART1_TX | USART1_RX;
	GPIO_InitStruct.GPIO_Mode - GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USART1_GPIO, &GPIO_InitStruct);

	GPIO_PinAFConfig(USART1_GPIO, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(USART1_GPIO, GPIO_PinSource10, GPIO_AF_USART1);

	USART_InitStruct.USART_BaudRate = BAUDRATE;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// enable receive interrupt
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	// enable transmission interrupt

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

void USART_send(USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		/*
		 * The LSR, line status register shows the current state of communication.
		 * Errors are reflected in this register.
		 * The state of the receive and transmit buffers is also available.
		 *
		 * 0	Data available
		 * 1	Overrun error
		 * 2	Parity error
		 * 3	Framing error
		 * 4	Break signal received / interrupt
		 * 5	Transmitter Holding Register is empty
		 * 6	Data Holding Register is empty, and line is idle
		 * 7	Errornous data in FIFO
		 */
		while(!(USARTx -> SR & 0x40));
		USART_SendData(USARTx, *s);
		*s++;
	}
}

void USART1_IRQHandler(void)
{

}

int main(void)
{

    while(1)
    {
    }
}
