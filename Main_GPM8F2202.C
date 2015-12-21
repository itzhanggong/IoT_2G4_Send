#include <GPM8F2202.h>
#include "xstring.h"
#include "uart.h"
#include "ver.h"
#include "spi.h"

void reset_mt7681()
{
	ES = 0;					//disable Uart Interrupt(Optional)
	MT7681_RESET=0;
	mt7681_wdt=8;
	while(mt7681_wdt);
	MT7681_RESET = 1;
	mt7681_wdt=10000;
	ES = 1;					//enable Uart Interrupt(Optional)
	SEND_VER;
}

void Init()
{
	
	P3_PU = P3 =0xFF; 					//Set P3 as input pull high stauts
    P3_PD = 0x18;						//set P3^4,P3^3 output;
	
	P0_PU = P0_PD = 0x3B;				//Set P0^5, P0^4, P0^3, P0^1, P0^0 output;			

	SPICS	= 1;
	SPICLK	= 0;
	RFRST	= 0;			
	
	IOSCCON = 0x00;					//set Clock Source = 16Mhz;
	
	SCON	= 0x50;					//Set UART as mode1 and enable reception.
	TMOD	= 0x22;					//Initialize Timer1 as mode2.
	PCON	= 0x80;					//Set SMOD =1
	TL1		= 0xF7;					//Set Boud rate as 9600bit/s
	TH1		= 0xF7;
	TL0		= 0x7A;
	TH0		= 0x7A;
	EA 		= 1;
	ET0		= 1;
	TR1		= 1;					//Start Timer1 Counter
	TR0		= 1;					//Start Timer0 Counter
	reset_mt7681();
}


uint16 RF_init()
{
	RFRST= 0; 	
	Wait_mSec(100);
	RFRST= 1; 							//Enable PL1167
	Wait_mSec(500); 					//delay 500ms to let PL1167 stable	~~~~~~~~~
	
	Reg_write16(0x00,0x6f,0xe0);
	Reg_write16(0x01,0x56,0x81);
	Reg_write16(0x02,0x66,0x17);
	Reg_write16(0x04,0x9c,0xc9);
	Reg_write16(0x05,0x66,0x37);
	Reg_write16(0x07,0x00,0x30);
	Reg_write16(0x08,0x6c,0x90);
	Reg_write16(0x09,0x48,0x00); 
	Reg_write16(0x0a,0x7f,0xfd);
	Reg_write16(0x0b,0x00,0x08);
	Reg_write16(0x0c,0x00,0x00);
	Reg_write16(0x0d,0x48,0xbd);
	Reg_write16(0x16,0x00,0xff);
	Reg_write16(0x17,0x80,0x05);
	Reg_write16(0x18,0x00,0x67);
	Reg_write16(0x19,0x16,0x59);
	Reg_write16(0x1a,0x1b,0xe0); 
	Reg_write16(0x1b,0x13,0x00);
	Reg_write16(0x1c,0x18,0x00);
	Reg_write16(0x20,0x40,0x00); 
	Reg_write16(0x21,0x3f,0xc7);
	Reg_write16(0x22,0x20,0x00);
	Reg_write16(0x23,0x05,0x80);
								//Begin Syncword				
	Reg_write16(0x24,0x42,0x31);
	Reg_write16(0x25,0x86,0x75);
	Reg_write16(0x26,0x9a,0x0b);
	Reg_write16(0x27,0xde,0xcf);
								//End syncword
	Reg_write16(0x28,0x44,0x01);
	Reg_write16(0x29,0xb8,0x00); 
	Reg_write16(0x2a,0xfd,0xb0);
	Reg_write16(0x2b,0x00,0x0f);
	Wait_mSec(10);				//delay 10ms to let PL1167 for operation					
	return GetReg7FromChip();
}



void main()
{
 	Init(); 
	CKCON = WatchDogTimer_11;
	while(RF_init() != 0x0030){
		RF_init();
	}
	 
//	xWatchdogSet
	while(1)
	{
//		xWatchdogClr 
		if(rx_complete){
			SEND_ACK;
			rx_complete=0;
			xmemcpy(cmd_buff, &rx_buff[1], CMD_BUFF_LEN);
			//_parseCmd();
		 	spi_Send(cmd_buff, CMD_BUFF_LEN);			//·¢ËÍÊý¾Ý
		}

		if(mt7681_wdt==0){
			reset_mt7681();
		}
	}
}
