#define _N_CPU_C_

#include "cpu_h.h"

/************************************************************************
ʱ��:2014.3.23
����: lza1205

	����stm32 ��boot ���ļ���cpu��صĴ��룬��ͬƽ̨��Ҫ�޸�

************************************************************************/
uint8_t __cpu_goto_app(uint32_t Addr)
{
	pFunction Jump_To_Application;
	__IO uint32_t JumpAddress; 

	if (((*(__IO uint32_t*)Addr) & 0x2FFE0000 ) == 0x20000000)
	{ 
		/* ��ַҪƫ��4 */
		JumpAddress = *(__IO uint32_t*) (Addr + 4);
		Jump_To_Application = (pFunction) JumpAddress; 

		/* ʹ��app��ջ */
		__set_MSP(*(__IO uint32_t*) Addr);

		/* ��ת���û�������ڵ�ַ */
		Jump_To_Application();
	}
	return 1;
}



/*********************************************************
	�������ԣ�����ת֮ǰ��Ҫ�Ƚ�ǰ���Ѿ��򿪵��ж϶����ε�
	��Ϊ��ת��ȥ��ʹ�õ����µ����������µ����������
	û�ж�Ӧ���жϣ����������
*********************************************************/
void __cpu_disable_int(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //�ж�ռ�ȵȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			  //���ж�
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	//ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //�ж�ռ�ȵȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			  //���ж�
	NVIC_Init(&NVIC_InitStructure);
}


