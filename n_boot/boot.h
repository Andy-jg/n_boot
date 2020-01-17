#ifndef _N_BOOT_H_
#define _N_BOOT_H_

#ifdef _N_BOOT_C_
#define _N_BOOT_EXT_
#else
#define _N_BOOT_EXT_ extern
#endif

#include "all.h"

#include "sys.h"
#include "cpu_h.h"
//#include "my_list.h"


/* boot ϵͳ���� */
struct boot_var{
	INT32U start_timeout;		//�ں�������ʱ
	INT32U mach;				//����ID

	INT32U boot_addr_base;		//boot��ַ
	INT32U boot_size;			//boot����δ�С
	INT32U boot_var_addr_base;	//boot ������ַ
	INT32U boot_var_size;		//boot�����δ�С
	INT32U kernel_addr_base;	//AP �ں˵�ַ

};


#define START_TIMEOUT 5					/* ������ʱ 5 ��*/

#define RX_BUF_SIZE		(2*1024)			/* ���ջ������Ĵ�С */

_N_BOOT_EXT_ struct boot_var gt_boot_var;		//ϵͳ����

//_N_BOOT_EXT_ INT8U gt_boot_var.start_timeout;		//�ں�������ʱ

_N_BOOT_EXT_ volatile INT32S g_delay_ms;		//��ʱ
_N_BOOT_EXT_ volatile INT8U g_compel_out;	//ǿ�ƴ���ʱ���˳�

_N_BOOT_EXT_ volatile INT8U uart_rx_buf[RX_BUF_SIZE];	//uart ���ջ�����
_N_BOOT_EXT_ volatile INT8U uart_rx_index;		//д���±�

_N_BOOT_EXT_ volatile INT8U uart_rx_ok;			//�Ѿ����յ�һ���س��������Զ����ݽ��д�����

_N_BOOT_EXT_ INT8U flash_buf[1024];			//����flash �Ļ���


_N_BOOT_EXT_ volatile void (*receive_input)(INT8U res);		//���봦����ָ��


void n_boot_memu(void);
void rd_boot_var(void);
void wr_boot_var(void);
void default_var(void);

volatile void receive_command(INT8U res);

void translate_tag(void);
void translate_command(void);


void send_str_prefix(INT8U *str);
void send_str(INT8U *str);
void send_var(INT32U var);
void n_boot_printf(INT8U *str, INT32U var);

void get_str(void);

INT8U __str_cmp(INT8U *str1, INT8U *str2);
INT32U str_to_hex(INT8U *str);




#endif






