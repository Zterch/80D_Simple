

#include "include\includes.h"


/*初始化子程序*/
#if 1
void Initialize ( void )
{

	  /*初始化*/
    Reset_Sources_Init();	   // 禁看门狗
    Port_IO_Init();          // 端口 + 交叉开关
	  Oscillator_Init();	     // 外部 22.1184MHz 晶振
	  Timer0Init();            // 50us 定时
	  Timer3Init();            // 10ms 定时
	  ADC0_Init();
	  DAC0_Init();
	  init_smbus();	           // SMBus 给 AT24C512
	  UART0_Init();            // 调试/上位机
	  UART1_Init();            // RFID 模块
	  Pwm_LD_init();           // 指示光 PWM
	  EA = 1;	
	  Delay_1s();	
	  Delay_1s();	
	  	           		         
	  saveshuzu[0] = 0;                //先写一次 否则直接读EEPROM 会有问题
	  write_data(0xa0, 0, saveshuzu, 1);	
    Delay_1s();
	
		/*8051 的 P5 端口不能位寻址（P5 地址 0x85 不是 8 的倍数，
		 *只有地址是 8 的倍数的 SFR 才能位寻址，比如 P0=0x80、P1=0x90、P2=0xA0、P3=0xB0）。
		 *先读整个字节到 RAM 变量，再用 sbit 访问它的位：*/
	  P5status = P5;
		
	  if (KAIHUAN == 1)      //判断是否进入开环状态
	  {		
	  	  change_picture(0x07);
	  	  test1_6function();			
	  }
	  
    change_picture(0x02);
	  
	  LCDzhiliaojizijian();
	  
	  
	  /*初始化工作参数*/
	  Initialize_5Parameter();
		
	  /*设置某些参数*/
	  Initialize_Other_Parameter();
	  
    
	  /*开机自检*/
    //吸合压缩机
	  CJDQ = JDQ_Open;
	  Delay_1s();
	  CA = 0;
    CB = 1;	  /*取单路沿,上升沿*/	
	  	
	  //预燃监测
	  fnYuRan_Off();
	  fnYuRan_On();
		
	  //光闸检测		
	  fnguangzha_on();
	  fnguangzha_off();
	  Delay_1s();
		
    //启动步进电机
	  fnSlowStartupDrive();   //启动到2.5HZ
	  fnDianJiCheckAdjust(5);
	  
	  
	  CheckPowerOn();
	  fnGenJuShangCiPinLvTiaoZheng(SetFrequency);
 	  fnDianJiCheckAdjust(SetFrequency);
	  pwm_ld_data = 20000;	//指示灯亮度为40%
	  Pwm_LD(pwm_ld_data);
    //Alarm(sound2);
  	return;
}


void Reset_Sources_Init()
{
    WDTCN     = 0xDE;
    WDTCN     = 0xAD;    //禁止看门狗
}

void Port_IO_Init()
{
    P1MDOUT   = 0x00;   // P1为开漏输出
		P2MDOUT   = 0x00;   // P2为开漏输出	 
		P3MDOUT   = 0x00;   // P3为开漏输出
    // P0.0  -  TX0 (UART0), Open-Drain, Digital
    // P0.1  -  RX0 (UART0), Open-Drain, Digital
    // P0.2  -  SDA (SMBus), Open-Drain, Digital
    // P0.3  -  SCL (SMBus), Open-Drain, Digital
    // P0.4  -  TX1 (UART1), Open-Drain, Digital
    // P0.5  -  RX1 (UART1), Open-Drain, Digital
    // P0.6  -  CEX0 (PCA),  Open-Drain, Digital
    // P0.7  -  CEX1 (PCA),  Open-Drain, Digital

    // P1.0  -  CEX2 (PCA),  Open-Drain, Digital
    // P1.1  -  T2EX (Tmr2), Open-Drain, Digital
    XBR0      = 0x1D;
    XBR1      = 0x40;
    XBR2      = 0x44;

		P74OUT    = 0x00;	//P4/P5/P6为数字输入方式
		P4        = 0xff;
		P5        = 0xff;
		P6        = 0xff;
		P7        = 0xff;
	
		P1   = 0x00;	  //  初始化控制端口
		P2   = 0x0F;	  // CLAMP=0  CJDQ=1 CWGZ=1 FSFMQ=1 CBJ=1 
		P3   = 0xF0;    // CA/CB=0 PULSE=0  CCDFD=0 	  CNL=1 CYR_A/CYR_B=1 2302HJ=1

}

