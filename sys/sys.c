#define _SYS_STM32_C_
#include "all.h"

/***************************************************************
		���ļ��� stm32 �������ʼ����غ���
***************************************************************/


#if 1
/***************************************************************
		ϵͳʱ�ӳ�ʼ����72 MHz
***************************************************************/
void sys_rcc_init(void)
{
/*  ʹ���ⲿRC���� */
	/* RCC system reset(for debug purpose) */
  	RCC_DeInit();

  	/* Enable HSE */
  	RCC_HSEConfig(RCC_HSE_ON);

  	/* Wait till HSE is ready */
  	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */	//AHBϵͳ����  72
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
  
    /* PCLK2 = HCLK */	//SPB��������72
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */ //36
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);
	
}

/***************************************************************
		gpio ��ʼ��
***************************************************************/
void sys_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_All ;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

/**********************************************************
	��ʱ��2
***********************************************************/
void sys_timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_truct;
	NVIC_InitTypeDef NVIC_truct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	//������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_truct.TIM_Period = 500;	//5ms �ж�һ��
	TIM_truct.TIM_Prescaler = 719;	//720��Ƶ
	TIM_truct.TIM_ClockDivision = 0;
	TIM_truct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2,&TIM_truct);			//��ʼ��
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//����ʱ���ж�
	TIM_Cmd(TIM2,ENABLE);						//ʹ�ܶ�ʱ��

	NVIC_truct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_truct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_truct.NVIC_IRQChannelSubPriority = 0;
	NVIC_truct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_truct);
}

/************************************************************************

	����1��ʼ��
	
**************************************************************************/
void sys_uart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
 	NVIC_InitTypeDef NVIC_InitStructure; 


	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);

	/* ���ùܽ� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	/* ���üĴ��� */
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1 

	/* �ж����� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	//ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //�ж�ռ�ȵȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //���ж�
	NVIC_Init(&NVIC_InitStructure);							  //��ʼ��

}
INT8U sys_uart1_getchar(void)
{
	INT8U ch=0;
	while(!(USART1->SR & USART_FLAG_RXNE));
	ch = USART_ReceiveData(USART1);
	//USART_ClearFlag(USART1,USART_FLAG_RXNE);
	return ch;
	
}
void sys_uart1_sendchar(uint8_t data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART1,data);	
}



#endif

