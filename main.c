#include<reg52.h>

unsigned char code PuZh[] = {0x03, 0x08, 0xC1, 0x20, 0x08, 0x00, 0x00, 0x1E};
unsigned char code baozhan00001[] = {0xAA, 0x07, 0x02, 0x00, 0x0C, 0xBF };
unsigned char UartRxDataFlag = 0x00; 
unsigned char RxDataCount = 0x00;
#define UART_RX_BUF_LEN 30	
unsigned char xdata UartRxBuf[UART_RX_BUF_LEN];	//串口缓冲区大小
#define MIN_PKT_LEN      8   //主动读卡号的数据包长度为12字节
#define MAX_PKT_LEN      28		//主动读卡号+指定数据块的数据包长度为28字节
unsigned char buf[MAX_PKT_LEN];
unsigned char CardData[UART_RX_BUF_LEN];	
unsigned char j;
unsigned char i;
unsigned char ReverOrCoro ;
unsigned char cnt=0;
void UsartConfiguration();
void Delay10ms(unsigned int c);
void Out_Data (unsigned char Data[]);
void ProcessUartRxData(void);

sbit IN1=P1^0;
sbit IN2=P1^1;
sbit IN3=P1^2;
sbit IN4=P1^3;
sbit IN5=P1^4;
sbit IN6=P1^5;
sbit IN7=P1^6;
sbit IN8=P1^7;

sbit lsen1=P3^5;
sbit rsen1=P3^4;

sbit IO1 = P2^1;
sbit IO2 = P2^2;
sbit IO3 = P2^3;
sbit IO4 = P2^4;
sbit IO5 = P2^5;
sbit IO6 = P2^6;
sbit IO7 = P2^7;

unsigned int pos_x=0;
unsigned int pos_y=0;


/* void UART() interrupt 4  //串口中断
{
        ES = 0;
	if(RI)	//检测是否接收完成 当硬件接收一个数据时 RI 高位
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
        EA = 1;
} */
 





void  Motor_right1();   //????
void  Motor_left1();
void  Motor_right2();   //????
void  Motor_left2();




void Motor_left1(bit ReverOrCoro)    //???
{
	if(ReverOrCoro == 1)
	{
		IN1=1;
		IN2=0;
	}
	else
	{
		IN2=1;
		IN1=0;
	}
}

void Motor_left2(bit ReverOrCoro)    //??? ???
{
	if(ReverOrCoro == 1)
	{
		IN5=1;
		IN6=0;
	}
	else
	{
		IN5=0;
		IN6=1;
	}
}

void Motor_right1(bit ReverOrCoro)    //??? ???
{
	if(ReverOrCoro == 1)
	{
		IN3=1;
		IN4=0;
	}
	else
	{
	IN4=1;
	IN3=0;
	}
}

void Motor_right2(bit ReverOrCoro)    //??? ???
{
	if(ReverOrCoro == 1)
	{
	IN7=1;
	IN8=0;
	}
	else
	{
	IN8=1;
	IN7=0;
	}
}



void stop()
{
	IN1=0;
	IN2=0;
	IN3=0;
	IN4=0;
	IN5=0;
	IN6=0;
	IN7=0;
	IN8=0;
}

void qianjin()
{
    Motor_left1(1);
		Motor_right1(1);
    Motor_left2(1);
		Motor_right2(1);    
}


void turn_left()
{
    Motor_left1(1);
		Motor_right1(0);
    Motor_left2(1);
	Motor_right2(0);   
}

void turn_right()
{
    Motor_left1(0);
	Motor_right1(1);
    Motor_left2(0);
	Motor_right2(1);
}

void xunji()   //?????
{
  unsigned char flag;
	if((rsen1==0)&&(lsen1==0))
	{
	  flag=0;
	}
	else
   if((rsen1==1)&&(lsen1==0))
	{
	  flag=1;
	}
	else
	if((rsen1==0)&&(lsen1==1))
	{
	  flag=2;
	}
	else
	if((rsen1==1)&&(lsen1==1))
	{
		flag=3;
	}
	
	switch(flag)
	{
		case 0:qianjin();break;
		case 1:turn_right();break;
		case 2:turn_left();break;
		case 3:stop();break;
		default:break;
	}
		
}




//
//
//
void UART_SER (void) interrupt 4 //????????
{
   //unsigned char Temp;          //?????? 

   if(RI)                        //?????????
     {
	RI=0;                      //?????
	//Temp=SBUF;                 //???????
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
      //SBUF=Temp;                 //????????????

	  
	 }
   if(TI)                        //????????,??
     TI=0;
}
//
//04 16 03 20 00 62 72 74 00 01 00 00 00 00 00 00 00 00 00 00 00 AB
//
void main(void)
{
        UsartConfiguration();
        Delay10ms(10);
		EA = 1;
        ES=0;
        TI=0;        
        j =  sizeof(PuZh);

        for (i=0;i<j;i++)//
        {
            SBUF = PuZh[i];
            while (!TI);
            TI = 0;
        }

        ES=1;

        for(i=0;i<UART_RX_BUF_LEN;i++)
	        UartRxBuf[i] = 0;
        while(1)
        {
            xunji();
            ProcessUartRxData();
        }
}

void UsartConfiguration(void)
{
        SCON=0X50;
        TMOD=0X20;
        PCON=0X00;
        TH1=0XFD;
        TL1=0XFD;
        TR1=1;
        REN = 1;
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




void ProcessUartRxData(void)
{

        if(UartRxDataFlag)
        {
                for(i=0;i<29;i++)
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
                                IO1 = 0;
                                IO2 = 1;
                                IO3 = 1;
                                IO4 = 1;
                                IO5 = 1;
                                IO6 = 1;
                                IO7 = 1;
                        }
                        if((CardData[0]==0x62)&&(CardData[1] == 0x72)&&(CardData[2] == 0x74)&&(CardData[3]==0x00)&&(CardData[4] == 0x02))
                        {
                                IO1 = 1;
                                IO2 = 0;
                                IO3 = 1;
                                IO4 = 1;
                                IO5 = 1;
                                IO6 = 1;
                                IO7 = 1;
                        }
                }
                

                
					for(i=0;i<UART_RX_BUF_LEN;i++)	//清空串口缓冲区
					UartRxBuf[i] = 0x00;                 
        }
}