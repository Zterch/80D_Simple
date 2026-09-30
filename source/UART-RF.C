

#include "include\includes.h"

uchar gxjishu1,gxjishu2,gxjishu3;
uchar RF_errojishu,RF_errojishu2,RF_errojishu3,RF_errojishu4=0;

uchar idata rinterruptrf=rinterrupt_Norf;/*rf接收完成标志*/
uchar rbufrf[30];/*串口rf接收字符串数组*/
uchar data_rf[20];

uchar *Ptr_Sendrf = 0;/*发送字符串指针*/
uint SendCountrf = 0;/*串口发送字节计数*/
uchar busyrf = 0;/*串口发送字节串完毕标志，=1 未完成=0已完成*/
uint RevCountrf = 0;/*串口接收字节计数*/

uchar g_cPackageStarted;
uchar g_cReceiveCounter;
uchar g_cCheckSum;
bit g_bReceiveOK;
bit g_b0x10Received;

#define 	COMM_CONTROL_ANTENNA		0X05	//模块天线控制、自动寻卡设置
#define 	COMM_SET_COM_BPS			0X15	//串口波特率设置
#define 	COMM_SET_TYPE_AB_OTHER		0X3A	//设定读卡模式TYPE A/B
#define 	COMM_MIFARE1_PCD_REQUEST	0X46	//mifare one 寻卡
#define 	COMM_MIFARE1_PCD_ANTICOLL	0X47	//mifare one 防冲撞
#define 	COMM_MIFARE1_PCD_SELECT		0X48	//mifare one 选卡
#define 	COMM_AUTHENTICATION2		0X4A	//认证2
#define 	COMM_PCD_READ_BLOCK			0X4B	//无密码读块
#define 	COMM_PCD_WRITE_BLOCK		0X4C	//无密码写块



unsigned char code COMM_CONTROL_ANTENNA_OFF[]     //关天线
= {0X00,0X00,
   0x04, 
   COMM_CONTROL_ANTENNA, 
   0x00};
unsigned char code COMM_CONTROL_ANTENNA_ON[]    //开天线
= {0X00,0X00,
   0x04, 
   COMM_CONTROL_ANTENNA, 
   0x01};
unsigned char code COMM_SET_COM_BPS_[]  //每次上电波特率为115200
= {0X00,0X00,
   0x04, 
   COMM_SET_COM_BPS, 
   0x07};
unsigned char code COMM_SET_TYPE_AB_OTHER_[]  //模式为s50卡
= {0X00,0X00,
   0x04, 
   COMM_SET_TYPE_AB_OTHER, 
   0x41};
unsigned char code COMM_MIFARE1_PCD_REQUEST_[]   //寻卡
= {0X00,0X00,
   0x04, 
   COMM_MIFARE1_PCD_REQUEST, 
   0x52};
unsigned char code COMM_MIFARE1_PCD_ANTICOLL_[]   //防冲突
= {0X00,0X00,
   0x04, 
   COMM_MIFARE1_PCD_ANTICOLL, 
   0x04};
unsigned char COMM_MIFARE1_PCD_SELECT_[]  //选卡
= {0X00,0X00,
   0x07, 
   COMM_MIFARE1_PCD_SELECT, 
   0x00,0x00,0x00,0x00};
unsigned char code COMM_AUTHENTICATION2_[]    //A秘钥认证  扇区2
= {0X00,0X00,
   0x0B, 
   COMM_AUTHENTICATION2, 
   0x60,0x05,0x01,0x00,0x64,0x39,0x20,0x84};
unsigned char code COMM_PCD_READ_BLOCK_4[]  	  //读块4
= {0X00,0X00,
   0x04, 
   COMM_PCD_READ_BLOCK, 
   0x04};
unsigned char code COMM_PCD_READ_BLOCK_5[]  	  //读块5
= {0X00,0X00,
   0x04, 
   COMM_PCD_READ_BLOCK, 
   0x05};
unsigned char COMM_PCD_WRITE_BLOCK_[]    //写块5
= {0X00,0X00,
   0x14, 
   COMM_PCD_WRITE_BLOCK, 
   0x05,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16};




 void UART1_Init( void )
{											
    SCON1  = 0x50; // SCON0: mode 1, 8-bit UART, enable RX
    PCON  |= 0x10;
    EIE2  |= 0x40; /*UART1 中断允许es1=1*/
}




