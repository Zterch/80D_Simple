#include "include\includes.h"
#include "include\main2.h"

/*
信号与状态

状态信号	成功（单片机输入端）	失败（单片机输入端）	
SLQSL	1	0	
SJT2	0	1	踩下与未踩
SJT1	1	0	
SLS	1	0	
SYM	1	0	
SGX	1	0	
SDY-A	0	1	
SDY-B	0	1	
SYR-A	1	0	
SYR-B	1	0	
SWGZ	1	0	打开与关闭
SWGZ0	0	1	
SZGZ-A	1	0	
SZGZ0-A	0	1	
SZGZ-B	1	0	
SZGZ0-B	0	1	
控制信号			
CYR-A	0	1	预燃与撤销
CYR-B	0	1	
CWGZ	0	1	打开与关闭
CZGZ-A	0	1	
CZGZ-B	0	1	
STY	1	0	正常与警报
STG	1	0	
PULSE	1→0→1	1	触发与不
CCDFD	0	1	未踩下与踩下
PLAYE	1→0→1	1	触发与不
CA、CB	定位A时CA=0 CB=1	定位B时CA=1 CB=0	
ISD-A4~6	语音地址选择位
指示光恒亮
ONF=0	指示光闪
ONF=1  FLS=1	指示光灭
ONF=1  FLS=0	


*/

uchar DaiJi (void);
uchar ZhunBei (void);
uchar Database (void);
uchar Information (void);

void main(void){
	
	Status = 1;                      // 系统初始时为待机状态
	Initialize();										 // 初始化硬件外设

	Pulse_time_left = 6;
  Pulse_time_right = 6;
  Pulse_time2 = 6;
  long_short_pluse_flag = 0;
	
	TotalJoule_benci = 0;
	TotalJoule_benci_1  = 0;
  TotalPulse_benci = 0;
	Totalminutes_benci = 0;


		while (1)
		{
			
		    switch(Status){
		    	case 1: 
		    		Status = DaiJi();		  
		    	  break;
		    	case 2: 
		    		Status = ZhunBei();
		    	  break;
			  case 3: 
		    		Status = Database();
		    	  break;
			  case 4: 
		    		Status = Information();
		    	  break;
		    	default:
			      Status = 1;
		        break;
		    }
				
		}
}


uchar Database (void)
{ 
    uchar pinlv_temp,nengliang_left_temp,nengliang_right_temp; 
    fnShowwork_database( );
    pinlv_temp=tuijian_database[0][0]; 
	nengliang_left_temp=tuijian_database[0][1];
	nengliang_right_temp=tuijian_database[0][2];
	show_database_qiehuan (0);
  while (1)
  { 
   if (rinterrupt==rinterrupt_Yes)
		{  
			rinterrupt = rinterrupt_No;
		 switch ( rbuf[8] )
		 {
			   
				case 0x30:
				{				
					change_picture(0x02);
					return 1;
				}
				break;
				case 0x31:
				{				
				  FlagPara = parachang; 
					SetFrequency=pinlv_temp;
				   SetEnergy_left=nengliang_left_temp;
				   SetEnergy_right=nengliang_right_temp;
				   fnGenJuShangCiPinLvTiaoZheng(SetFrequency);//调整电机转速
				   fnDianJiCheckAdjust( SetFrequency ); //电机转速微调，测量周期
				   change_picture(0x02);
				   return 1;
				}
				break;
				case 0x32:
				{				
				   pinlv_temp=tuijian_database[rbuf[8]-50][0]; 
				   nengliang_left_temp=tuijian_database[rbuf[8]-50][1];
				   nengliang_right_temp=tuijian_database[rbuf[8]-50][2];
				   show_database_qiehuan (rbuf[8]-50);
				}
				break;
				case 0x33:
				{				
				   pinlv_temp=tuijian_database[rbuf[8]-50][0]; 
				   nengliang_left_temp=tuijian_database[rbuf[8]-50][1];
				   nengliang_right_temp=tuijian_database[rbuf[8]-50][2];
				   show_database_qiehuan (rbuf[8]-50);
				}
				break;
				case 0x34:
				{				
				  pinlv_temp=tuijian_database[rbuf[8]-50][0]; 
				   nengliang_left_temp=tuijian_database[rbuf[8]-50][1];
				   nengliang_right_temp=tuijian_database[rbuf[8]-50][2];
				   show_database_qiehuan (rbuf[8]-50); 
				}
				break;
				case 0x35:
				{				
				  pinlv_temp=tuijian_database[rbuf[8]-50][0]; 
				   nengliang_left_temp=tuijian_database[rbuf[8]-50][1];
				   nengliang_right_temp=tuijian_database[rbuf[8]-50][2];
				   show_database_qiehuan (rbuf[8]-50);
				}
				break;
				case 0x36:
				{				
				  pinlv_temp=tuijian_database[rbuf[8]-50][0]; 
				   nengliang_left_temp=tuijian_database[rbuf[8]-50][1];
				   nengliang_right_temp=tuijian_database[rbuf[8]-50][2];
				   show_database_qiehuan (rbuf[8]-50); 
				}
				break;
				case 0x37:
				{				
				  pinlv_temp=tuijian_database[rbuf[8]-50][0]; 
				   nengliang_left_temp=tuijian_database[rbuf[8]-50][1];
				   nengliang_right_temp=tuijian_database[rbuf[8]-50][2];
				   show_database_qiehuan (rbuf[8]-50);
				}
				break;
		 }
	}
  }
}


uchar Information (void)
{ 
  uchar work_info_jishu_temp,weizhi_temp1;
  uint weizhi_temp2;

  RH_HJ();
  fnShowwork_RH_HJ();

  fnShowwork_info_benci();

  work_info_jishu_temp=work_info_jishu;
  if(work_info_jishu_temp>10){
	if(work_info_weizhishu<10)
	{
	   weizhi_temp1=10-work_info_weizhishu;
	   work_info_jishu_temp=10;
	   while (work_info_jishu_temp){
	     
	  	 if(weizhi_temp1>0){
		   weizhi_temp2= Address_work_info_state_jishu-weizhi_temp1*32;
		   weizhi_temp1--;
		 }else{
		   weizhi_temp2=work_info_weizhi-work_info_jishu_temp*32;
		 }
	  	 read_data(0xa0,weizhi_temp2,work_info_read,15);
		 Delay( 1000 );
		 fnShowwork_info_lishi(work_info_jishu_temp);
	  	 work_info_jishu_temp--;
	  }
	}else
	{
	  work_info_jishu_temp=10;
	  while (work_info_jishu_temp){
	  
	  	read_data(0xa0,work_info_weizhi-work_info_jishu_temp*32,work_info_read,15);
		Delay( 1000 );
		fnShowwork_info_lishi(work_info_jishu_temp);
	  	work_info_jishu_temp--;

	  }
	}

  }else{
	while (work_info_jishu_temp){
	  
	  read_data(0xa0,work_info_weizhi-work_info_jishu_temp*32,work_info_read,15);
	  Delay( 1000 );
	  fnShowwork_info_lishi(work_info_jishu_temp);
	  work_info_jishu_temp--;

	}
  }
 change_picture(0x03);
   
 while (1)
 {  
   if (rinterrupt==rinterrupt_Yes)
		{  
		rinterrupt = rinterrupt_No;
		 switch ( rbuf[8] )
		 {
			   
				case 0x20:
				{				
				    change_picture(0x02);
					return 1;
				}
				break;
				case 0x21:
				{				
				 
				}
				break;
		 }
	}
  }		
   
   
}




