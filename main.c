#include "all.h"
//#include "my_list.h"
//#include <stdlib.h>
#include "cpu_h.h"
#include "boot.h"
#include "debug.h"
#include "timer.h"
#include "xmodem.h"

#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field.  */


void main_delay_ms(INT32U ms)
{
	INT32U i,j;
	for(i = 0; i < ms; i ++)
		for(j = 0; j < 214; j++)
			;
}
void USART_RCC_Configuration(void)
{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
}

/**
  * @brief  ���ô���1������������ŵ�ģʽ 
  * @param  None
  * @retval None
  */
void USART_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  ���ô���1����ʹ�ܴ���1
  * @param  None
  * @retval None
  */
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStruct;

	USART_RCC_Configuration();

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1

	USART_GPIO_Configuration();
}

INT8U dest_buf[10*1024];

int main(void)
{ 
	
	SystemInit();
	sys_uart1_init();
	sys_timer2_init();

	n_boot_memu();
	while(1)
	{
		
	}	
//	return 0;
}