//22.1184M
void Oscillator_Init()
{
    int i = 0;
    OSCXCN    = 0x67;            //选用外部晶体振荡器 频率大于6.7M
    for (i = 0; i < 3000; i++);  // Wait 1ms for initialization
    while ((OSCXCN & 0x80) == 0);//XTLVLD 等待晶体振荡器有效标志位 置位ckcon
    OSCICN    = 0x88;            //允许检测时钟丢失 选择外部晶振 
}

void Timer0Init( void )
{
	TMOD   |= 0x01; //16位定时模式 	                
	TL0 = (65536-92)%256;     //系统时钟12分频 作为定时器2时钟源	           
	TH0 = (65536-92)/256;     //精度可以满足   此定时为50us，准确数为49.945us	  精度1%    
	ET0=1;
}

void Timer3Init( void )
{ 
    TMR3CN    = 0x04;		                //定时器3启动
    TMR3RLL   = (65536-18400)%256;		  //系统时钟12分频 作为定时器3时钟源
    TMR3RLH   = (65536-18400)/256;		  //定时10ms
	  EIE2     |= 0x01;		                //允许定时器3中断
}

 /*  //初始化中断优先级、使能标志
void fnInterruptTimer ( void )
{
	
      //IP=0x02;
}	  */

void Initialize_5Parameter ( void )
{
	uchar temp;

	/*读取频率*/
  read_data(0xa0,AddressFreq_left,qushuzu,1);
  Delay( 1000 );
	temp = qushuzu[0];
  if((temp>=minFreq)&&(temp<=maxFreq))
  {
	  SetFrequency = temp;
		SetFrequency_only = temp;
  }
  else
  {
    SetFrequency = LimitFrequency;
		SetFrequency_only = LimitFrequency;
  }
  SetFrequency_last=5;
	
	read_data(0xa0,AddressFreq_left_double,qushuzu,1);
  Delay( 1000 );
	temp = qushuzu[0];
  if((temp>=3)&&(temp<=30))
  {
		SetFrequency_double = temp;
  }
  else
  {
		SetFrequency_double = 3;
  }

	/*读取能量*/
 	read_data(0xa0,AddressEner_left,qushuzu,1);
	Delay( 1000 );
	temp = qushuzu[0];
	CalMaxEner_Ho();
	if ( temp>maxLimitSetEnergy_Ho)
	{
		SetEnergy_left = minEner;
		SetEnergy_left_only = minEner;
	}
	else
	{
		if((temp>=minEner)&&(temp<=maxEner))
	  {
	       	/*此处未限制能量*/
	       SetEnergy_left = temp;
			SetEnergy_left_only = temp;
		}
	  else
	  {
			SetEnergy_left = minEner;
			SetEnergy_left_only = minEner;
	  }
	}

 	read_data(0xa0,AddressEner_left_double,qushuzu,1);
	Delay( 1000 );
	temp = qushuzu[0];
	CalMaxEner_Ho();
	if ( temp>maxLimitSetEnergy_Ho)
	{
		SetEnergy_left_double = minEner;
	}
	else
	{
		if((temp>=minEner)&&(temp<=maxEner))
	  {

			SetEnergy_left_double = temp;
		}
	  else
	  {
			SetEnergy_left_double = minEner;
	  }
	}
	
	read_data(0xa0,AddressEner_right,qushuzu,1);
	Delay( 1000 );
	temp = qushuzu[0];
	CalMaxEner_Ho();
	if ( temp>maxLimitSetEnergy_Ho)
	{
		SetEnergy_right = minEner;
		SetEnergy_right_only = minEner;
	}
	else
	{
		if((temp>=minEner)&&(temp<=maxEner))
	  {
	       	/*此处未限制能量*/
	       SetEnergy_right = temp;
			SetEnergy_right_only = temp;
		}
	  else
	  {
			SetEnergy_right = minEner;
			SetEnergy_right_only = minEner;
	  }
	}

 	read_data(0xa0,AddressEner_right_double,qushuzu,1);
	Delay( 1000 );
	temp = qushuzu[0];
	CalMaxEner_Ho();
	if ( temp>maxLimitSetEnergy_Ho)
	{
		SetEnergy_right_double = minEner;
	}
	else
	{
		if((temp>=minEner)&&(temp<=maxEner))
	  {

			SetEnergy_right_double = temp;
		}
	  else
	  {
			SetEnergy_right_double = minEner;
	  }
	}


		/*读取总脉冲，用户型*/
	TotalPulse_U = 0;
		read_data(0xa0,Address_TP_U_HH,qushuzu,1);
	Delay( 1000 );
	TotalPulse_U = (ulong)qushuzu[0]*16777216 + TotalPulse_U;//ys+
	read_data(0xa0,Address_TP_U_HH+1,qushuzu,1);
	Delay( 1000 );
	TotalPulse_U = (ulong)qushuzu[0]*65536 + TotalPulse_U;
	read_data(0xa0,Address_TP_U_HH+2,qushuzu,1);
	Delay( 1000 );
	TotalPulse_U = (ulong)qushuzu[0]*256 + TotalPulse_U;
  read_data(0xa0,Address_TP_U_HH+3,qushuzu,1);
  Delay( 1000 );
	TotalPulse_U = (ulong)qushuzu[0] + TotalPulse_U;
       if(TotalPulse_U>=TP_U_AutoClear)
	{
		TotalPulse_U=0;
	}

	/*读取总能量，用户型*/
	TotalJoule_U= 0;
	 read_data(0xa0,Address_TJ_U_HH,qushuzu,1);
  Delay( 1000 );
	TotalJoule_U = (ulong)qushuzu[0]*16777216 + TotalJoule_U;//ys+
  read_data(0xa0,Address_TJ_U_HH+1,qushuzu,1);
  Delay( 1000 );
	TotalJoule_U = (ulong)qushuzu[0]*65536 + TotalJoule_U;
	read_data(0xa0,Address_TJ_U_HH+2,qushuzu,1);
	Delay( 1000 );
	TotalJoule_U = (ulong)qushuzu[0]*256 + TotalJoule_U;
	read_data(0xa0,Address_TJ_U_HH+3,qushuzu,1);
	Delay( 1000 );
	TotalJoule_U = (ulong)qushuzu[0] + TotalJoule_U;
       if (TotalPulse_U==0)
	{
		TotalJoule_U=0;
	}


	//读取工作信息存储总数
	read_data(0xa0,Address_work_info_state_jishu,qushuzu,1);
	Delay( 1000 );
	work_info_jishu = qushuzu[0];

	//读取工作信息存储位置
	read_data(0xa0,Address_work_info_state_weizhi,qushuzu,1);
	Delay( 1000 );
	work_info_weizhishu = qushuzu[0];
	work_info_weizhi=Address_work_info_first+work_info_weizhishu*32;

	//读取上一次工作信息
	read_data(0xa0,work_info_weizhi,work_info_read,15);
	Delay( 1000 );
	if((work_info_read[6]|work_info_read[7]|work_info_read[8])!=0){	 //上一次是否有激光工作，若有，上次信息有效
	 if((work_info_read[9]|work_info_read[10]|work_info_read[11])!=0){
	   //位置数增加
	   work_info_weizhishu++;
	   if(work_info_weizhishu>=100){
		  work_info_weizhishu=0;
	   }
	   work_info_weizhi=Address_work_info_first+work_info_weizhishu*32;
	   saveshuzu[0]=work_info_weizhishu;
	   write_data(0xa0,Address_work_info_state_weizhi,saveshuzu,1);
	   Delay( 1000 );
	   
	   //记录总数加1
	   work_info_jishu++;
	   if(work_info_jishu>=100){
		  work_info_jishu=100;
	   }
	   saveshuzu[0]=work_info_jishu;
	   write_data(0xa0,Address_work_info_state_jishu,saveshuzu,1);
	   Delay( 1000 );
		//将上次工作信息累加到 总脉冲 总能量
		TotalPulse_U=TotalPulse_U+work_info_read[6]*65536+work_info_read[7]*256+work_info_read[8];
		TotalJoule_U=TotalJoule_U+work_info_read[9]*65536+work_info_read[10]*256+work_info_read[11];

	   	work_info_write[0]=TotalPulse_U/16777216;
		work_info_write[1]=TotalPulse_U%16777216/65536;
		work_info_write[2]=TotalPulse_U%65536/256;								
		work_info_write[3]=TotalPulse_U%256;

		work_info_write[4]=TotalJoule_U/16777216;
		work_info_write[5]=TotalJoule_U%16777216/65536;								
		work_info_write[6]=TotalJoule_U%65536/256;
		work_info_write[7]=TotalJoule_U%256;
		write_data(0xa0,Address_TP_U_HH,work_info_write,8);
		Delay( 2000 );
	   }
	}

	//读取本次开机时间且存入24c64
	real_time_flag=0;
	read_realtime();
	while(!real_time_flag){

	}
	if((rbuf[11]==0&&rbuf[12]==0)||rbuf[6]<0x19||rbuf[6]>0x50){		 //若分针秒针同时为0 || 小于2019年 || 大于2050年 重新读取时间
	   Delay_1s();
	   Delay_1s();
	   read_realtime();
	   while(!real_time_flag){

		}
	   if((rbuf[11]==0&&rbuf[12]==0)||rbuf[6]<0x19||rbuf[6]>0x50){
	//	  LCD_jiaozhengtime();				  //显示校正时间

	   }
	}

	//将时间存入本次位置中，且将工作信息清零
	work_info_write[0]=rbuf[6];
	work_info_write[1]=rbuf[7];
	work_info_write[2]=rbuf[8];
	work_info_write[3]=rbuf[10];
	work_info_write[4]=rbuf[11];
	work_info_write[5]=rbuf[12];
	work_info_write[6]=0;
	work_info_write[7]=0;
	work_info_write[8]=0;
	work_info_write[9]=0;
	work_info_write[10]=0;
	work_info_write[11]=0;
	work_info_write[12]=0;
	work_info_write[13]=0;
	work_info_write[14]=0;
	write_data(0xa0,work_info_weizhi,work_info_write,15);
	Delay( 2000 );
//	read_data(0xa0,work_info_weizhi,work_info_read,6);
//	Delay( 1000 );

}