static UART1_ISR( void ) interrupt 20
{
   	static uchar data rbufrf_temp;
	EA = 0;
    if( SCON1& ( 1<<1 ) )
    {
        //TX ok
        SCON1 &= ~( 1<<1 );
		if( SendCountrf!= 0 )
		{
		 	SBUF1= *Ptr_Sendrf++;
			SendCountrf--;
			busyrf= 1;
		} 
		else
		{
		 	busyrf= 0;
		}

    }
    else if( SCON1& ( 1<<0 ) )
    {
        //RX ok
        SCON1&= ~( 1<<0 );
        rbufrf_temp  = SBUF1;	
		if (!g_bReceiveOK)		//接收该包数据
		{
 				
			if ((0x02 == rbufrf_temp) && (0 == g_b0x10Received))
			{
				g_cPackageStarted = 1;		
				g_cReceiveCounter = 0;
				g_cCheckSum = 0;
			}
			else if (( 0x03 == rbufrf_temp) && (0 == g_b0x10Received) && (g_cPackageStarted))		//package end
			{				//check package
				g_cPackageStarted = 0;				
				if(g_cReceiveCounter < sizeof(rbufrf) - 2)
				{
					g_bReceiveOK = 1;
					g_cPackageStarted = 0;
					g_cReceiveCounter = 0;
				}
			}
			else if (( 0x10 == rbufrf_temp ) && (0 == g_b0x10Received))
			{
				g_b0x10Received = 1;
			}
			else if (g_cPackageStarted)
			{
				g_b0x10Received = 0;

				if (g_cReceiveCounter < sizeof(rbufrf) - 2)
				{
					if (g_cReceiveCounter != 0)
					{
						g_cCheckSum += rbufrf[g_cReceiveCounter-1];	
					}
					rbufrf[g_cReceiveCounter++] = rbufrf_temp;				
				}
				else
				{
					g_cPackageStarted 		= 0;		//标准串口接收包起始标志
					g_cReceiveCounter		= 0;
				}
			}
			else
			{
				 g_b0x10Received = 0;   //没收到02头时，若收到10就永远不再接受
			}
		}
						
    }
	EA = 1;
}


/*发送1  个字节*/
void UART1_Senddata(unsigned char buf)
{
	while( busyrf );
	SBUF1= buf;
	SendCountrf = 0;
	busyrf = 1;
	while( busyrf );
}

  
void UART1_Send(unsigned char * cpBUFFER)
{    
	unsigned char i;	   						
	g_cCheckSum = 0;

	UART1_Senddata(0x02);	 
	
	for (i = 0; i < (cpBUFFER[2] + 1); i++)
	{
		if(( cpBUFFER[i] == 0x02 ) || ( cpBUFFER[i] == 0x03 ) || ( cpBUFFER[i] == 0x10))
		{
			UART1_Senddata(0x10);		
		}	
		UART1_Senddata(cpBUFFER[i]);	
			 	
		g_cCheckSum += cpBUFFER[i];
	} 
	if (( g_cCheckSum == 0x02 ) || ( g_cCheckSum == 0x03 ) || ( g_cCheckSum == 0x10))
	{
		UART1_Senddata(0x10);	 	
	} 
	UART1_Senddata(g_cCheckSum);	
	 	
	UART1_Senddata(0x03);	
	g_cReceiveCounter = 0;
	g_bReceiveOK = 0;
	g_b0x10Received = 0; 
	
}

unsigned char Uart_rfinit()			
{

	unsigned int cCnt;

	UART1_Send(COMM_CONTROL_ANTENNA_OFF);		   //关天线
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 3;
	}
/*	有问题，返回数据以21开头，数据不对
	UART1_Send(COMM_SET_TYPE_AB_OTHER_);	 //模式为s50卡
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 2;
	} 				*/
	
	return 0;
}		 

