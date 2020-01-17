#define _N_BOOT_C_

#include "boot.h"
#include "command.h"
#include "string.h"
/*********************************************************************
ʱ��:2014.3.23
����: lza1205

	boot ����ز���
	���ļ���ƽ̨�޹�


*********************************************************************/
INT8U letf_mov[4] = {0x1b,0x5b,0x44,0x0};

static void delay_ms(INT32U ms)
{
	g_delay_ms = (ms/10);
	for(;;)
	{
		if(g_delay_ms <= 0)
			return ;
		if(g_compel_out == 1)
		{
			return ;
		}
	}
}

/**********************************************************************
	boot �˵�����

***********************************************************************/
void n_boot_memu(void)
{
	INT8U delay;
	
	//gt_boot_var.start_timeout = START_TIMEOUT;
	rd_boot_var();
	//gt_boot_var.start_timeout = START_TIMEOUT;
	command_init();

	send_str("\r\n\r\n\r\n\r\n\r\n");
	send_str("\r\nTo pay tribute to John von Neumann\r\n");
	send_str("����:			lza1205\r\n");
	n_boot_printf("�汾��:			1.0\r\n",1);
	send_str("�����˵����綼����Զ�����ز�ȥ�����綼�������硣\r\n");
	send_str("����һֱ�����ģ�ȴ�Ǳ�Զ����ҪԶ�ĵط�\r\n");
	send_str("                            ���������鹫����\r\n");
	send_str("����ʱ�����������ں�,���������������������ģʽ��\r\n");

	receive_input = receive_command;			//������ģʽ
	delay = gt_boot_var.start_timeout;
	while(delay)
	{
		n_boot_printf("%d\r\n",delay --);
		delay_ms(1000);
		if(g_compel_out == 1)
		{
			goto memu_loop;
		}
	}

	

	send_str("�����ں��У���ȴ� .....................................\r\n");

	/* ��ǰ���Ѿ��򿪵��ж϶��رյ� */
	__cpu_disable_int();
	
	__cpu_goto_app(gt_boot_var.kernel_addr_base);
	return ;

memu_loop:

	send_str("\r\n##################% stm32_boot1.0 &####################\r\n");
	send_str("��ѡ��:\r\n");
	send_str("   [s] ���� bin �ļ�\r\n");
	send_str("   [e] �������� flash\r\n");
	send_str("   [b] �����ں�\r\n");
	send_str("��ѡ���ܻ�������������: \r\n");
   	uart_rx_ok = 0;

command_loop:
	send_str_prefix(" ");
	get_str();					//�ȴ�����
	translate_command();

	goto command_loop;

//	send_str("[s] start kernel\r\n");
	
}




/**********************************************
	��������
***********************************************/
INT8U __str_cmp(INT8U *str1, INT8U *str2)
{
	INT8U i = 0;
	while(((*str2) != '\0') && ((*str1) != '\0'))
	{
		if((*str1) != (*str2))
			return 0;
		str1 ++;
		str2 ++;
		i ++;
	}
	if(((*str2) == '\0') && (((*str1) == '\0')))
	{
		return i;
	}
	return 0;
}
/* ȡ������ */
void translate_tag(void)
{
	INT8U *tmp_p;
	INT8U i;
	tmp_p = uart_rx_buf;
	i = 0;
	g_comman_num = 0;
	gp_comman_tag[i++] = tmp_p;
	while((*tmp_p) != '\0')
	{
		if((*tmp_p) == ' ')
		{
			(*tmp_p) = '\0';
			gp_comman_tag[i++] = tmp_p + 1;
			g_comman_num ++;
		}
		tmp_p ++;
	}
}
void translate_command(void)
{
	INT8U i,err;
//	volatile INT8U *com;
	err = 1;

	/* ֻ�������˻س� */
	if(uart_rx_index == 0)
	{
		//send_str_prefix(" ");
		return ;
	}		
	uart_rx_buf[uart_rx_index] = '\0';
	uart_rx_index = 0;
	translate_tag();				//�����ֽ�
//	com = uart_rx_buf;
	for(i = 0; i < command_index; i++)	//һ�����ȶ�
	{
//#if 0
//		err = __str_cmp(com, gt_comman[i].name);
//		if(err != 0)
//		{
//			//gt_comman[i].tag_p = uart_rx_buf + err + 1;
//			translate_tag();
//			break;
//		}
//#endif
		err = __str_cmp(gp_comman_tag[0], gt_comman[i].name);
		if(err != 0)
		{
			//gt_comman[i].tag_p = uart_rx_buf + err + 1;
			//translate_tag();
			break;
		}
	}
	if(err != 0)
		gt_comman[i].com_fun();
	else
		send_str("û���ҵ�������,��������������Ƿ���ȷ,�������롰help������ð�����\r\n");
}


/********************************************
		�������������� 
*********************************************/
volatile void receive_command(INT8U res)
{
	if(res == 0x0d) 			//�س���
	{
		uart_rx_ok = 1;
		//uart_rx_index = 0;
		send_str("\r\n");
		return ;
	}if(res == 0x08)			//ɾ��
	{
		if(uart_rx_index == 0)
			return;
		uart_rx_index --;
		send_str(letf_mov);
		send_str(" ");
		send_str(letf_mov);
		return ;
	}
	uart_rx_buf[uart_rx_index] = res;
	uart_rx_index = (uart_rx_index + 1)%RX_BUF_SIZE;
	sys_uart1_sendchar(res);
	
}