void Initialize_Other_Parameter ( void )
{
	
	/*读取低频灯选标志*/
	read_data(0xa0,Address_ucChoiceABlight,qushuzu,1);
	Delay( 1000 );
	ucChoiceABlight = qushuzu[0];

	/*清除充电电压*/
	fnDischangVolt_OutPut_uiA( 0x0000 );
	fnDischangVolt_OutPut_uiB( 0x0000 );


	/*读取信号屏蔽字节*/
	read_data(0xa0,Address_ucMaskByte,qushuzu,1);
	Delay( 1000 );
	ucMaskByte = qushuzu[0];

	if (MB_SGX==Check_Disable)	  //只能屏蔽光纤50次
	{
			//读取屏蔽光纤次数
		  read_data(0xa0,Address_MB_SGX_jishu,qushuzu,1);
			Delay( 1000 );
			MB_SGX_jishu = qushuzu[0];
			if(MB_SGX_jishu==0xff){  //初始化若数据等于255，计数清零
				MB_SGX_jishu=0;
			}
			if(MB_SGX_jishu>250){   //大于250次禁用
				SysFault(95);
				while(1){
							Alarm(sound3);
							Delay_1s();
				}
			}else{
				MB_SGX_jishu++;
				saveshuzu[0]=MB_SGX_jishu;
				write_data(0xa0,Address_MB_SGX_jishu,saveshuzu,1);
				Delay( 1000 );
			}
			
	}

	/*读取显示能量反馈值、充电电压标志*/
	read_data(0xa0,Address_ShowFeedBackVoltage,qushuzu,1);
	Delay( 1000 );
	Flag_ShowFeedBack = (bit)qushuzu[0];
	
	
			//读取光纤最大寿命
	read_data(0xa0,Address_guangxian_shouming,qushuzu,1);
	Delay( 1000 );
	guangxian_shouming = qushuzu[0];
	if(guangxian_shouming>100||guangxian_shouming<10){
			guangxian_shouming=50;
			saveshuzu[0]=guangxian_shouming;
			write_data(0xa0,Address_guangxian_shouming,saveshuzu,1);
			Delay( 1000 );
	}
	
		//读取保养计数
	read_data(0xa0,Address_baoyang_jishu,qushuzu,1);
	Delay( 1000 );
	baoyang_jishu = qushuzu[0];
	baoyang_jishu = baoyang_jishu<<8;
	read_data(0xa0,Address_baoyang_jishu+1,qushuzu,1);
	Delay( 1000 );
	baoyang_jishu += qushuzu[0];
	//读取保养参考
	read_data(0xa0,Address_baoyang_cankao,qushuzu,1);
	Delay( 1000 );
	baoyang_cankao = qushuzu[0];
	baoyang_cankao = baoyang_cankao<<8;
	read_data(0xa0,Address_baoyang_cankao+1,qushuzu,1);
	Delay( 1000 );
	baoyang_cankao += qushuzu[0];	
	if(baoyang_jishu>=baoyang_cankao){
			
		while(1)
		{
			  BY_tishi();	
			  if (rinterrupt==rinterrupt_Yes)
			  {  
			  	  rinterrupt = rinterrupt_No;
			  	  switch ( rbuf[8] )
			  	  {
			  	  			case 0x0c://待机
			  	  			{
			  	  				
			  	  				goto check2;
			  	  			}
			  	  			break;
			  	  			default:
			  	  			break;
			  	  }
			  }
		}
					
		check2:	LCDzhiliaojizijian();
				
	}
	
	return;
}