unsigned char Uart_rfread()			 //估算小于40ms
{

	unsigned int cCnt;

	UART1_Send(COMM_CONTROL_ANTENNA_ON);		   //开天线
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 1;
	}
 	UART1_Send(COMM_MIFARE1_PCD_REQUEST_);		   //寻卡
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 2;
	}

	UART1_Send(COMM_MIFARE1_PCD_ANTICOLL_);		  //仿冲突
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 3;
	}
	COMM_MIFARE1_PCD_SELECT_[4]=rbufrf[5];
	COMM_MIFARE1_PCD_SELECT_[5]=rbufrf[6];
	COMM_MIFARE1_PCD_SELECT_[6]=rbufrf[7];
	COMM_MIFARE1_PCD_SELECT_[7]=rbufrf[8];
	UART1_Send(COMM_MIFARE1_PCD_SELECT_);		  //选卡
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 4;
	}
	UART1_Send(COMM_AUTHENTICATION2_);		  //秘钥认证
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 5;
	}
	UART1_Send(COMM_PCD_READ_BLOCK_4);		  //读块4
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 6;
	}
	
	for (cCnt=0; cCnt < 10; cCnt++)  //10ms
	{
		data_rf[cCnt]=rbufrf[cCnt+5];
	}
	UART1_Send(COMM_PCD_READ_BLOCK_5);		  //读块5
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 6;
	}
	for (cCnt=10; cCnt < 20; cCnt++)  //10ms
	{
		data_rf[cCnt]=rbufrf[cCnt-5];
	}

	UART1_Send(COMM_CONTROL_ANTENNA_OFF);		   //关天线
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 8;
	}

	return 0;
}


unsigned char Uart_rfwrite(){


  unsigned int cCnt;

	UART1_Send(COMM_CONTROL_ANTENNA_ON);		   //开天线
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 1;
	}
 	UART1_Send(COMM_MIFARE1_PCD_REQUEST_);		   //寻卡
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 2;
	}

	UART1_Send(COMM_MIFARE1_PCD_ANTICOLL_);		  //仿冲突
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 3;
	}
	COMM_MIFARE1_PCD_SELECT_[4]=rbufrf[5];
	COMM_MIFARE1_PCD_SELECT_[5]=rbufrf[6];
	COMM_MIFARE1_PCD_SELECT_[6]=rbufrf[7];
	COMM_MIFARE1_PCD_SELECT_[7]=rbufrf[8];
	UART1_Send(COMM_MIFARE1_PCD_SELECT_);		  //选卡
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 4;
	}
	UART1_Send(COMM_AUTHENTICATION2_);		  //秘钥认证
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 5;
	}

	
	for (cCnt=5; cCnt < 15; cCnt++)  //10ms
	{
		
		COMM_PCD_WRITE_BLOCK_[cCnt]=data_rf[cCnt+5];
	}
	

	UART1_Send(COMM_PCD_WRITE_BLOCK_);		  //写块5
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 7;
	}

	UART1_Send(COMM_CONTROL_ANTENNA_OFF);		   //关天线
	for (cCnt=1000; ((cCnt > 0) && !g_bReceiveOK)>0; cCnt--)  //10ms
	{
		Delay(1);
	}
	if (((rbufrf[4])!=0) || (0 == cCnt)||(g_bReceiveOK==0))
	{
		return 8;
	}
  return 0;

}