/*0--进入待机、1--返回设参*/
uchar DaiJi (void)
{    
			
	//显示信息刷新		
	Display_Energy_left( SetEnergy_left);
	Display_Energy_right( SetEnergy_right);
	Display_Frequency_left( SetFrequency );
	Display_Frequency_right( SetFrequency );					
	Power = SetFrequency*SetEnergy_right;
	Display_power_right( Power );
	Power = SetFrequency*SetEnergy_left;
	Display_power_left( Power );
	
	fnShowdaiji();	
	/*关闭外光闸*/
	fnguangzha_off();
	/*启动电机故障监测*/
	fnSelectTimer2_Fun2_first ();
	while (1)
	{
		if ( CheckZC_dai_zhun_Discharge( ) )/*检测流程、水温检测*/
		{
			return 1;
		}
		if (rinterrupt_ld==rinterrupt_Yes)
			{  
			  rinterrupt_ld = rinterrupt_No;
			  switch ( rbuf[8] )
			  {
			   
				case 0x00:
				{				
				  onf_flag = 1;
					pwm_ld_data=0;
				 show_ldoff_qiehuan();
				}
				break;
				case 0x01:
				{				
				  onf_flag = 0;
					pwm_ld_data=10000;
				 show_ldon_qiehuan();
				}
				break;
				case 0x02:
				{				
				 pwm_ld_data=20000;
				}
				break;
				case 0x03:
				{				
				 pwm_ld_data=30000;
				}
				break;
				case 0x04:
				{				
				 pwm_ld_data=40000;
				}
				break;
				case 0x05:
				{				
				 pwm_ld_data=55000;
				}
				break;
			  }
			  Pwm_LD(pwm_ld_data);

			}
		if (rinterrupt==rinterrupt_Yes)
		{  
			rinterrupt = rinterrupt_No;
			switch ( rbuf[8] )
			{
			   
				case 0x00:
				{
					FlagPara = parachang;
					if(double_pluse_flag){
							if(SetFrequency>6)
							{
								SetFrequency -=2;				/*12~40hz 频率减2*/
//								Alarm(sound7);
							}
							else if(SetFrequency>3)
							{                              /*5~11hz 频率减1*/
								SetFrequency --;
//								Alarm(sound7);
							}
							else
							{                             /*频率减越界*/
//								Alarm(sound3);
							}
							
					}else{
						if(SetFrequency>minFreq)
						{
							if (SetFrequency>40)
							{
								SetFrequency -=5;

							}
							else if(SetFrequency==40)
							{                               /*40hz 频率减4*/
								SetFrequency -=4;

							}
							else if (SetFrequency>24)
							{
								SetFrequency -=3;

							}
							else if (SetFrequency>=LimitFrequency)
							{
								SetFrequency--;
								SetFrequency--;
							}
							else
							{
								SetFrequency--;
							}
//							Alarm(sound7);						
						}
						else
						{
//							Alarm(sound3);
						}

					}

					CalMaxEner_Ho();
					if (SetEnergy_left>maxLimitSetEnergy_Ho)
					{
						SetEnergy_left = maxLimitSetEnergy_Ho;
						Display_Energy_left( SetEnergy_left);
					}
					if (SetEnergy_right>maxLimitSetEnergy_Ho)
					{
						SetEnergy_right = maxLimitSetEnergy_Ho;
						Display_Energy_right( SetEnergy_right);
					}
					Display_Frequency_left( SetFrequency );
					Display_Frequency_right( SetFrequency );	
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );

				}
				break;
				case 0x01:
				{
					FlagPara = parachang;
					if(double_pluse_flag){
							if( SetFrequency<6 )
							{
								SetFrequency++;				/*5~11hz 频率加1*/
//								Alarm(sound7);
							}
							 else if(SetFrequency<30)
							{                               /*12~40hz 频率加2*/
								SetFrequency +=2;
//								Alarm(sound7);
							}
							else
							{                             /*频率加越界*/
//								Alarm(sound3);
							}
					}else{
							if( SetFrequency<maxFreq )
							{
								if( SetFrequency<12 )
								{
									SetFrequency++;				/*5~11hz 频率加1*/
								}
								else if(SetFrequency<24)
								{                               /*12~22hz 频率加2*/
									SetFrequency +=2;
			
								}
								else if(SetFrequency<36)
								{                              /*24~33hz 频率加3*/
									SetFrequency +=3;
								}
								else if(SetFrequency==36)
								{                              /*36hz 频率加4*/
									SetFrequency +=4;
								}
								else if(SetFrequency<maxFreq)
								{                              /*40~55hz 频率加5*/
									SetFrequency +=5;
								}
//								Alarm(sound7);
							}
										 else
							{
								/*频率加越界*/
//								Alarm(sound3);
							}
					}
					CalMaxEner_Ho();
					if (SetEnergy_left>maxLimitSetEnergy_Ho)
					{
						SetEnergy_left= maxLimitSetEnergy_Ho;
						Display_Energy_left( SetEnergy_left);
					}
					if (SetEnergy_right>maxLimitSetEnergy_Ho)
					{
						SetEnergy_right = maxLimitSetEnergy_Ho;
						Display_Energy_right( SetEnergy_right);
					}
					Display_Frequency_left( SetFrequency );
					Display_Frequency_right( SetFrequency );	
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );
					
				}
				break;
				case 0x02:
				 {
					FlagPara = parachang;
					if(SetEnergy_left>minEner)
					{
						SetEnergy_left--;
//						Alarm (sound7);
					}
					else
					{
//						Alarm (sound3);
					}
					Display_Energy_left( SetEnergy_left);
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );

				}
				break;
				case 0x03:
				{
					FlagPara = parachang;
					CalMaxEner_Ho();
					if((SetEnergy_left<maxLimitSetEnergy_Ho)&&(SetEnergy_left<maxEner))
					{
						SetEnergy_left++;
//						Alarm (sound7);
					}
					else
					{
//						Alarm (sound3);
					}
					Display_Energy_left( SetEnergy_left);
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );


				}
				break;

				case 0x04:
				{
					FlagPara = parachang;
					if(double_pluse_flag){
							if(SetFrequency>6)
							{
								SetFrequency -=2;				/*12~40hz 频率减2*/
//								Alarm(sound7);
							}
							else if(SetFrequency>3)
							{                              /*5~11hz 频率减1*/
								SetFrequency --;
//								Alarm(sound7);
							}
							else
							{                             /*频率减越界*/
//								Alarm(sound3);
							}
							
					}else{
						if(SetFrequency>minFreq)
						{
							if (SetFrequency>40)
							{
								SetFrequency -=5;

							}
							else if(SetFrequency==40)
							{                               /*40hz 频率减4*/
								SetFrequency -=4;

							}
							else if (SetFrequency>24)
							{
								SetFrequency -=3;

							}
							else if (SetFrequency>=LimitFrequency)
							{
								SetFrequency--;
								SetFrequency--;
							}
							else
							{
								SetFrequency--;
							}
//							Alarm(sound7);						
						}
						else
						{
//							Alarm(sound3);
						}

					}
					CalMaxEner_Ho();
					if (SetEnergy_left>maxLimitSetEnergy_Ho)
					{
						SetEnergy_left= maxLimitSetEnergy_Ho;
						Display_Energy_left( SetEnergy_left);
					}
					if (SetEnergy_right>maxLimitSetEnergy_Ho)
					{
						SetEnergy_right = maxLimitSetEnergy_Ho;
						Display_Energy_right( SetEnergy_right);
					}
					Display_Frequency_left( SetFrequency );
					Display_Frequency_right( SetFrequency );					
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );

				}
				break;
				case 0x05:
				{
					FlagPara = parachang;
					if(double_pluse_flag){
							if( SetFrequency<6 )
							{
								SetFrequency++;				/*5~11hz 频率加1*/
//								Alarm(sound7);
							}
							 else if(SetFrequency<30)
							{                               /*12~40hz 频率加2*/
								SetFrequency +=2;
//								Alarm(sound7);
							}
							else
							{                             /*频率加越界*/
//								Alarm(sound3);
							}
					}else{
							if( SetFrequency<maxFreq )
							{
								if( SetFrequency<12 )
								{
									SetFrequency++;				/*5~11hz 频率加1*/
								}
								else if(SetFrequency<24)
								{                               /*12~22hz 频率加2*/
									SetFrequency +=2;
			
								}
								else if(SetFrequency<36)
								{                              /*24~33hz 频率加3*/
									SetFrequency +=3;
								}
								else if(SetFrequency==36)
								{                              /*36hz 频率加4*/
									SetFrequency +=4;
								}
								else if(SetFrequency<maxFreq)
								{                              /*40~55hz 频率加5*/
									SetFrequency +=5;
								}
//								Alarm(sound7);
							}
										 else
							{
								/*频率加越界*/
//								Alarm(sound3);
							}
					}
					CalMaxEner_Ho();
					if (SetEnergy_left>maxLimitSetEnergy_Ho)
					{
						SetEnergy_left= maxLimitSetEnergy_Ho;
						Display_Energy_left( SetEnergy_left);
					}
					if (SetEnergy_right>maxLimitSetEnergy_Ho)
					{
						SetEnergy_right = maxLimitSetEnergy_Ho;
						Display_Energy_right( SetEnergy_right);
					}
					Display_Frequency_left( SetFrequency );
					Display_Frequency_right( SetFrequency );					
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );
				

					
				}
				break;
				case 0x06:
				 {
					FlagPara = parachang;
					if(SetEnergy_right>minEner)
					{
						SetEnergy_right--;
	//					Alarm (sound7);
					}
					else
					{
	//					Alarm (sound3);
					}
					Display_Energy_right( SetEnergy_right);
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );
				
				}
				break;
				case 0x07:
				{
					FlagPara = parachang;
					CalMaxEner_Ho();
					if((SetEnergy_right<maxLimitSetEnergy_Ho)&&(SetEnergy_right<maxEner))
					{
						SetEnergy_right++;
	//					Alarm (sound7);
					}
					else
					{
	//					Alarm (sound3);
					}
					Display_Energy_right( SetEnergy_right);
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );				

				}
				break;
				
				
				