void fnSelectTimer2_Fun2_first (void)
{
	biTestDianji = TestDianji;
	biChoiceT2Fun = T2Fun2;
	uiT2OverFlows = 0x00;
	T2CON = 0x09;//t1做uart波特率发生器，捕捉模式，t2ex有效
	ET2 = 1;
	TR2 = 1;
	return;
}
			 

void fnYuRan_On (void)
{
  uchar i;
  CYR_A=YR_Open;								//第一次预燃
   	 CYR_B=YR_Open;//执行预燃命令
  Delay_1s();					   
  Delay_1s();
  if (MB_SYR_AB==Check_Enable)
  {       
	    for (i=0;i<50;i++)
	    {
		      //读取状态信号
		      Delay(10);
		      Delay(10);
		      Delay(10);
		      Delay(10);
		      
		      P6status=P6;
		      if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		      {
		   	      Delay(10);
		   	      P6status=P6;
		   	      if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		   	      {
		   	          Delay(10);
		   	          P6status=P6;
		   	          if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		   	          {
                      //Alarm(sound2);
		   	              return;
		   	          }
		   	      }
		      }
	    }
			
	    if ( SYR_A==SYR_cancel && SYR_B==SYR_cancel )	   //第二次预燃
	    {
		      CYR_A=YR_Close;
		      CYR_B=YR_Close;//撤销预燃命令
		      Delay (50000);
		      CYR_A=YR_Open;
		      CYR_B=YR_Open;//执行预燃命令
	    }
	    else if (SYR_A==SYR_cancel )
	    {
		      CYR_A=YR_Close;
		      Delay (50000);
		      CYR_A=YR_Open;
	    }
	    else if (SYR_B==SYR_cancel )
	    {
		      CYR_B=YR_Close;//撤销预燃命令
		      Delay (50000);
		      CYR_B=YR_Open;//执行预燃命令
	    }
	    
	    Delay_1s();
	    Delay_1s();
	    for (i=0;i<50;i++)
	    {
		      //读取状态信号
		      Delay(10);
		      Delay(10);
		      Delay(10);
		      Delay(10);
		      
		      P6status=P6;
		      if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		      {
		          Delay(10);
		          
		          P6status=P6;
		          if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		          {
		      	      Delay(10);
		      	      
		      	      P6status=P6;
		      	      if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		      	      {
			    	          //Alarm(sound2);
		      	   	      return;
		      	      }
		          }   
		      }
	    }
	    
	    
	    if ( SYR_A==SYR_cancel && SYR_B==SYR_cancel )	   //第三次预燃
	    {
					CYR_A=YR_Close;
					CYR_B=YR_Close;//撤销预燃命令
					Delay (50000);
					CYR_A=YR_Open;
					CYR_B=YR_Open;//执行预燃命令
	    }
	    else if (SYR_A==SYR_cancel )
	    {
					CYR_A=YR_Close;
					Delay (50000);
					CYR_A=YR_Open;
	    }
	    else if (SYR_B==SYR_cancel )
	    {
					CYR_B=YR_Close;//撤销预燃命令
					Delay (50000);
					CYR_B=YR_Open;//执行预燃命令
	    }
	    
	    Delay_1s();
	    Delay_1s();
	    for (i=0;i<50;i++)
	    {
		      //读取状态信号
		      Delay(10);
		      Delay(10);
		      Delay(10);
		      Delay(10);

		      P6status=P6;
		      if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		      {
		   	      Delay(10);
		   	      
		   	      P6status=P6;
		   	      if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		   	      {
		   	  	      Delay(10);
		   	  	      
		   	  	      P6status=P6;
		   	  	      if (SYR_A==SYR_keep && SYR_B==SYR_keep)
		   	  	      {
									    //Alarm(sound2);
		   	  	          return;
		   	  	      }
		   	      }
		      }   
	    }			 					 
	    
	    Delay_1s();
	    P6status = P6;
	    if(SYR_A==SYR_cancel || SYR_B==SYR_cancel)
			{		
	        Delay_1s();
	        P6status=P6;
	        if(SYR_A==SYR_cancel || SYR_B==SYR_cancel){
					    
		          if ( SYR_A==SYR_cancel && SYR_B==SYR_cancel )
		          {
		            SysFault(33);
		          }
		          else if (SYR_A==SYR_cancel )
		          {
		            SysFault(31);
		          }
		          else if (SYR_B==SYR_cancel )
		          {
		            SysFault(32);
		          }
					    
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



void fnYuRan_Off (void)
{
	uchar i;
	CYR_A = YR_Close;
  CYR_B = YR_Close;/*撤销预燃命令*/
	Delay_1s();
	if (MB_SYR_AB == Check_Enable)
	{
		
		for (i=0;i<100;i++)
		{
			/*读取状态信号*/
			P6status=P6;
			if ( SYR_A==SYR_cancel && SYR_B==SYR_cancel )
			{
				return;
			}
			Delay(10);
		}
		
		if ( SYR_A==SYR_keep && SYR_B==SYR_keep )
		{
			SysFault(36);
		}
		else if (SYR_A==SYR_keep )
		{
			SysFault(34);
		}
		else if (SYR_B==SYR_keep )
		{
			SysFault(35);
		}
		halt();
		CJDQ = JDQ_Close; //压缩机断电
		while (1)
		{
			Alarm(sound3);
			Delay_5s();
		}
	}
}

void fnguangzha_on(void){
	
	CWGZ = GZ_Open;	 
	Delay_1s();
  if (MB_SGZ_ABW == Check_Enable)
	{
	  P6status=P6;
		if (SWGZ0==SWGZ_close)
		{
			Delay(10);
			P6status=P6;
			if (SWGZ0==SWGZ_close)
			{
				Delay(10);
				P6status=P6;
				if (SWGZ0==SWGZ_close)
				{
					halt();
					CJDQ = JDQ_Close; //压缩机断电
					SysFault(53);
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

void fnguangzha_off(void){
	
	CWGZ=GZ_Close;	
	Delay_1s(); 
  if (MB_SGZ_ABW==Check_Enable)
	{
    
    
   		P6status=P6;
		if (SWGZ0==SWGZ_open)
		{
			Delay(10);
			P6status=P6;
			if (SWGZ0==SWGZ_open)
			{
				Delay(10);
				P6status=P6;
				if (SWGZ0==SWGZ_open)
				{
					halt();
					CJDQ = JDQ_Close; //压缩机断电
					SysFault(53);
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


#endif