void check_GX_init()
{
	
			change_picture(0x01);
		
		if (SGX==SGX_renegade)
		{
			Delay_100us();
			P4status=P4;
			if (SGX==SGX_renegade)
			{
				Delay_100us();
				P4status=P4;
				if (SGX==SGX_renegade)
				{

zice:			RF_init_qingchu();		
					RF_gxlianjieshow();           //未连接光纤
						while ( SGX==SGX_renegade )
						{/*SGX=0，光纤故障*/
							Alarm (sound3);
							Delay_1s();
							P4status=P4;
						}
				//	change_picture(0x02);
				//	LCDzhiliaojizijian();

				}
			}
		}
		
		if(Uart_rfinit())      //射频板故障或未连接 故障80
		{
		//	change_picture(0x01);
			RF_erroshow();
			while (1)
				{
						Alarm(sound3);
						Delay_5s();
				}
		}
    while(Uart_rfread()){  //光纤未带S50卡模块  或者密码错误，或者验证过程失效  请使用爱科光纤

	   		RF_gxshow();
	   		Alarm(sound3);
	   		Delay_1s();
		}
		
				//判断是否为复制卡等
		gx_weizhi_temp=100;
		while(gx_weizhi_temp){  //读取100条光纤序号存储记录，与本次读取序列号作比较
			
			read_data(0xa0,Address_gx_info_first+(--gx_weizhi_temp)*8,gx_info_read,8);
			Delay( 3000 );
			if(gx_info_read[0]==COMM_MIFARE1_PCD_SELECT_[4]){
				if(gx_info_read[1]==COMM_MIFARE1_PCD_SELECT_[5]){
					if(gx_info_read[2]==COMM_MIFARE1_PCD_SELECT_[6]){
						if(gx_info_read[3]==COMM_MIFARE1_PCD_SELECT_[7]){
							if((gx_info_read[6]==0x5a)&&(data_rf[10]>=gx_info_read[4])){   //上次数据写入未成功  在此写入
								data_rf[10]++;
								gxjishu1= data_rf[10];
								if(Uart_rfwrite()){      //写入不成功
										//LCD_InsertFiber();
								//	SysFault(90);
									RF_erroshow();
									while (1)
									{
										Alarm(sound3);
										Delay_5s();
									}
								}else{                  //成功写入，进一步读取验证存储是否正确
									Delay(10000);
									if(Uart_rfread()){
											//LCD_InsertFiber();
									//	SysFault(91);
										RF_erroshow();
											while (1)
										{
											Alarm(sound3);
											Delay_5s();
										}
									}else{
									 if(data_rf[10]!=gxjishu1){
										 //LCD_InsertFiber();
									//	SysFault(91);
										 RF_erroshow();
											while (1)
										{
											Alarm(sound3);
											Delay_5s();
										}
									 }
										gx_youxiao=1;                     //否则，光纤次数有可能已被篡改
									  gx_weizhi=gx_weizhi_temp;
									  gx_weizhi_temp=0; 
										gx_info_read[4]=data_rf[10];
										saveshuzu[0] = data_rf[10];
										write_data(0xa0,Address_gx_info_first+gx_weizhi*8+4,saveshuzu,1);
										Delay( 1000 );
										saveshuzu[0] = 0;
										write_data(0xa0,Address_gx_info_first+gx_weizhi*8+6,saveshuzu,1);
										Delay( 1000 );
									      
									}
								}
								
							}
							else{
							
								if(data_rf[10]>gx_info_read[4]){   //若序列号一致，则比较存储次数，若光线内部次数小于等于24c64中次数则有效
									gx_youxiao=1;                     //否则，光纤次数有可能已被篡改
									gx_weizhi=gx_weizhi_temp;
									gx_weizhi_temp=0;
									gx_info_read[4]=data_rf[10];
									saveshuzu[0] = data_rf[10];
									write_data(0xa0,Address_gx_info_first+gx_weizhi*8+4,saveshuzu,1);
									Delay( 1000 );
									
								}else if(data_rf[10]==gx_info_read[4]){
									gx_youxiao=1;                     //否则，光纤次数有可能已被篡改
									gx_weizhi=gx_weizhi_temp;
									gx_weizhi_temp=0;
								}else{
									RF_gxshow();
									while ( SGX==SGX_inserted ){
										Alarm(sound3);
										Delay_1s();
										P4status=P4;
									}
									goto zice;
								}
						  }
						}else{
							gx_youxiao=0;
						}
					}else{
						gx_youxiao=0;
					}
				}else{
					gx_youxiao=0;
				}
			}else{
				gx_youxiao=0;
			}
			
		}
		
		
			if(gx_youxiao==0){      //若本光纤未使用过，则为新光纤，判断光纤内部校验字是否和程序算出来的一致，若一致则为有效光纤
			gx_jiaoyan=(uchar)(COMM_MIFARE1_PCD_SELECT_[4]+9)^(uchar)(COMM_MIFARE1_PCD_SELECT_[5]>>2);
			gx_jiaoyan=gx_jiaoyan*((uchar)(COMM_MIFARE1_PCD_SELECT_[6]<<1)^(uchar)(~COMM_MIFARE1_PCD_SELECT_[7]));
			if((gx_jiaoyan%256)==data_rf[17]&&(gx_jiaoyan/256)==data_rf[16]){
						gx_info_read[0]=COMM_MIFARE1_PCD_SELECT_[4];
						gx_info_read[1]=COMM_MIFARE1_PCD_SELECT_[5];
						gx_info_read[2]=COMM_MIFARE1_PCD_SELECT_[6];
						gx_info_read[3]=COMM_MIFARE1_PCD_SELECT_[7];
						gx_info_read[4]=data_rf[10];
						if(data_rf[11]>10){  //光纤可用次数异常，需更换光纤
							RF_gxshow();
							while ( SGX==SGX_inserted ){
								Alarm(sound3);
								Delay_1s();
								P4status=P4;
							}
							goto zice;
						}
						gx_info_read[5]=data_rf[11];
						gx_info_read[6]=0;
						gx_info_read[7]=0;
					read_data(0xa0,Address_gx_info_weizhi,qushuzu,1);
					Delay( 1000 );
					gx_weizhi = qushuzu[0];
					if(gx_weizhi>99)
						gx_weizhi=0;						
					write_data(0xa0,Address_gx_info_first+gx_weizhi*8,gx_info_read,8);
					Delay( 1000 );
					gx_weizhi++;
					saveshuzu[0]=gx_weizhi;
					write_data(0xa0,Address_gx_info_weizhi,saveshuzu,1);
					Delay( 1000 );
					gx_weizhi--;
			
			}else{    //不一致则为复制光纤，需更换光纤
			
				RF_gxshow();
				while ( SGX==SGX_inserted ){
					Alarm(sound3);
					Delay_1s();
					P4status=P4;
				}
				goto zice;
		  }
		}
		
		RF_initshow();
			if(data_rf[10]>guangxian_shouming){
				GX_shixiaoshow();
				while(1){
							Alarm(sound3);
							Delay_1s();
						}
			}
		
// 		if((data_rf[10]>=10)){   //光纤次数耗尽    
// 			GX_shixiaoshow();
// 			Alarm(sound3);
// 			Delay_1s();	
// 			Alarm(sound3);
// 			Delay_1s();	
// 			Alarm(sound3);
// 			Delay_1s();	
// 			Alarm(sound3);
// 			if(data_rf[10]>guangxian_shouming){
// 				while(1){
// 							Alarm(sound3);
// 							Delay_1s();
// 						}
// 			}
// 			
// 		}
		
		if((lot_data[0]== COMM_MIFARE1_PCD_SELECT_[4])&&(lot_data[1]== COMM_MIFARE1_PCD_SELECT_[5])&&(lot_data[2]== COMM_MIFARE1_PCD_SELECT_[6])&&(lot_data[3]== COMM_MIFARE1_PCD_SELECT_[7])){
						    
		}else{
						    RF_writeflag=0;
								lot_data[0]= COMM_MIFARE1_PCD_SELECT_[4];
								lot_data[1]= COMM_MIFARE1_PCD_SELECT_[5];
								lot_data[2]= COMM_MIFARE1_PCD_SELECT_[6];
								lot_data[3]= COMM_MIFARE1_PCD_SELECT_[7];
		}	
		
		Delay_5s();
    change_picture(0x02);
		if(gx_ener_restrict==4&&SetFrequency>40){
			SetFrequency=10;		
			FlagPara = parachang;			
		}
		CalMaxEner_Ho();
		if (SetEnergy_left>maxLimitSetEnergy_Ho)
		{
			FlagPara = parachang;
			SetEnergy_left= maxLimitSetEnergy_Ho;
			Display_Energy_left( SetEnergy_left);
			Power = SetFrequency*SetEnergy_left;
			Display_power_left( Power );
		}
		if (SetEnergy_right>maxLimitSetEnergy_Ho)
		{
			FlagPara = parachang;
			SetEnergy_right = maxLimitSetEnergy_Ho;
			Display_Energy_right( SetEnergy_right);
			Power = SetFrequency*SetEnergy_right;
			Display_power_right( Power );
		}		
		LCDzhiliaojizijian();
	
	
	
}


