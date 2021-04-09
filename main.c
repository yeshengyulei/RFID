#include<reg52.h>

unsigned char code PuZh[] = {0x03, 0x08, 0xC1, 0x20, 0x08, 0x00, 0x00, 0x1E};
unsigned char code baozhan00001[] = {0xAA, 0x07, 0x02, 0x00, 0x0C, 0xBF };
unsigned char UartRxDataFlag = 0x00; 
unsigned char RxDataCount = 0x0;
#define UART_RX_BUF_LEN 30	
unsigned char xdata UartRxBuf[UART_RX_BUF_LEN];	//串口缓冲区大小
#define MIN_PKT_LEN      8   //主动读卡号的数据包长度为12字节
#define MAX_PKT_LEN      28		//主动读卡号+指定数据块的数据包长度为28字节
unsigned char buf[MAX_PKT_LEN];
unsigned char CardData[UART_RX_BUF_LEN];	
unsigned char j;
unsigned char i;
void UsartConfiguration();
void Delay10ms(unsigned int c);
void Out_Data (unsigned char Data[]);
void ProcessUartRxData(void);


//
//
//
void UART(void) interrupt 4  //串口中断
{
        ES = 0;
	if(RI==1)	//检测是否接收完成 当硬件接收一个数据时 RI 高位
	{
                RI = 0;
                if(UartRxDataFlag == 0x00)
                {
                        UartRxBuf[RxDataCount] = SBUF;
                        RxDataCount++;
                        if(RxDataCount >= UART_RX_BUF_LEN)
                        {
                                RxDataCount = UART_RX_BUF_LEN - 1;
                        }
                        if ((UartRxBuf[1] == RxDataCount)&&(UartRxBuf[1] >= MIN_PKT_LEN)&&(UartRxBuf[1] <= MAX_PKT_LEN))
                        {
                                UartRxDataFlag = 0x01;
                                RxDataCount = 0x00;
                        }
                        else if ((UartRxBuf[1]<MIN_PKT_LEN)&&(UartRxBuf[1] > MAX_PKT_LEN))
                        {
                                RxDataCount = 0x00;
                        }
                }
	}
        ES = 1;
}
//
//04 16 03 20 00 62 72 74 00 01 00 00 00 00 00 00 00 00 00 00 00 AB
//
void main(void)
{
        UsartConfiguration();
        Delay10ms(100);
      
        ES=0;
        TI=0;        
        j =  sizeof(PuZh);

        for (i=0;i<j;i++)//
        {
                SBUF = PuZh[i];
                while (!TI);
                TI = 0;
        }
        while(TI==0);
        TI=0;
        ES=1;

        for(i=0;i<UART_RX_BUF_LEN;i++)
	        UartRxBuf[i] = 0;
        while(1)
        {
               void ProcessUartRxData(void);
        }
}

void UsartConfiguration(void)
{
        SCON=0X50;
        TMOD=0X20;
        PCON=0X80;
        TH1=0XFA;
        TL1=0XFA;
        TR1=1;
}

void Delay10ms(unsigned int c)
{
    unsigned char a, b;
    for (;c>0;c--)
        {
                for (b=38;b>0;b--)
                {
                        for (a=130;a>0;a--);
                }         
        }      
}

void Out_Data (unsigned char Data[])
{       
        ES=0;
        TI=0;        
        j =  sizeof(Data);

        for (i=0;i<j;i++)//
        {
                SBUF = Data[i];
                while (!TI);
		TI = 0;
        }
        TI=0;
        ES=1;
}



void ProcessUartRxData(void)
{
	unsigned char pktlen;

        if(UartRxDataFlag)
        {
                pktlen  = UartRxBuf[1];
                for(i=0;i<pktlen;i++)
                {
                        buf[i] = UartRxBuf[i];
                }
                UartRxDataFlag = 0x00;
                if((buf[0] == 0x04)&&(buf[1] == 0x16)&&(buf[2] == 0x03)&&(buf[3] == 0x20)&&(buf[4] ==0x00))
                {
                        for(i = 0; i < 16 ; i++)
                        {
                                CardData[i] = buf[(5+i)];
                        }
                        if((CardData[0]==0x62)&&(CardData[1] == 0x72)&&(CardData[2] == 0x74)&&(CardData[3]==0x00)&&(CardData[4] == 0x01))
                        {
                                ES=0;
                                TI=0;        
                                j =  sizeof(baozhan00001);

                                for (i=0;i<j;i++)//
                                {
                                SBUF = baozhan00001[i];
                                while (!TI);
		                TI = 0;
                                }
        TI=0;
        ES=1;
}
                        }
                        return;

                }
	for(i=0;i<UART_RX_BUF_LEN;i++)	//清空串口缓冲区
		UartRxBuf[i] = 0x00;                 
        }
}