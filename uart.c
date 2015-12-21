#include <GPM8F2202.h>
//#include "xstring.h"
#include "uart.h"
#include "ver.h"

void uart_send(uint8 tx_buff[],uint8 len)
{
	uint8 i;
	ES = 0;
	
	for(i=0;i<len;i++)
	{
		SBUF = tx_buff[i];
		while(!TI);
		TI=0;
	}
	ES = 1;	
}

void Uart_ISR() interrupt 4
{
	RI = 0;  							//UART Recept Flag is clean by firmware( Write 0 to clean)	
	rx_SBUFF = SBUF;
	if(rx_SBUFF == '#'){
		if(cmd_flag == 0){
			rx_buff[0] = '#';
			rx_index = 1;
			cmd_flag = 1;
		}else{
			cmd_flag = 0;
			if(rx_index == 17){
				rx_buff[17] = '#';
				rx_complete=1;
				rx_index = 0;
			}else{
				rx_buff[0] = '#';
				rx_index = 1;
				cmd_flag = 1;
			}
		}
	}else if(cmd_flag == 1){
		if(rx_index > 16){
			cmd_flag = 0;
			rx_index = 0;
			return;
		}
		rx_buff[rx_index ++] = rx_SBUFF;
	}

	mt7681_wdt=10000;	
}