void check_GX()
{
		
		if (SGX==SGX_renegade)
		{
			Delay_100us();
			P4status=P4;
			if (SGX==SGX_renegade)
			{
				Delay_100us();
				P4status=P4;
				if (SGX==SGX_renegade)
				{

				  if(Status==1){
					  change_picture(0x01);
zice_daiji:	RF_init_qingchu();		
					  RF_gxlianjieshow();           //未连接光纤
						while ( SGX==SGX_renegade )
						{/*SGX=0，光纤故障*/
							Alarm (sound3);
							Delay_1s();
							P4status=P4;
						}
		
					while(Uart_rfread()){  //光纤未带S50卡模块  或者密码错误，或者验证过程失效  请使用爱科光纤

							RF_gxshow();
							Alarm(sound3);
							Delay_1s();
					}
		
				//判断是否为复制卡等
		gx_weizhi_temp=100;
		while(gx_weizhi_temp){  //读取100条光纤序号存储记录，与本次读取序列号作比较
			
			read_data(0xa0,Address_gx_info_first+(--gx_weizhi_temp)*8,gx_info_read,8);
			Delay( 3000 );
			if(gx_info_read[0]==COMM_MIFARE1_PCD_SELECT_[4]){
				if(gx_info_read[1]==COMM_MIFARE1_PCD_SELECT_[5]){
					if(gx_info_read[2]==COMM_MIFARE1_PCD_SELECT_[6]){
						if(gx_info_read[3]==COMM_MIFARE1_PCD_SELECT_[7]){
							if((gx_info_read[6]==0x5a)&&(data_rf[10]>=gx_info_read[4])){   //上次数据写入未成功  在此写入
								data_rf[10]++;
								gxjishu1= data_rf[10];
								if(Uart_rfwrite()){      //写入不成功
										//LCD_InsertFiber();
								//	SysFault(90);
									RF_erroshow();
									while (1)
									{
										Alarm(sound3);
										Delay_5s();
									}
								}else{                  //成功写入，进一步读取验证存储是否正确
									Delay(10000);
									if(Uart_rfread()){
											//LCD_InsertFiber();
									//	SysFault(91);
										RF_erroshow();
											while (1)
										{
											Alarm(sound3);
											Delay_5s();
										}
									}else{
									 if(data_rf[10]!=gxjishu1){
										 //LCD_InsertFiber();
									//	SysFault(91);
										 RF_erroshow();
											while (1)
										{
											Alarm(sound3);
											Delay_5s();
										}
									 }
										gx_youxiao=1;                     //否则，光纤次数有可能已被篡改
									  gx_weizhi=gx_weizhi_temp;
									  gx_weizhi_temp=0; 
										gx_info_read[4]=data_rf[10];
										saveshuzu[0] = data_rf[10];
										write_data(0xa0,Address_gx_info_first+gx_weizhi*8+4,saveshuzu,1);
										Delay( 1000 );
										saveshuzu[0] = 0;
										write_data(0xa0,Address_gx_info_first+gx_weizhi*8+6,saveshuzu,1);
										Delay( 1000 );
									      
									}
								}
								
							}
							else{
							
								if(data_rf[10]>gx_info_read[4]){   //若序列号一致，则比较存储次数，若光线内部次数小于等于24c64中次数则有效
									gx_youxiao=1;                     //否则，光纤次数有可能已被篡改
									gx_weizhi=gx_weizhi_temp;
									gx_weizhi_temp=0;
									gx_info_read[4]=data_rf[10];
									saveshuzu[0] = data_rf[10];
									write_data(0xa0,Address_gx_info_first+gx_weizhi*8+4,saveshuzu,1);
									Delay( 1000 );
									
								}else if(data_rf[10]==gx_info_read[4]){
									gx_youxiao=1;                     //否则，光纤次数有可能已被篡改
									gx_weizhi=gx_weizhi_temp;
									gx_weizhi_temp=0;
								}else{
									RF_gxshow();
									while ( SGX==SGX_inserted ){
										Alarm(sound3);
										Delay_1s();
										P4status=P4;
									}
									goto zice_daiji;
								}
						  }
						}else{
							gx_youxiao=0;
						}
					}else{
						gx_youxiao=0;
					}
				}else{
					gx_youxiao=0;
				}
			}else{
				gx_youxiao=0;
			}
			
		}
		
		
			if(gx_youxiao==0){      //若本光纤未使用过，则为新光纤，判断光纤内部校验字是否和程序算出来的一致，若一致则为有效光纤
			gx_jiaoyan=(uchar)(COMM_MIFARE1_PCD_SELECT_[4]+9)^(uchar)(COMM_MIFARE1_PCD_SELECT_[5]>>2);
			gx_jiaoyan=gx_jiaoyan*((uchar)(COMM_MIFARE1_PCD_SELECT_[6]<<1)^(uchar)(~COMM_MIFARE1_PCD_SELECT_[7]));
			if((gx_jiaoyan%256)==data_rf[17]&&(gx_jiaoyan/256)==data_rf[16]){
						gx_info_read[0]=COMM_MIFARE1_PCD_SELECT_[4];
						gx_info_read[1]=COMM_MIFARE1_PCD_SELECT_[5];
						gx_info_read[2]=COMM_MIFARE1_PCD_SELECT_[6];
						gx_info_read[3]=COMM_MIFARE1_PCD_SELECT_[7];
						gx_info_read[4]=data_rf[10];
						if(data_rf[11]>10){  //光纤可用次数异常，需更换光纤
							RF_gxshow();
							while ( SGX==SGX_inserted ){
								Alarm(sound3);
								Delay_1s();
								P4status=P4;
							}
							goto zice_daiji;
						}
						gx_info_read[5]=data_rf[11];
						gx_info_read[6]=0;
						gx_info_read[7]=0;
					read_data(0xa0,Address_gx_info_weizhi,qushuzu,1);
					Delay( 1000 );
					gx_weizhi = qushuzu[0];
					if(gx_weizhi>99)
						gx_weizhi=0;						
					write_data(0xa0,Address_gx_info_first+gx_weizhi*8,gx_info_read,8);
					Delay( 1000 );
					gx_weizhi++;
					saveshuzu[0]=gx_weizhi;
					write_data(0xa0,Address_gx_info_weizhi,saveshuzu,1);
					Delay( 1000 );
					gx_weizhi--;
			
			}else{    //不一致则为复制光纤，需更换光纤
			
				RF_gxshow();
				while ( SGX==SGX_inserted ){
					Alarm(sound3);
					Delay_1s();
					P4status=P4;
				}
				goto zice_daiji;
		  }
		}
		
		RF_initshow();
			if(data_rf[10]>guangxian_shouming){
				GX_shixiaoshow();
				while(1){
							Alarm(sound3);
							Delay_1s();
						}
			}
		
// 		if((data_rf[10]>=10)){   //光纤次数耗尽    
// 			GX_shixiaoshow();
// 			Alarm(sound3);
// 			Delay_1s();	
// 			Alarm(sound3);
// 			Delay_1s();	
// 			Alarm(sound3);
// 			Delay_1s();	
// 			Alarm(sound3);
// 			if(data_rf[10]>guangxian_shouming){
// 				while(1){
// 							Alarm(sound3);
// 							Delay_1s();
// 						}
// 			}
// 			
// 		}
		
		if((lot_data[0]== COMM_MIFARE1_PCD_SELECT_[4])&&(lot_data[1]== COMM_MIFARE1_PCD_SELECT_[5])&&(lot_data[2]== COMM_MIFARE1_PCD_SELECT_[6])&&(lot_data[3]== COMM_MIFARE1_PCD_SELECT_[7])){
						    
		}else{
						    RF_writeflag=0;
								lot_data[0]= COMM_MIFARE1_PCD_SELECT_[4];
								lot_data[1]= COMM_MIFARE1_PCD_SELECT_[5];
								lot_data[2]= COMM_MIFARE1_PCD_SELECT_[6];
								lot_data[3]= COMM_MIFARE1_PCD_SELECT_[7];
		}	
		
		Delay_5s();
    change_picture(0x02);
		if(gx_ener_restrict==4&&SetFrequency>40){
			SetFrequency=10;		
			FlagPara = parachang;			
		}
		CalMaxEner_Ho();
		if (SetEnergy_left>maxLimitSetEnergy_Ho)
		{
			FlagPara = parachang;
			SetEnergy_left= maxLimitSetEnergy_Ho;
			Display_Energy_left( SetEnergy_left);
			Power = SetFrequency*SetEnergy_left;
			Display_power_left( Power );
		}
		if (SetEnergy_right>maxLimitSetEnergy_Ho)
		{
			FlagPara = parachang;
			SetEnergy_right = maxLimitSetEnergy_Ho;
			Display_Energy_right( SetEnergy_right);
			Power = SetFrequency*SetEnergy_right;
			Display_power_right( Power );
		}		
		fnShowdaiji();
		}else if(Status==2){				//准备或发射
			 LCD_InsertFiber();
			 halt();
			 CJDQ = JDQ_Close; //压缩机断电
			 while (1)
			{
				Alarm(sound3);
				Delay_5s();
			}
		}
		
	
				}
			}
		}
	
}