// 				case 0x08:
// 				{
// 				  change_picture(12);
// 				  return 3;

// 				}
// 				break;
				case 0x09:
				{
				  
				  return 4;

				}
				break;

				case 0x0b:/*指示光闪恒*/
				{
					if( fls_flag==0 ) //闪
					{
						fls_flag = 1;
						show_ldshan_qiehuan();
					}
					else			 //恒
					{
						fls_flag = 0;
						Pwm_LD(pwm_ld_data);
						show_ldheng_qiehuan();
						
					}
					
					
				}
				break;	
				
				case 0x0e:/*指示光开关*/
				{
					if( onf_flag==0 ) //关
					{
						onf_flag = 1;
						LCD_zengliangtiaojie_xiuzheng(0);
						pwm_ld_data=0;
						Pwm_LD(pwm_ld_data);
						show_ldoff_qiehuan();
					}
					else			 //开
					{
						onf_flag = 0;
						LCD_zengliangtiaojie_xiuzheng(1);
						pwm_ld_data=10000;
						Pwm_LD(pwm_ld_data);
						show_ldon_qiehuan();
						
					}
					
					
				}
				break;	
				
				case 0x0d:
				{
					/*LCD[系统调整中]*/
					LCD_SystemAdjust();
				//	show_zhunbei_qiehuan();

					/*禁止电机检测功能*/
					biTestDianji = notTestDianji;
					

					
					if(FlagPara == parachang){
						if( double_pluse_flag==0 )
						{
							/*保存设置频率值*/
							saveshuzu[0]=SetFrequency;
							SetFrequency_only=SetFrequency;
							write_data(0xa0,AddressFreq_left,saveshuzu,1);
							/*连续存储延时*/
							Delay( 1000 );
							/*保存设置能量值*/
							saveshuzu[0]=SetEnergy_left;
							SetEnergy_left_only=SetEnergy_left;
							write_data(0xa0,AddressEner_left,saveshuzu,1);
							Delay( 1000 );
							saveshuzu[0]=SetEnergy_right;
							SetEnergy_right_only=SetEnergy_right;
							write_data(0xa0,AddressEner_right,saveshuzu,1);
							Delay( 1000 );
					
						}
						else
						{
							/*保存设置频率值*/
							saveshuzu[0]=SetFrequency;
							SetFrequency_double=SetFrequency;
							write_data(0xa0,AddressFreq_left_double,saveshuzu,1);
							/*连续存储延时*/
							Delay( 1000 );
							/*保存设置能量值*/
							saveshuzu[0]=SetEnergy_left;
							SetEnergy_left_double=SetEnergy_left;
							write_data(0xa0,AddressEner_left_double,saveshuzu,1);
							Delay( 1000 );		
							saveshuzu[0]=SetEnergy_right;
							SetEnergy_right_double=SetEnergy_right;
							write_data(0xa0,AddressEner_right_double,saveshuzu,1);
							Delay( 1000 );								
						}

						fnGenJuShangCiPinLvTiaoZheng(SetFrequency);//调整电机转速
						fnDianJiCheckAdjust( SetFrequency ); //电机转速微调，测量周期

						Power_FeedBack(SetEnergy_left);  //显示功率或能量反馈曲线参考值
						Ener_Sleft_A=Ener_SA_temp;
						Ener_Sleft_B=Ener_SB_temp;
						if (Flag_ShowFeedBack==Show_Enable)
						{		
							Display_FeedBackEnergyVolt_left( Ener_Sleft_A*10,  Ener_Sleft_B*10);
						}
						
						Power_FeedBack(SetEnergy_right);  //显示功率或能量反馈曲线参考值
						Ener_Sright_A=Ener_SA_temp;
						Ener_Sright_B=Ener_SB_temp;
						if (Flag_ShowFeedBack==Show_Enable)
						{		
							Display_FeedBackEnergyVolt_right( Ener_Sright_A*10,  Ener_Sright_A*10);
						}
					}

					

					/*设置流程标志*/
					Flag_Flow = Flag_ZhengChang;	 
					
					return 2;
				

				}
				break;

				case 0x18: //短脉冲
				{
				   FlagPara = parachang;
				   Pulse_time_left=6;
					 Pulse_time_right=6;
					 Pulse_time2=6;
					 long_short_pluse_flag=0;
				   LCD_Pulsetime();
					 show_short_qiehuan();
				}
				break;
				case 0x19://长脉冲
				{
				   FlagPara = parachang;
				   Pulse_time_left=10;
					 Pulse_time_right=10;
					 Pulse_time2=10;
					 long_short_pluse_flag=1;
				   LCD_Pulsetime();
					 show_long_qiehuan();
				}
				break;
				
				case 0x1A:  //单脉冲切换
				{
				  FlagPara = parachang;

					double_pluse_flag = 0;	//0 单脉冲	
					SetFrequency=SetFrequency_only;
					SetEnergy_left=SetEnergy_left_only;			
					SetEnergy_right=SetEnergy_right_only;		
					show_single_qiehuan();

				  LCD_Pulsetime();

					Display_Energy_left( SetEnergy_left);
					Display_Energy_right( SetEnergy_right);
					Display_Frequency_left( SetFrequency );
					Display_Frequency_right( SetFrequency );					
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );

				}
				break;
				
				case 0x1B:  //双脉冲切换
				{
				  FlagPara = parachang;

					double_pluse_flag = 1;	//1 多脉冲		
					SetFrequency=SetFrequency_double;
					SetEnergy_left=SetEnergy_left_double;		
					SetEnergy_right=SetEnergy_right_double;	
					show_double_qiehuan();

				  LCD_Pulsetime();
					Display_Energy_left( SetEnergy_left);
					Display_Energy_right( SetEnergy_right);
					Display_Frequency_left( SetFrequency );
					Display_Frequency_right( SetFrequency );					
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );

				}
				break;
				

				default:/*按其它键无效*/
				{
//					Alarm(sound3);
				}
				break;
				
			}
		}
		
		
	}
}