/**************************************************************
	��������
***************************************************************/
volatile void receive_data(INT8U data)
{
	
}


/**************************************************************
	����ϵͳ���� 
**************************************************************/
void rd_boot_var(void)
{
	flash_rd(N_VAR_ADDR_BASE, flash_buf, sizeof(struct boot_var) + 1);
	if(flash_buf[0] != 0xe6)
	{
		default_var();
	}else{
		memcpy(&gt_boot_var, flash_buf+1,sizeof(struct boot_var) + 1);
		//flash_erase_page(N_VAL_ADDR_BASE);
		//flash_wr(N_VAL_ADDR_BASE, flash_buf, 1024);
	}
}
/******************************************************************
	д��ϵͳ����
******************************************************************/
void wr_boot_var(void)
{
	flash_buf[0] = 0xe6;
	memcpy(flash_buf+1, &gt_boot_var, sizeof(struct boot_var) + 1);
	flash_erase_page(N_VAR_ADDR_BASE);
	flash_wr(N_VAR_ADDR_BASE, flash_buf, sizeof(struct boot_var) + 1);
}


void printf_var(void)
{
	n_boot_printf("start_timeout:      %d                 ϵͳ������ʱ\r\n",gt_boot_var.start_timeout);
	n_boot_printf("mach:               %d                 ������\r\n",gt_boot_var.mach);
	n_boot_printf("boot_addr_base:     %d                 boot ��ŵ�ַ\r\n",gt_boot_var.boot_addr_base);
	n_boot_printf("boot_var_addr_base  %d                 var ��ŵ�ַ\r\n",gt_boot_var.boot_var_addr_base);
	n_boot_printf("kernel_addr_base    %d                 kernel ��ŵ�ַ\r\n",gt_boot_var.kernel_addr_base);
}
/*******************************************************************
	ϵͳĬ�ϲ���
********************************************************************/
void default_var(void)
{
	gt_boot_var.start_timeout		= START_TIMEOUT;
	gt_boot_var.mach				= 0x58;
	gt_boot_var.boot_addr_base		= N_BOOT_ADDR_BASE;
	gt_boot_var.boot_size			= N_BOOT_SIZE;
	gt_boot_var.boot_var_addr_base	= N_VAR_ADDR_BASE;
	gt_boot_var.boot_var_size		= N_VAR_SIZE;
	gt_boot_var.kernel_addr_base	= N_KENREL_ADDR_BASE;
	flash_buf[0] = 0xe6;
	memcpy(flash_buf+1, &gt_boot_var, sizeof(struct boot_var) + 1);
	flash_erase_page(N_VAR_ADDR_BASE);
	flash_wr(N_VAR_ADDR_BASE, flash_buf, sizeof(struct boot_var) + 1);
}


/********************************************************************
		����ַ���
********************************************************************/
//����ǰ׺��ʾ��
void send_str_prefix(INT8U *str)
{
	send_str(str);
	send_str("# stm32_boot #  $>>:");
}

void send_str(INT8U *str)
{
	while((*str) != '\0')
	{
		send_char((*str++));
	}
}

void send_var(INT32U var)
{
	int i,temp = 1,j = 1, num_tmp;
	num_tmp = var;
	for(i = 0; i < 10; i++)
	{
		temp = temp * 10;
		if(var < temp)	break;
		j++;
	}
	for(i = 0; i < j; i++)
	{
		temp = temp/10;
		num_tmp = var/temp;
		var = var % temp;
		send_char(num_tmp + 0x30);	
	}
}

/* ��ʮ�����Ʒ��� */
void send_hex(INT32U var)
{
	int i,temp = 1,j = 1, num_tmp;
	num_tmp = var;
	for(i = 0; i < 16; i++)
	{
		temp = temp * 16;
		if(var < temp)	break;
		j++;
	}
	send_str("0x");
	for(i = 0; i < j; i++)
	{
		temp = temp/16;
		num_tmp = var/temp;
		var = var % temp;
		if(num_tmp < 10)
			send_char(num_tmp + 0x30);	
		else
			send_char(num_tmp - 10 + 97);
	}	
}

void n_boot_printf(INT8U *str, INT32U var)
{
	while((*str) != '\0')
	{
		if((*str) == '%')
		{
			str++;
			if((*str) == 'd')
				send_var(var);
			else if((*str) == 'x')
				send_hex(var);
		}else{
			send_char((*str));
		}
		str ++;
	}
}

/****************************************************************
	��ȡ�������������
****************************************************************/
void get_str(void)
{
	while(uart_rx_ok != 1)
		;
	uart_rx_ok = 0;
}


/********************************************************************
	ʹ�ú���
*******************************************************************/
INT32U str_to_hex(INT8U *str)
{
	INT32U s = 0;
	while((*str) != '\0')
	{
		s *= 10;
		s += ((*str)-0x30);
		str ++;
	}
	return s;
}