void check_GX_zhunbei()
{
	if(T_2s_flag){
			T_2s_flag=0;
			if(Uart_rfread()){				//连续3次读取异常 认为s50卡未在范围 ，或者密码错误，认证失败等，请使用爱科光纤
				RF_errojishu++;
				if(RF_errojishu>=3){
						LCD_InsertFiber();
							SysFault(92);
						while(1){
							Alarm(sound3);
							Delay_1s();
						}
				}
			}else{							//正常读取后判断次数
				RF_errojishu=0;
				if(gx_info_read[4]!=data_rf[10]){  //次数耗尽或者总次数异常
					RF_errojishu3++;
					if(RF_errojishu3>=3){
						LCD_InsertFiber();
							SysFault(93);
						while(1){
							Alarm(sound3);
							Delay_1s();
						}
					}
				}else{
					RF_errojishu3=0;
				}
				//序列号不一致
				if((lot_data[0]== COMM_MIFARE1_PCD_SELECT_[4])&&(lot_data[1]== COMM_MIFARE1_PCD_SELECT_[5])&&(lot_data[2]== COMM_MIFARE1_PCD_SELECT_[6])&&(lot_data[3]== COMM_MIFARE1_PCD_SELECT_[7])){
					 RF_errojishu4=0;	
				}else{
					RF_errojishu4++;
					if(RF_errojishu4>=3){
						LCD_InsertFiber();
							SysFault(94);
						while(1){
							Alarm(sound3);
							Delay_1s();
						}
					}
					
				}
			}
	}
			
	
}