uchar ZhunBei (void)
{    
	uchar xiefang_pulse;	   
	uchar SetEnergy_jishu;

	
	/*增加打光时按待机键返回待机状态*/
	#define HaltLaser2DaiJi_Enable 1
	#define HaltLaser2DaiJi_Disable 0
	bit HaltLaser2DaiJi = HaltLaser2DaiJi_Disable;
	
	
	/*初始设置为非充放电状态*/
	biDischanging = Dischanging_No;
    
	//光闸打开且检测
	fnguangzha_on();
	/*启动电机故障监测*/
	fnSelectTimer2_Fun2_first ();

	//准备闪烁3次
	
	show_zhunbei_qiehuan();
 	//如果参数有改变，则泄放电容库电压
	if (FlagPara!=paraunchang){
					for(xiefang_pulse=0;xiefang_pulse<50;xiefang_pulse++){	 //放电50次	0.5s
						PULSE = 1;
						Delay (20);
						PULSE = 0;
						Delay(1000);
					}			
	}
	show_daiji_qiehuan();
	if (FlagPara!=paraunchang){
					for(xiefang_pulse=0;xiefang_pulse<50;xiefang_pulse++){	 //放电50次	0.5s
						PULSE = 1;
						Delay (20);
						PULSE = 0;
						Delay(1000);
					}			
	}
	show_zhunbei_qiehuan();
	Delay (50000);
	show_daiji_qiehuan();
	Delay (50000);
	show_zhunbei_qiehuan();
	Delay (50000);
	show_daiji_qiehuan();
	Delay (50000);
	show_zhunbei_qiehuan();

	fnShowzhunbei ();
//	Alarm(sound2);

	while (1)
	{
		/*非充放电状态*/
		if ( biDischanging==Dischanging_No )
		{   
			if (rinterrupt_ld==rinterrupt_Yes)
			{  
			  rinterrupt_ld = rinterrupt_No;
			  switch ( rbuf[8] )
			  {
			   
				case 0x00:
				{				
				  onf_flag = 1;
					pwm_ld_data=0;
				 show_ldoff_qiehuan();
				}
				break;
				case 0x01:
				{				
				  onf_flag = 0;
					pwm_ld_data=10000;
				 show_ldon_qiehuan();
				}
				break;
				case 0x02:
				{				
				 pwm_ld_data=20000;
				}
				break;
				case 0x03:
				{				
				 pwm_ld_data=30000;
				}
				break;
				case 0x04:
				{				
				 pwm_ld_data=40000;
				}
				break;
				case 0x05:
				{				
				 pwm_ld_data=55000;
				}
				break;
			  }
			  Pwm_LD(pwm_ld_data);

			}
		
			if (rinterrupt==rinterrupt_Yes)
			{  
				rinterrupt = rinterrupt_No;
				switch ( rbuf[8] )
				{
						   
						
					case 0x02:
				   {
					FlagEnergy_L = decEnergy;
					if(SetEnergy_left>minEner)
					{
						SetEnergy_left--;
//						Alarm (sound7);
					}
					else
					{
//						Alarm (sound3);
					}
					Display_Energy_left( SetEnergy_left);
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );

					}
					break;
					case 0x03:
					{
					FlagEnergy_L = incEnergy;
					CalMaxEner_Ho();
					if((SetEnergy_left<maxLimitSetEnergy_Ho)&&(SetEnergy_left<maxEner))
					{
						SetEnergy_left++;
//						Alarm (sound7);
					}
					else
					{
//						Alarm (sound3);
					}
					Display_Energy_left( SetEnergy_left);
					Power = SetFrequency*SetEnergy_left;
					Display_power_left( Power );
					}
					break;
					
					case 0x06:
					 {
					FlagEnergy_R = decEnergy;
					if(SetEnergy_right>minEner)
					{
						SetEnergy_right--;
		//				Alarm (sound7);
					}
					else
					{
		//				Alarm (sound3);
					}
					Display_Energy_right( SetEnergy_right);
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );

					}
					break;
					case 0x07:
					{
					FlagEnergy_R = incEnergy;
					CalMaxEner_Ho();
					if((SetEnergy_right<maxLimitSetEnergy_Ho)&&(SetEnergy_right<maxEner))
					{
						SetEnergy_right++;
	//					Alarm (sound7);
					}
					else
					{
		//				Alarm (sound3);
					}
					Display_Energy_right( SetEnergy_right);
					Power = SetFrequency*SetEnergy_right;
					Display_power_right( Power );				

					}
					break;								
					
					case 0x0c://待机
					{

								
						show_daiji_qiehuan();
//						Alarm (sound2);
						return 1;/*退出准备状态，返回待机状态*/
							

					}
					break;
					
					case 0x0b:/*指示光闪恒*/
					{
					if( fls_flag==0 ) //闪
					{
						fls_flag = 1;
						show_ldshan_qiehuan();
					}
					else			 //恒
					{
						fls_flag = 0;
						Pwm_LD(pwm_ld_data);
						show_ldheng_qiehuan();
						
					}
					
					
					}
					break;	

					case 0x0e:/*指示光开关*/
					{
						if( onf_flag==0 ) //关
						{
							onf_flag = 1;
							LCD_zengliangtiaojie_xiuzheng(0);
							pwm_ld_data=0;
							Pwm_LD(pwm_ld_data);
							show_ldoff_qiehuan();
						}
						else			 //开
						{
							onf_flag = 0;
							LCD_zengliangtiaojie_xiuzheng(1);
							pwm_ld_data=10000;
							Pwm_LD(pwm_ld_data);
							show_ldon_qiehuan();
							
						}								
					}
					break;	
					
					default:/*按其它键无效*/
					{
//								Alarm(sound3);
					}
					break;
							
				}
			}
			/*只在非充放电状态允许用户使用键盘*/
			
		}
		/*非充放电状态*/
		if ( biDischanging==Dischanging_No )
		{
			/*执行一般检测*/
			if ( CheckZC_dai_zhun_Discharge() )
			{
				return 1;
			}
			
			if (MB_SGX==Check_Enable)	  //每2秒钟检测一次射频卡
			{
	//			check_GX_zhunbei();
			}
			
			/*刚刚踩下脚踏开关*/
			P4status=P4;
			if ( SJTA0==1)
			{
				/*LCD[治疗中----]*/
				LCD_Curing( );
				show_fashe_left();
				SetEnergy_jishu = SetEnergy_left;
				/*设置当前为充放电状态*/
				biDischanging = Dischanging_Yes;								
				/*脉冲计数清零*/
				CurPulse = 0;
				/*超差-50%、充电电压超1000V，计次清零*/
				Count_F50V1000 = 0;
				Pulse_time=Pulse_time_left;
				if( double_pluse_flag==0&&long_short_pluse_flag==1 ) //单脉冲 长脉冲
				{
// 					if(SetFrequency==20&&SetEnergy_left>40)
// 					{
// 						Pulse_time=13;
// 					}
// // 					if(SetFrequency==24&&SetEnergy_left>35)
// // 					{
// // 						Pulse_time=12;
// // 					}
// 													
// 					if(SetFrequency==40&&SetEnergy_left>15)
// 					{
// 						Pulse_time=11;
// 					}
				}


				//显示功率或能量反馈曲线参考值
				if (FlagEnergy_L!=unChangEnergy)
				{					
					Power_FeedBack(SetEnergy_left);  //显示功率或能量反馈曲线参考值
					Ener_Sleft_A=Ener_SA_temp;
					Ener_Sleft_B=Ener_SB_temp;

					if (Flag_ShowFeedBack==Show_Enable)
					{		
						Display_FeedBackEnergyVolt_left( Ener_Sleft_A*10,  Ener_Sleft_B*10);
					}

					if( double_pluse_flag)
						{
							/*保存设置能量值*/
							saveshuzu[0]=SetEnergy_left;
							SetEnergy_left_double=SetEnergy_left;
							write_data(0xa0,AddressEner_left_double,saveshuzu,1);
							Delay( 1000 );					
						}
						else
						{
							/*保存设置能量值*/
							saveshuzu[0]=SetEnergy_left;
							SetEnergy_left_only=SetEnergy_left;
							write_data(0xa0,AddressEner_left,saveshuzu,1);
							Delay( 1000 );
											
						}
						
				}
				//如果准备状态下能量降低，则泄放电容库电压
				if (FlagEnergy_L==decEnergy||((JT_L_R==Right)&&(SetEnergy_right>SetEnergy_left+5))){
					for(xiefang_pulse=0;xiefang_pulse<100;xiefang_pulse++){	 //放电100次	1s
						PULSE = 1;
						Delay (25);
						PULSE = 0;
						Delay(975);
					}
				  			
				}

				/*初始充电电压*/
				if (FlagEnergy_L!=unChangEnergy||FlagPara!=paraunchang||JT_L_R==Right)
				{
					/*如果参数发生变化，使用统一初始值*/
					if( double_pluse_flag)
					{
						if(Pulse_time>6){
						uiChangVolt_A = Vc0+(SetEnergy_left/2-5)*3;
						uiChangVolt_B = Vc0+(SetEnergy_left/2-5)*3;
						uiChangVolt_A_MAX=550;	  //赋初值550v
						uiChangVolt_B_MAX=550;
					}else{
						uiChangVolt_A = 600+(SetEnergy_left/2-5)*4;
						uiChangVolt_B = 600+(SetEnergy_left/2-5)*4;
						uiChangVolt_A_MAX=650;	  //赋初值550v
						uiChangVolt_B_MAX=650;
						if(uiChangVolt_A>=750)
								uiChangVolt_A=750;
						if(uiChangVolt_B>=750)
								uiChangVolt_B=750;
					}
					
					}else
					{
						if(Pulse_time>6){
						uiChangVolt_A = Vc0+(SetEnergy_left-5)*4;
						uiChangVolt_B = Vc0+(SetEnergy_left-5)*4;
						uiChangVolt_A_MAX=550;	  //赋初值550v
						uiChangVolt_B_MAX=550;
					}else{
						uiChangVolt_A = 620+(SetEnergy_left-5)*7;
						uiChangVolt_B = 620+(SetEnergy_left-5)*7;
						uiChangVolt_A_MAX=650;	  //赋初值550v
						uiChangVolt_B_MAX=650;
						if(uiChangVolt_A>=750)
								uiChangVolt_A=750;
						if(uiChangVolt_B>=750)
								uiChangVolt_B=750;
					}
					}
					
					Ener_A_MAX=2;	//防止底噪产生的干扰，赋初值0.2v
					Ener_B_MAX=2;
					Tiaojie_A =0;
					Tiaojie_B =0;
					shuiwen_chuli_old=shuiwen_chuli;
				}
				else
				{	//参数为变化，沿用上次充电电压,由于再次踩脚踏时水温下降而使得前几个脉冲能量过高，所以适当降低上次保存的电压值
				  if( double_pluse_flag )
				  {
						if(SetEnergy_left<=30){ 
							if( SetFrequency>=3 && SetFrequency<6 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left*5/10;									
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left*5/10;
									}
							}else if ( SetFrequency>=6 && SetFrequency<=30 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left*5/10;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left*5/10;
							}
						}else{
							if( SetFrequency>=3 && SetFrequency<6 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left;								
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left;
									}
							}else if ( SetFrequency>=6 && SetFrequency<=30 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left;
							}
						
						}
					}else
				  {
						if(SetEnergy_left<=15){ 
							if( SetFrequency>=5 && SetFrequency<=12 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left*5/10;									
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left*5/10;
									}
							}else if ( SetFrequency>=14 && SetFrequency<=60 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left*5/10;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left*5/10;
							}
						}else{
							if( SetFrequency>=5 && SetFrequency<=12 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left;								
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left;
									}
							}else if ( SetFrequency>=14 && SetFrequency<=60 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_left;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_left;
							}
						
						}
					}

					if(SetFrequency==40&&SetEnergy_left>10)
					{
						Tiaojie_A =0;
						Tiaojie_B =0;
					}
				}


				if( double_pluse_flag )
				{
					if( SetFrequency>=3 && SetFrequency<6 ){
					if(ucChoiceABlight==ALight){
					fnDischangVolt_OutPut_uiA(uiChangVolt_A);
					CA = 1;
					CB = 0;
					Select_Laser=ALight;
					}else if(ucChoiceABlight==BLight){
					fnDischangVolt_OutPut_uiB(uiChangVolt_B);
					CA = 0;
					CB = 1;
					Select_Laser=BLight;
						}
					}else if ( SetFrequency>=6 && SetFrequency<=30 ){
					
				/*高频放电每次都从通光位置开始，即A  先放电*/
					CA = 1;
						CB = 0;
					Select_Laser=ALight;
					fnDischangVolt_OutPut_uiA( uiChangVolt_A );
					fnDischangVolt_OutPut_uiB( uiChangVolt_B );
					}
				
				}
				else
				{
					if( SetFrequency>=5 && SetFrequency<=12 ){
					  if(ucChoiceABlight==ALight){
						fnDischangVolt_OutPut_uiA(uiChangVolt_A);
					 	CA = 1;
						CB = 0;
						Select_Laser=ALight;
				  	  }else if(ucChoiceABlight==BLight){
						fnDischangVolt_OutPut_uiB(uiChangVolt_B);
					 	CA = 0;
						CB = 1;
						Select_Laser=BLight;
				  	  }
				  }else if ( SetFrequency>=14 && SetFrequency<=60 ){
						
					/*高频放电每次都从通光位置开始，即A  先放电*/
					  CA = 1;
				  	  CB = 0;
					  Select_Laser=ALight;
					  fnDischangVolt_OutPut_uiA( uiChangVolt_A );
					  fnDischangVolt_OutPut_uiB( uiChangVolt_B );
				  }
				}


					//显示找到的电压
					if (Flag_ShowFeedBack==Show_Enable)
					{						
						LCD_A_ChargeVolt(uiChangVolt_A);
						LCD_B_ChargeVolt(uiChangVolt_B);
					}


				    ucDischangingDianJi_T2overflows = 0x00;
					/*禁止电机检测功能*/
					biTestDianji = notTestDianji;

					CCDFD=1;  //允许充电电源工作
					PULSE = 0;
					//启动边沿捕获放电
					biChoiceT2Fun = T2Fun1;
					capture = T2SpringDischarge_DingWei;
					T2CON = 0x09;/*捕获*/
					ET2 = 1;
					TR2 = 1;
					EXF2 = 0;	  //很重要的清除边沿中断标志位
					TF2 = 0;

					JT_L_R=Left;
					/*标志量设置参数为未改变*/
					FlagPara = paraunchang;
					FlagEnergy_L = unChangEnergy;
					FSFMQ = 0;
			}
			else if ( SJTA1==1)
			{
				/*LCD[治疗中----]*/
				LCD_Curing( );
				show_fashe_right();
				SetEnergy_jishu = SetEnergy_right;
				/*设置当前为充放电状态*/
				biDischanging = Dischanging_Yes;								
				/*脉冲计数清零*/
				CurPulse = 0;
				/*超差-50%、充电电压超1000V，计次清零*/
				Count_F50V1000 = 0;
				Pulse_time=Pulse_time_right;
				if( double_pluse_flag==0&&long_short_pluse_flag==1 ) //单脉冲 长脉冲
				{
// 					if(SetFrequency==20&&SetEnergy_right>40)
// 					{
// 						Pulse_time=13;
// 					}
// // 					if(SetFrequency==24&&SetEnergy_right>35)
// // 					{
// // 						Pulse_time=12;
// // 					}
// 													
// 					if(SetFrequency==40&&SetEnergy_right>15)
// 					{
// 						Pulse_time=11;
// 					}
				}

				//显示功率或能量反馈曲线参考值
				if (FlagEnergy_R!=unChangEnergy)
				{					
					Power_FeedBack(SetEnergy_right);  //显示功率或能量反馈曲线参考值
					Ener_Sright_A=Ener_SA_temp;
					Ener_Sright_B=Ener_SB_temp;

					if (Flag_ShowFeedBack==Show_Enable)
					{		
						Display_FeedBackEnergyVolt_right( Ener_Sright_A*10,  Ener_Sright_B*10);
					}

					if( double_pluse_flag)
						{
							/*保存设置能量值*/
							saveshuzu[0]=SetEnergy_right;
							SetEnergy_right_double=SetEnergy_right;
							write_data(0xa0,AddressEner_right_double,saveshuzu,1);
							Delay( 1000 );	
					
						}
						else
						{
							/*保存设置能量值*/
							saveshuzu[0]=SetEnergy_right;
							SetEnergy_right_only=SetEnergy_right;
							write_data(0xa0,AddressEner_right,saveshuzu,1);
							Delay( 1000 );
											
						}
						
				}
				//如果准备状态下能量降低，则泄放电容库电压
				if (FlagEnergy_R==decEnergy||((JT_L_R==Left)&&(SetEnergy_left>SetEnergy_right+5))){
					for(xiefang_pulse=0;xiefang_pulse<100;xiefang_pulse++){	 //放电100次	1s
						PULSE = 1;
						Delay (25);
						PULSE = 0;
						Delay(975);
					}
				  			
				}

				/*初始充电电压*/
				if (FlagEnergy_R!=unChangEnergy||FlagPara!=paraunchang||JT_L_R==Left)
				{
					/*如果参数发生变化，使用统一初始值*/
					
					if( double_pluse_flag)
					{
						if(Pulse_time>6){
						uiChangVolt_A = Vc0+(SetEnergy_right/2-5)*3;
						uiChangVolt_B = Vc0+(SetEnergy_right/2-5)*3;
						uiChangVolt_A_MAX=550;	  //赋初值550v
						uiChangVolt_B_MAX=550;
					}else{
						uiChangVolt_A = 600+(SetEnergy_right/2-5)*4;
						uiChangVolt_B = 600+(SetEnergy_right/2-5)*4;
						uiChangVolt_A_MAX=650;	  //赋初值550v
						uiChangVolt_B_MAX=650;
						if(uiChangVolt_A>=750)
								uiChangVolt_A=750;
						if(uiChangVolt_B>=750)
								uiChangVolt_B=750;
					}
					
					}else
					{
						if(Pulse_time>6){
						uiChangVolt_A = Vc0+(SetEnergy_right-5)*4;
						uiChangVolt_B = Vc0+(SetEnergy_right-5)*4;
						uiChangVolt_A_MAX=550;	  //赋初值550v
						uiChangVolt_B_MAX=550;
					}else{
						uiChangVolt_A = 620+(SetEnergy_right-5)*7;
						uiChangVolt_B = 620+(SetEnergy_right-5)*7;
						uiChangVolt_A_MAX=650;	  //赋初值550v
						uiChangVolt_B_MAX=650;
						if(uiChangVolt_A>=750)
								uiChangVolt_A=750;
						if(uiChangVolt_B>=750)
								uiChangVolt_B=750;
					}
					}
					
					Ener_A_MAX=2;	//防止底噪产生的干扰，赋初值0.2v
					Ener_B_MAX=2;
					Tiaojie_A =0;
					Tiaojie_B =0;
					shuiwen_chuli_old=shuiwen_chuli;
				}
				else
				{	//参数为变化，沿用上次充电电压,由于再次踩脚踏时水温下降而使得前几个脉冲能量过高，所以适当降低上次保存的电压值
				  if( double_pluse_flag )
				  {
						if(SetEnergy_right<=30){ 
							if( SetFrequency>=3 && SetFrequency<6 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right*5/10;									
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right*5/10;
									}
							}else if ( SetFrequency>=6 && SetFrequency<=30 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right*5/10;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right*5/10;
							}
						}else{
							if( SetFrequency>=3 && SetFrequency<6 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right;								
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right;
									}
							}else if ( SetFrequency>=6 && SetFrequency<=30 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right;
							}
						
						}
					}else
				  {
						if(SetEnergy_right<=15){ 
							if( SetFrequency>=5 && SetFrequency<=12 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right*5/10;									
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right*5/10;
									}
							}else if ( SetFrequency>=14 && SetFrequency<=60 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right*5/10;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right*5/10;
							}
						}else{
							if( SetFrequency>=5 && SetFrequency<=12 ){
									if(ucChoiceABlight==ALight){
										uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right;								
									}else if(ucChoiceABlight==BLight){
										uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right;
									}
							}else if ( SetFrequency>=14 && SetFrequency<=60 ){
								uiChangVolt_A = uiChangVolt_A_last-SetEnergy_right;
								uiChangVolt_B = uiChangVolt_B_last-SetEnergy_right;
							}
						
						}
					}
					if(SetFrequency==40&&SetEnergy_right>10)
					{
						Tiaojie_A =0;
						Tiaojie_B =0;
					}
					

				}


				if( double_pluse_flag )
				{
					if( SetFrequency>=3 && SetFrequency<6 ){
					if(ucChoiceABlight==ALight){
					fnDischangVolt_OutPut_uiA(uiChangVolt_A);
					CA = 1;
					CB = 0;
					Select_Laser=ALight;
					}else if(ucChoiceABlight==BLight){
					fnDischangVolt_OutPut_uiB(uiChangVolt_B);
					CA = 0;
					CB = 1;
					Select_Laser=BLight;
						}
					}else if ( SetFrequency>=6 && SetFrequency<=30 ){
					
				/*高频放电每次都从通光位置开始，即A  先放电*/
					CA = 1;
						CB = 0;
					Select_Laser=ALight;
					fnDischangVolt_OutPut_uiA( uiChangVolt_A );
					fnDischangVolt_OutPut_uiB( uiChangVolt_B );
					}
				
				}
				else
				{
					if( SetFrequency>=5 && SetFrequency<=12 ){
					  if(ucChoiceABlight==ALight){
						fnDischangVolt_OutPut_uiA(uiChangVolt_A);
					 	CA = 1;
						CB = 0;
						Select_Laser=ALight;
				  	  }else if(ucChoiceABlight==BLight){
						fnDischangVolt_OutPut_uiB(uiChangVolt_B);
					 	CA = 0;
						CB = 1;
						Select_Laser=BLight;
				  	  }
				  }else if ( SetFrequency>=14 && SetFrequency<=60 ){
						
					/*高频放电每次都从通光位置开始，即A  先放电*/
					  CA = 1;
				  	  CB = 0;
					  Select_Laser=ALight;
					  fnDischangVolt_OutPut_uiA( uiChangVolt_A );
					  fnDischangVolt_OutPut_uiB( uiChangVolt_B );
				  }
				}


					//显示找到的电压
					if (Flag_ShowFeedBack==Show_Enable)
					{						
						LCD_A_ChargeVolt(uiChangVolt_A);
						LCD_B_ChargeVolt(uiChangVolt_B);
					}


				    ucDischangingDianJi_T2overflows = 0x00;
					/*禁止电机检测功能*/
					biTestDianji = notTestDianji;

					CCDFD=1;  //允许充电电源工作
					PULSE = 0;
					//启动边沿捕获放电
					biChoiceT2Fun = T2Fun1;
					capture = T2SpringDischarge_DingWei;
					T2CON = 0x09;/*捕获*/
					ET2 = 1;
					TR2 = 1;
					EXF2 = 0;	  //很重要的清除边沿中断标志位
					TF2 = 0;

					JT_L_R=Right;
					/*标志量设置参数为未改变*/
					FlagPara = paraunchang;
					FlagEnergy_R = unChangEnergy;
					FSFMQ = 0;
			}
		}
		/*正在充放电*/
		else if ( biDischanging==Dischanging_Yes  )
			{
				count1ms=count50us/20;
				if ( count1ms>=1&&count1ms<=2)
				{
					
					/*处理采样数据，判断能量选择下二次的充电电压，*/
					if(onecheck_flag==1){
						onecheck_flag=0;
						if ( CurPulse>6 ) 
						{
							fnEnergyControl_ZhunBei_first();
						}
						
						CNL=1;				//能量反馈清零
		      		}																											
											
				}
				else if ( count1ms>=4&&count1ms<=5)
				{
						if ( CurPulse>6 ) 
						{
							if ( fnEnergyControl_ZhunBei() )
							{
		         				halt( );
								return 1;
		         	}
						}
						CNL=1;				//能量反馈清零
						
					/*执行充放电中的检测*/
					if ( Check_Dischanging_1040 () || CheckZC_dai_zhun_Discharge() )
					{
						return 1;
					}
								
					if (rinterrupt==rinterrupt_Yes)
		      			{
			      			rinterrupt = rinterrupt_No;
				    		if (rbuf[8]==0x0c)               //待机键
				    		{
				    			HaltLaser2DaiJi=HaltLaser2DaiJi_Enable;
								goto HiFre_End;
				    		}				    
				  		}
					
					
					ET0 = 0;
		      TR0 = 0;/*关闭放电及能量反馈计时*/

					TR2 = 1;  //开启定时器2
					ET2 = 1;
					EXEN2=1;			//允许引脚捕获功能	
					EXF2 = 0;	  //很重要的清除边沿中断标志位
					TF2 = 0;

					count50us=0;			  //放电计时清零
							
					if(one_orflag==0)	  //切换放电抢
					{	 
						if( double_pluse_flag )
						{
							if(SetFrequency>=6)			   /*14-40hz 三路工作*/
							{
								if(Select_Laser == ALight){
									CA = 0;
									CB = 1;
									Select_Laser=BLight;
								}else if(Select_Laser == BLight){
									CA = 1;
									CB = 0;
									Select_Laser=ALight;
								}
							}
						}else{
							if(SetFrequency>=14)			   /*14-40hz 三路工作*/
							{
								if(Select_Laser == ALight){
									CA = 0;
									CB = 1;
									Select_Laser=BLight;
								}else if(Select_Laser == BLight){
									CA = 1;
									CB = 0;
									Select_Laser=ALight;
								}
							}
						}							
					  }
				}
				else
				{
					/*刚刚抬起脚踏开关*/
					P4status=P4;
					if(JT_L_R==Right)
					{
					  if ( SJTA1==0 )
					  {
						Delay_100us();
						P4status=P4;
						if ( SJTA1==0 )
						{
							Delay_100us();
							P4status=P4;
							if ( SJTA1==0 )
							{
							   goto HiFre_End;
							}
						}
					  }
					}
					if(JT_L_R==Left)
					{
					  if ( SJTA0==0 )
					  {
						Delay_100us();
						P4status=P4;
						if ( SJTA0==0 )
						{
							Delay_100us();
							P4status=P4;
							if ( SJTA0==0 )
							{
								/*设置变量停止充放电*/
HiFre_End:						PULSE = 0;
								CA = 1;
								CB = 0;
								CCDFD=0;  //禁止充电电源工作
								TR0 = 0;
				      			ET0 = 0;/*停止充放电中断*/
								TR2 = 0;
								ET2 = 0;/*停止充放电中断*/
								/*标志复位*/
								biDischanging = Dischanging_No;
								/*充电电压归零*/
								fnDischangVolt_OutPut_uiA(0);
								fnDischangVolt_OutPut_uiB(0);
								/*启动电机故障监测*/
								fnSelectTimer2_Fun2_first();
								/*LCD[激光准备发射----]*/
								fnShowzhunbei ();
	 						  show_fashe_no();
								CLAMP=0;
								Clamp_flag=0;
								FSFMQ = 1;
								CNL=1;
								/*保存末次充电电压*/
								uiChangVolt_A_last = uiChangVolt_A;
								uiChangVolt_B_last = uiChangVolt_B;
						
								
								//记录本次数据
								TotalJoule_benci+=(ulong)SetEnergy_jishu*(ulong)CurPulse;
								TotalJoule_benci_1 = TotalJoule_benci/10;
                                TotalPulse_benci+=(ulong)CurPulse;
								Totalminutes_benci+=(float)CurPulse/(float)SetFrequency/6;
								//长时间开机，不停机的情况下判断下是否超限
								if(TotalJoule_benci_1>=9999999)
									TotalJoule_benci_1=9999999;
								if(TotalPulse_benci>=9999999)
									TotalPulse_benci=9999999;
								if(Totalminutes_benci>=9999)
									Totalminutes_benci=9999;
						    

								
									//打光累计超过18S
								if (MB_SGX==Check_Enable)	  //写射频卡次数
								{
		//							GX_write();
								}
								
								if(Totalminutes_benci>=5&&baoyang_jishu_flag==0){  //30s
									baoyang_jishu_flag=1;
									baoyang_jishu++;
									saveshuzu[0]=baoyang_jishu/256;
									write_data(0xa0,Address_baoyang_jishu,saveshuzu,1);
									Delay( 1000 );
									saveshuzu[0]=baoyang_jishu%256;
									write_data(0xa0,Address_baoyang_jishu+1,saveshuzu,1);
									Delay( 1000 );
								}
								

								//存储本次数据
								work_info_write[0]=TotalPulse_benci/65536;
								work_info_write[1]=TotalPulse_benci%65536/256;
								work_info_write[2]=TotalPulse_benci%256;
								
								work_info_write[3]=TotalJoule_benci_1/65536;
								work_info_write[4]=TotalJoule_benci_1%65536/256;
								work_info_write[5]=TotalJoule_benci_1%256;
								
								work_info_write[6]=(uint)Totalminutes_benci/256;
								work_info_write[7]=(uint)Totalminutes_benci%256;

								write_data(0xa0,work_info_weizhi+6,work_info_write,8);	  
								Delay( 500 );

									    

								if (HaltLaser2DaiJi==HaltLaser2DaiJi_Enable)
								{
									HaltLaser2DaiJi=HaltLaser2DaiJi_Disable;
								
									show_daiji_qiehuan();
//									Alarm (sound2);
									return 1;/*退出准备状态，返回待机状态(已经找到SVOL)*/
								}
								

								if (Flag_ShowFeedBack==Show_Enable)
								{
									//1显示找到的电压
									
									LCD_A_ChargeVolt(uiChangVolt_A);
									LCD_B_ChargeVolt(uiChangVolt_B);
									
								}

							}
						}
					}
				  }
				}
			}
		
	}
}


