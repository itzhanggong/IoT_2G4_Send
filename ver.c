#include <GPM8F2202.h>
#include "ver.h"


//uart--------------------------------------------
uint8 rx_buff[RX_BUF_LENTH] = {0};
uint8 pre_cmd_buff[CMD_BUFF_LEN] = {0};
uint8 cmd_buff[CMD_BUFF_LEN] = {0};
uint8 rx_index = 0;
uint8 rx_timout = 0;
bit rx_complete = 0;
bit cmd_flag = 0;
uint8 rx_SBUFF = 0;
//------------------------------------------------

//timer-------------------------------------------
uint16 mt7681_wdt = 0;
uint16 Timer_Delay1 = 0;
uint8 timer0_xbase = 0;
//------------------------------------------------
 

//2.4G--------------------------------------------------
volatile uint8 	gReg7_high, gReg7_low;	// �Ĵ���7��д�ݴ����
uint8 Auto_Ack_Select=0;		// �Զ�Ӧ�𿪹�
//----------------------------------------------------