void GX_write()
{
	
	if(Totalminutes_benci>=3){
				if(RF_writeflag==0){
				RF_writeflag=1;
				data_rf[10]++;
				gxjishu1= data_rf[10];
				if(Uart_rfwrite()){
					data_rf[10]--;
					RF_writeflag=0;
						RF_errojishu2++;
// 						if(RF_errojishu2>=3){
// 							//LCD_InsertFiber();
// 							SysFault(90);
// 							while (1)
// 						{
// 							Alarm(sound3);
// 							Delay_5s();
// 						}
// 						}
					saveshuzu[0] = 0x5a;
					write_data(0xa0,Address_gx_info_first+gx_weizhi*8+6,saveshuzu,1);
					Delay( 1000 );
				}else{
					gx_info_read[4]=data_rf[10];
					saveshuzu[0] = data_rf[10];
					write_data(0xa0,Address_gx_info_first+gx_weizhi*8+4,saveshuzu,1);
					Delay( 1000 );
					saveshuzu[0] = 0;
					write_data(0xa0,Address_gx_info_first+gx_weizhi*8+6,saveshuzu,1);
					Delay( 1000 );
					
					 RF_errojishu2=0;
				}
				}

		}
	
}




void Pwm_LD_init(void)
{  
	  PCA0MD   = 0x08;   //系统时钟，禁止溢出中断
    PCA0CPM1 = 0xc2;   //PCA1 16位pwm模式
    PCA0CPL1 = 0;	     //占空比为0%
	  PCA0CPH1 = 0;    
	  PCA0CN   = 0x40;  //启动pca定时器
	  pwm_ld_data = 0;
}