/*timecount++;//50us加一，tf0溢出20次为1ms；counter单位是ms，电机周期的4分之一时间;count1ms实际上是50us加1，从放电瞬间开始加*/
void T0_int (void) interrupt 1
{
   	TR0=0;
   	TL0 = (65536-92)%256;     //系统时钟12分频 作为定时器2时钟源	           
	  TH0 = (65536-92)/256;     //精度可以满足   此定时为50us，准确数为49.945us	  精度1%
   	TR0=1;
	
		count50us++;
	  if( double_pluse_flag )  //双脉冲模式
	  {
			if(Pulse_time_flag==1&&count50us==Pulse_time) {	  //第一个脉冲的放电延时的放电后沿
				PULSE = 0;					  //放电停止
				ucDischangingDianJi_T2overflows = 0x00;
												
			}
			if(Pulse_time_flag==1&&count50us==Pulse_jiange) {	  //第二个脉冲的放电延时的放电前沿
				CNL=0; 
				PULSE = 1;					  //放电开始										  
			} 
			if(Pulse_time_flag==1&&count50us==(Pulse_time2+Pulse_jiange)) {	  //第二个脉冲的放电延时的放电后沿
				PULSE = 0;					  //放电停止
				Pulse_time_flag=0;			  //清除放电标志
				TotalPulse_U++;/*12us*/
					CurPulse++;/*6us*/
				ucDischangingDianJi_T2overflows = 0x00;
												
			} 
		}else{   //单脉冲模式
			if(Pulse_time_flag==1&&count50us==Pulse_time) {	  //第一个脉冲的放电延时的放电后沿
				PULSE = 0;					  //放电停止
			  Pulse_time_flag=0;			  //清除放电标志
				TotalPulse_U++;/*12us*/
					CurPulse++;/*6us*/
				ucDischangingDianJi_T2overflows = 0x00;
												
			}
		}

}