void Pwm_LD(uint cc)
{  
    PCA0CPH1 = cc / 256;
	  PCA0CPL1 = cc % 256;	
	  PCA0CPM1 = 0xc2;     //PCA1 16位pwm模式
}            
/*
void Pwm_FS_init(void)
{  
	PCA0MD=0x08;//系统时钟，禁止溢出中断
    PCA0CPM2=0xc2;//PCA2 16位pwm模式
	PCA0CPM3=0xc2;//PCA2  16位pwm模式
	PCA0CPM4=0xc2;//PCA2  16位pwm模式
    PCA0CPL2=0;	  // 因为电路中有反向驱动  故此时占空比为0%
	PCA0CPH2=0;
	PCA0CPL3=0;
	PCA0CPH3=0;
	PCA0CPL4=0;
	PCA0CPH4=0;
	PCA0CN= 0x40;//启动pca定时器
}

void Pwm_FS(uint cc)
{  
    PCA0CPL2=cc%256;
	PCA0CPH2=cc/256;
	PCA0CPL3=cc%256;	
	PCA0CPH3=cc/256;
	PCA0CPL4=cc%256;
	PCA0CPH4=cc/256;
	PCA0CPM2=0xc2;//PCA2 16位pwm模式
	PCA0CPM3=0xc2;//PCA2  16位pwm模式
	PCA0CPM4=0xc2;//PCA2  16位pwm模式	

}
				  */
