/*TF2溢出时间为0.03555556s，T2走一步时间为0.542us，5hz-20hz电机周期为200--50ms*/
void T2_int (void) interrupt 5
{


	if ( biChoiceT2Fun==T2Fun1 )
	{
		if (capture==T2MeasureSpeed)
		{
			if ( TF2==1 )/*Timer2  OverFlow65536*12/22118400=0.035555s*/
			{
				nb_overflows++;/*溢出计数增1，
				*/
			}
			if ( EXF2==1 )/*P1.1  产生负跳变*/
			{
				 ucSamples++;/*采样计数增1*/
				 nb_overflows_old +=nb_overflows;/*保存溢出计数*/
				 nb_overflows=0x0000;/*清除溢出计数*/
				 
				 if(ucSamples==1){
				 	nb_overflows_old=0;			 
				 	RCAP2_old = RCAP2H;
				 	RCAP2_old=(RCAP2_old<<8)&0xFF00;
				 	RCAP2_old |= RCAP2L;
				 }else if(ucSamples==7){
				 	EXEN2=0;				 	
				 }
			}
			EXF2=0;/*清除Timer2  外部事件标志*/
			TF2 = 0;/*清除Timer2  溢出标志*/
		}
		else if(capture==T2SpringDischarge_DingWei)
		{
			if ( EXF2==1)/*P1.1  产生负跳变*/
			{
				CNL=0;			       	  //能量反馈打开有效
				count50us=0;			  //放电计时清零
				onecheck_flag=1;		  //反馈只采集一次标志
				Pulse_time_flag=1;		  //放电标志
				TL0 = (65536-92)%256;     //系统时钟12分频 作为定时器0时钟源	           
				TH0 = (65536-92)/256;     //精度可以满足   此定时为50us，准确数为49.945us	  精度1%
// 				if(Select_Laser==ALight){								       
// 					adstart(1);  //a路电压检测   
// 			    }else if(Select_Laser == BLight){
// 					adstart(2);  //b路电压检测 
// 				}
//			Delay(200);				//5ms延时
				PULSE=1;	  //放电前沿
				ET0 = 1;
		    TR0 = 1;/*启动放电延时*/

				TR2 = 0;  //关闭定时器2，防止干扰再次触发放电
				ET2 = 0;										
				EXEN2=0;			//禁止引脚捕获功能	
				Clamp_flag=1;

				
			}
			if ( TF2==1 )/*Timer2  OverFlow*/
			{
				ucDischangingDianJi_T2overflows++;
			}
			EXF2 = 0;
			TF2 = 0;
		}
	}
	else                    //电机故障检测
	{
		if ( TF2==1 )/*Timer2  OverFlow:0.0355555s*/
		{
			uiT2OverFlows++;
		}
		if ( EXF2==1 )/*P1.1  产生负跳变*/
		{
			uiT2OverFlows = 0x00;
		}
		EXF2=0;/*清除Timer2  外部事件标志*/
		TF2 = 0;/*清除Timer2  溢出标志*/
	}
}


void T3_int (void) interrupt 14
{
  TMR3CN   &= 0x7F;
  am2302_timeflg++;
  if(am2302_timeflg>=50){
     am2302_timeflg=0;
	 am2302flg=1;
	 if(fls_flag){
	 	if(LD_flg){
			Pwm_LD(0);
			LD_flg=0;
		}else{
			Pwm_LD(pwm_ld_data);
			LD_flg=1;
		}
	}
	 
  }
  if(Clamp_flag){
	 CLAMP=1;
	 Clamp_flag=0;

  }else{
	 CLAMP=0;
  }

}





























