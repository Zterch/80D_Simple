
#include "include\includes.h"



/*能量检测子程序*/

void fnEnergyControl_ZhunBei_first(void)
{
	uint uiVolt_F;/*反馈电压*/
	uiVolt_F = 0;
	/*4 次测量实际输出能量，并取平均*/
	uiVolt_F = EnerVoltJZ();
	/*能量反馈电压浮点化，并取10 倍*/
	Ener_S_first = 0.0117*(float)uiVolt_F;  //  uiVolt_F/4096*2.4*10*2   结果为两位数 如：30，存的曲线是读的反馈的2倍
}



#if 1
/*准备流程----	能量控制方案*/
bool fnEnergyControl_ZhunBei ( void )
{
  //	float temp;/*能量超差*/
	uint uiVolt_F;/*反馈电压*/
	float Ener_F;/*反馈能量*/
	uchar SetEnergy_fankui;

	uiVolt_F = 0;

	if( double_pluse_flag )
	{
		/*4 次测量实际输出能量，并取平均*/
		uiVolt_F = EnerVoltJZ();
		/*能量反馈电压浮点化，并取10 倍*/
		Ener_S_second = 0.0117*(float)uiVolt_F;  //  uiVolt_F/4096*2.4*10*2   结果为两位数 如：30，存的曲线是读的反馈的2倍		
		Ener_F = Ener_S_second+Ener_S_first;
	}else{
		Ener_F = Ener_S_first;
	}


  	if(JT_L_R==Left)
	{
	  if(Select_Laser == ALight){
			Ener_S = Ener_Sleft_A;
		}else{
			Ener_S = Ener_Sleft_B;
		}
		
	  SetEnergy_fankui = SetEnergy_left;
	}
	else if(JT_L_R==Right)
	{
	  if(Select_Laser == ALight){
			Ener_S = Ener_Sright_A;
		}else{
			Ener_S = Ener_Sright_B;
		}
	  SetEnergy_fankui = SetEnergy_right;
	}

	if(Select_Laser == ALight){
		if(Ener_F>Ener_A_MAX&&Ener_F<=Ener_S){
		   Ener_A_MAX=Ener_F;
		   uiChangVolt_A_MAX=uiChangVolt_A;
		}
	}else if(Select_Laser == BLight){
		if(Ener_F>Ener_B_MAX&&Ener_F<=Ener_S){
		   Ener_B_MAX=Ener_F;
		   uiChangVolt_B_MAX=uiChangVolt_B;
		}
	}

	if(SetEnergy_fankui==3)
	  {
		 uiChangVolt_A = 530;
		 uiChangVolt_B = 530;
	  }else{
	
	if( Ener_F>Ener_S )/*能量超差大于20%*/
   	{
    	if(Ener_F>=(Ener_S*1.2))
		/*能量超过20%*/
    	{
    		 if(Select_Laser == ALight)
    		 {
    				uiChangVolt_A = uiChangVolt_A - 5;/*-5V*/
    		 }
			   else if(Select_Laser == BLight)
			   {
				    uiChangVolt_B = uiChangVolt_B - 5;/*-5V*/
			   }
			 /*充电电压<VcminV，报警70*/
			if (uiChangVolt_A<Vcmin||uiChangVolt_B<Vcmin)
			{
				halt();
				SysFault(70);
				while (1)
				{
					Alarm(sound3);
					Delay_5s();
				}
			}
    	}
    	else if (Ener_F>=(Ener_S*1.1)&&Ener_F<(Ener_S*1.2))/*[+10%,+20%)*/
        {
    		 if(Select_Laser == ALight)
    		 {
    				uiChangVolt_A = uiChangVolt_A - 2;/*-2V*/
    		 }
			   else if(Select_Laser == BLight)
			   {
				    uiChangVolt_B = uiChangVolt_B - 2;/*-2V*/
			   }
			 /*充电电压<VcminV，报警70*/
			if (uiChangVolt_A<Vcmin||uiChangVolt_B<Vcmin)
			{
				halt();
				SysFault(71);
				while (1)
				{
					Alarm(sound3);
					Delay_5s();
				}
			}
    	}
		else if (Ener_F>=(Ener_S*1.03)&&Ener_F<(Ener_S*1.1))/*[+10%,+20%)*/
        {
    		 if(Select_Laser == ALight)
    		 {
    				uiChangVolt_A = uiChangVolt_A - 1;/*-1V*/
    		 }
			   else if(Select_Laser == BLight)
			   {
				    uiChangVolt_B = uiChangVolt_B - 1;/*-1V*/
			   }
			 /*充电电压<VcminV，报警70*/
			if (uiChangVolt_A<Vcmin||uiChangVolt_B<Vcmin)
			{
				halt();
				SysFault(72);
				while (1)
				{
					Alarm(sound3);
					Delay_5s();
				}
			}
    	}
   	}
  	else
{
   		if (Ener_F>(Ener_S*0.9)&&Ener_F<(Ener_S*0.97))/*(-10%,-3%)*/
		{
			   if(Select_Laser == ALight)
    		   {
    				uiChangVolt_A = uiChangVolt_A + 1;/*-1V*/
    		   }
			   else if(Select_Laser == BLight)
			   {
				    uiChangVolt_B = uiChangVolt_B + 1;/*-1V*/
			   }
			/*调整后充电电压>Vcmax，则充电电压固定为Vcmax*/
			
			if (uiChangVolt_A>Vcmax)
						{
							uiChangVolt_A = uiChangVolt_A_MAX;
							Tiaojie_A =1;
						}
			if (uiChangVolt_B>Vcmax)
						{
						uiChangVolt_B = uiChangVolt_B_MAX;
						Tiaojie_B =1;
						}
			if(Tiaojie_A ==1){
				uiChangVolt_A = uiChangVolt_A_MAX;
			}
			if(Tiaojie_B ==1){
				uiChangVolt_B = uiChangVolt_B_MAX;
			}
			
		}
		else if (Ener_F>(Ener_S*0.8)&&Ener_F<(Ener_S*0.9))/*(-20%,-10%)*/
		{
			   if(Select_Laser == ALight)
    		   {
    				uiChangVolt_A = uiChangVolt_A + 2;/*-2V*/
    		   }
			   else if(Select_Laser == BLight)
			   {
				    uiChangVolt_B = uiChangVolt_B + 2;/*-2V*/
			   }
			/*调整后充电电压>Vcmax，则充电电压固定为Vcmax*/
			
			if (uiChangVolt_A>Vcmax)
						{
							uiChangVolt_A = uiChangVolt_A_MAX;
							Tiaojie_A =1;
						}
			if (uiChangVolt_B>Vcmax)
						{
						uiChangVolt_B = uiChangVolt_B_MAX;
						Tiaojie_B =1;
						}
			if(Tiaojie_A ==1){
				uiChangVolt_A = uiChangVolt_A_MAX;
			}
			if(Tiaojie_B ==1){
				uiChangVolt_B = uiChangVolt_B_MAX;
			}
		}
		else if (Ener_F>=(Ener_S*0.5)&&Ener_F<=(Ener_S*0.8))/*[-50%,-20%]*/
		{
			  if(Select_Laser == ALight)
			{    if(SetEnergy_fankui>=15)
					uiChangVolt_A = uiChangVolt_A + 10;/*+10V*/
				else
					uiChangVolt_A = uiChangVolt_A + 3;/*+3V*/
			}
			else if(Select_Laser == BLight)
			{   if(SetEnergy_fankui>=15)
					uiChangVolt_B = uiChangVolt_B + 10;/*+10V*/
			    else
					uiChangVolt_B = uiChangVolt_B + 3;/*+3V*/
			}
			/*调整后充电电压>Vcmax，则充电电压固定为Vcmax*/
			
			if (uiChangVolt_A>Vcmax)
						{
							uiChangVolt_A = uiChangVolt_A_MAX;
							Tiaojie_A =1;
						}
			if (uiChangVolt_B>Vcmax)
						{
						uiChangVolt_B = uiChangVolt_B_MAX;
						Tiaojie_B =1;
						}
			if(Tiaojie_A ==1){
				uiChangVolt_A = uiChangVolt_A_MAX;
			}
			if(Tiaojie_B ==1){
				uiChangVolt_B = uiChangVolt_B_MAX;
			}
		}
		else if (Ener_F<(Ener_S*0.5))/*(-oo%,-50%)*/
		{
		  	
			if(Select_Laser == ALight)
			{    if(SetEnergy_fankui>=15)
					uiChangVolt_A = uiChangVolt_A + 25;/*+25V*/
				else
					uiChangVolt_A = uiChangVolt_A + 6;/*+6V*/
			}
			else if(Select_Laser == BLight)
			{   if(SetEnergy_fankui>=15)
					uiChangVolt_B = uiChangVolt_B + 25;/*+25V*/
			    else
					uiChangVolt_B = uiChangVolt_B + 6;/*+6V*/
			}
			
			/*调整后充电电压>Vcmax，则充电电压固定为Vcmax  	*/
			if (Tiaojie_A ==1||Tiaojie_B ==1)
			{
				Count_F50V1000++;
				if (Count_F50V1000>200)
				{
					halt();
					SysFault(73);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}							
			if (uiChangVolt_A>Vcmax)
						{
							uiChangVolt_A = uiChangVolt_A_MAX;
							Tiaojie_A =1;
						}
			if (uiChangVolt_B>Vcmax)
						{
						uiChangVolt_B = uiChangVolt_B_MAX;
						Tiaojie_B =1;
						}
			if(Tiaojie_A ==1){
				uiChangVolt_A = uiChangVolt_A_MAX;
			}
			if(Tiaojie_B ==1){
				uiChangVolt_B = uiChangVolt_B_MAX;
			}
		}
	  }
	 }
	  

	 //修改充电电压
	if(Select_Laser == ALight){
		fnDischangVolt_OutPut_uiA(uiChangVolt_A);

	}else if(Select_Laser == BLight){
		fnDischangVolt_OutPut_uiB(uiChangVolt_B);
	
	}

	return 0;
}




/*各个频率下最大可设定能量*/
void CalMaxEner_Ho(void)
{
 if(double_pluse_flag){
	switch (SetFrequency)
	{
		case 3: maxLimitSetEnergy_Ho=10;break;
		case 4: maxLimitSetEnergy_Ho=20;break;
		case 5: maxLimitSetEnergy_Ho=70;break;
		case 6: maxLimitSetEnergy_Ho=70;break;
		case 7: maxLimitSetEnergy_Ho=70;break;
		case 8: maxLimitSetEnergy_Ho=70;break;
		case 9: maxLimitSetEnergy_Ho=70;break;
		case 10: maxLimitSetEnergy_Ho=70;break;
		case 11: maxLimitSetEnergy_Ho=68;break;
		case 12: maxLimitSetEnergy_Ho=67;break;	
		case 13: maxLimitSetEnergy_Ho=62;break;				
		case 14: maxLimitSetEnergy_Ho=46;break;
		case 15: maxLimitSetEnergy_Ho=40;break;		
		case 16: maxLimitSetEnergy_Ho=38;break;
		case 17: maxLimitSetEnergy_Ho=30;break;		
		case 18: maxLimitSetEnergy_Ho=28;break;
		case 19: maxLimitSetEnergy_Ho=20;break;		
		case 20: maxLimitSetEnergy_Ho=20;break;
		case 21: maxLimitSetEnergy_Ho=19;break;		
		case 22: maxLimitSetEnergy_Ho=14;break;		
		case 23: maxLimitSetEnergy_Ho=13;break;		
		case 24: maxLimitSetEnergy_Ho=13;break;		
		case 25: maxLimitSetEnergy_Ho=12;break;		
		case 26: maxLimitSetEnergy_Ho=12;break;
		case 27: maxLimitSetEnergy_Ho=11;break;		
		case 28: maxLimitSetEnergy_Ho=8;break;		
		case 29: maxLimitSetEnergy_Ho=6;break;		
		case 30: maxLimitSetEnergy_Ho=5;break;

		default: maxLimitSetEnergy_Ho=minEner;break;
	}
 }else{
	 	switch (SetFrequency)
	{
		case 5: maxLimitSetEnergy_Ho=10;break;
		case 6: maxLimitSetEnergy_Ho=10;break;
		case 7: maxLimitSetEnergy_Ho=20;break;
		case 8: maxLimitSetEnergy_Ho=50;break;
		case 9: maxLimitSetEnergy_Ho=35;break;
		case 10: maxLimitSetEnergy_Ho=30;break;
		case 11: maxLimitSetEnergy_Ho=28;break;
		case 12: maxLimitSetEnergy_Ho=28;break;
		
		case 14: maxLimitSetEnergy_Ho=20;break;
		case 16: maxLimitSetEnergy_Ho=35;break;
		case 18: maxLimitSetEnergy_Ho=35;break;
		case 20: maxLimitSetEnergy_Ho=50;break;
		case 22: maxLimitSetEnergy_Ho=30;break;
		
		case 24: maxLimitSetEnergy_Ho=38;break;
		
		case 27: maxLimitSetEnergy_Ho=28;break;
//		case 28: maxLimitSetEnergy_Ho=25;break;
		case 30: maxLimitSetEnergy_Ho=20;break;
		case 33: maxLimitSetEnergy_Ho=20;break;
//		case 34: maxLimitSetEnergy_Ho=20;break;
		case 36: maxLimitSetEnergy_Ho=20;break;
//		case 38: maxLimitSetEnergy_Ho=20;break;
		case 40: maxLimitSetEnergy_Ho=20;break;
		case 45: maxLimitSetEnergy_Ho=5;break;
		case 50: maxLimitSetEnergy_Ho=5;break;
//		case 52: maxLimitSetEnergy_Ho=5;break;
		case 55: maxLimitSetEnergy_Ho=5;break;
		case 60: maxLimitSetEnergy_Ho=5;break;
		default: maxLimitSetEnergy_Ho=minEner;break;
	}
 }
	return;
}




/*目标值*/
#if 1
/*读取该工作点的能量电压目标值*/
void fnReadEnergyVoltage (uchar ucEne, uchar ucFre)
{
	uchar temp1;
	uchar temp2;
	uint address_save;
	if( Ener_AB_flag==0 )  //A路反馈
	{		
		if( double_pluse_flag ) //双脉冲
		{
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre<=10)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 5 ) * 2;
				}
				else if (ucFre<20)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 5 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 5 ) * 2;
				}
		  } 
			else //短脉冲
			{
				if (ucFre<=10)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 3 ) * 2;
				}
				else if (ucFre<20)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 3 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 3 ) * 2;
				}
		 }


		}else{
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre==8||ucFre==16)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 5 ) * 2;
				}
				else if (ucFre==10||ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 5 ) * 2;
				}
				else if (ucFre==24||ucFre==12)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 5 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 5 ) * 2;
				}
		  } 
			else //短脉冲
			{
				if (ucFre==8||ucFre==16)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 3 ) * 2;
				}
				else if (ucFre==10||ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 3 ) * 2;
				}
				else if (ucFre==24||ucFre==12)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 3 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 3 ) * 2;
				}
				
			}
		}
	}else   //B路反馈
	{		
		if( double_pluse_flag ) //双脉冲
		{
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre<=10)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 4 ) * 2;
				}
				else if (ucFre<20)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 4 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 4 ) * 2;
				}
		  } 
			else //短脉冲
			{
				if (ucFre<=10)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 2 ) * 2;
				}
				else if (ucFre<20)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 2 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 2 ) * 2;
				}
		 }


		}else{
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre==8||ucFre==16)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 4 ) * 2;
				}
				else if (ucFre==10||ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 4 ) * 2;
				}
				else if (ucFre==24||ucFre==12)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 4 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 4 ) * 2;
				}
		  } 
			else //短脉冲
			{
				if (ucFre==8||ucFre==16)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 2 ) * 2;
				}
				else if (ucFre==10||ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 2 ) * 2;
				}
				else if (ucFre==24||ucFre==12)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 2 ) * 2;
				}
				else
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 2 ) * 2;
				}
				
			}
		}
	}
	
	read_data(0xa0,address_save,qushuzu,1);
	Delay( 1000 );
	temp1 = qushuzu[0];
	read_data(0xa0,address_save+1,qushuzu,1);
	Delay( 1000 );
	temp2 = qushuzu[0];
	uiEner_S = 256*(uint)temp1 + (uint)temp2;
	if (uiEner_S>1000||uiEner_S==0)
	{
//		uiEner_S=50;
		if( double_pluse_flag )  //双脉冲
		{
			if(ucEne==5){
				uiEner_S=90;			
			}else if(ucEne==13){
				uiEner_S=200;			
			}else if(ucEne==21){
				uiEner_S=320;			
			}else if(ucEne==29){
				uiEner_S=410;			
			}else if(ucEne==37){
				uiEner_S=490;			
			}
		}else{
			if(ucEne==5){
				uiEner_S=50;			
			}else if(ucEne==13){
				uiEner_S=100;			
			}else if(ucEne==21){
				uiEner_S=180;			
			}else if(ucEne==29){
				uiEner_S=230;			
			}else if(ucEne==37){
				uiEner_S=260;			
			}else if(ucEne==45){
				uiEner_S=290;			
			}else if(ucEne==53){
				uiEner_S=310;			
			}
		}

	}
}

/*保存该工作点的能量电压目标值*/
void fnWriteEnergyVoltage (uchar ucEne, uchar ucFre)
{
	uint address_save;
	if( Ener_AB_flag==0 )  //A路反馈
	{		
		if( double_pluse_flag )  //双脉冲
		{
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre==4)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 5 ) * 2;
				}
				else if (ucFre==14)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 5 ) * 2;
				}
				else if (ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 5 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}
			else{    //短脉冲
				if (ucFre==4)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 3 ) * 2;
				}
				else if (ucFre==14)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 3 ) * 2;
				}
				else if (ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 3 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}

		}else{    //单脉冲
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre==8)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 5 ) * 2;
				}
				else if (ucFre==10)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 5 ) * 2;
				}
				else if (ucFre==24)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 5 ) * 2;
				}
				else if(ucFre==99||!(ucFre==8||ucFre==10||ucFre==24))
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 5 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}
			else{    //短脉冲
				if (ucFre==8)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 3 ) * 2;
				}
				else if (ucFre==10)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 3 ) * 2;
				}
				else if (ucFre==24)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 3 ) * 2;
				}
				else if(ucFre==99||!(ucFre==8||ucFre==10||ucFre==24))
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 3 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}
			
		}
  }else  //B路反馈
	{		
		if( double_pluse_flag )    //双脉冲
		{
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre==4)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 4 ) * 2;
				}
				else if (ucFre==14)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 4 ) * 2;
				}
				else if (ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 4 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}
			else{    //短脉冲
				if (ucFre==4)
				{
					address_save = Address_BaseEnergyVoltage_03Hz_D + ( ucEne - 2 ) * 2;
				}
				else if (ucFre==14)
				{
					address_save = Address_BaseEnergyVoltage_06Hz_D + ( ucEne - 2 ) * 2;
				}
				else if (ucFre==20)
				{
					address_save = Address_BaseEnergyVoltage_10Hz_D + ( ucEne - 2 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}

		}else{
			if( long_short_pluse_flag ) //长脉冲
			{
				if (ucFre==8)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 4 ) * 2;
				}
				else if (ucFre==10)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 4 ) * 2;
				}
				else if (ucFre==24)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 4 ) * 2;
				}
				else if(ucFre==99||!(ucFre==8||ucFre==10||ucFre==24))
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 4 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}
			else{    //短脉冲
				if (ucFre==8)
				{
					address_save = Address_BaseEnergyVoltage_08Hz + ( ucEne - 2 ) * 2;
				}
				else if (ucFre==10)
				{
					address_save = Address_BaseEnergyVoltage_10Hz + ( ucEne - 2 ) * 2;
				}
				else if (ucFre==24)
				{
					address_save = Address_BaseEnergyVoltage_24Hz + ( ucEne - 2 ) * 2;
				}
				else if(ucFre==99||!(ucFre==8||ucFre==10||ucFre==24))
				{
					address_save = Address_BaseEnergyVoltage_99Hz + ( ucEne - 2 ) * 2;
				}
				else
				{
					SysFault(61);
					while (1)
					{
						Alarm(sound3);
						Delay_5s();
					}
				}
			}
			
		}
  }
	
	saveshuzu[0]=uiEner_S/256;;
	write_data(0xa0,address_save,saveshuzu,1);
	Delay( 1000 );
	saveshuzu[0]=uiEner_S%256;;
	write_data(0xa0,address_save+1,saveshuzu,1);
	Delay( 1000 );
}
#endif

uint EnerVoltJZ(void)
{/*1.5ms*/
	uint sumavgEnergy;/*能量累加值*/	
	uchar i;
	sumavgEnergy = 0;
	for (i=0;i<4;i++)
	{
		adstart(0);	  //启动ADC能量检测
    while(adc0_flag==0);
    adc0_flag=0;
    sumavgEnergy += oneEnergy;
	}
	sumavgEnergy = sumavgEnergy>>2;
	return sumavgEnergy;
}

void ADC0_Init (void)
{
	AMX0CF   = 0x00;	 					//单端方式
	AMX0SL   = 0x00;						//AIN0
	REF0CN  |= 0x03;            //内部基准 使用vef0
	ADC0CF   = 0x40;            //系统时钟10分频作为adc0的时钟源  无增益  可能会有问题 输入电压最大不应超过3.3v
	ADC0CN   = 0xC0;            //使能，定义写ad0busy=1开始转换 右对齐
	EIE2    |= 0x02;            //使能中断	  
}

 void adstart(unsigned char k)
{

  adc_select=k;
  switch(k)
  {
   
  case 0:
  	  AMX0SL=0x00;  //AIN0
   ADC0CN=ADC0CN|0x10;  //star AD converte
   break;
   case 1:				
	  AMX0SL=0x01;  //AIN2
	  ADC0CN=ADC0CN|0x10;  //star AD converte    
   break;
   case 2:
	  AMX0SL=0x02;  //AIN2
	  ADC0CN=ADC0CN|0x10;  //star AD converte
   break;
    default:	
     break;
   }
} 


void ADC0_ISR (void) interrupt 15
{
 	                    

   AD0INT = 0;						   // 清 ADC 中断标志位
  if(adc_select==0){
   oneEnergy = ADC0H;						   // 读取ADC0H数据，此处演示，仅取高8位
   oneEnergy <<= 8;
   oneEnergy |= ADC0L;
   adc0_flag=1;
  }else if(adc_select==1){
	detect_VA = ADC0H;						   // 读取ADC0H数据，此处演示，仅取高8位
   detect_VA <<= 8;
   detect_VA |= ADC0L;
   detect_flag=1;
  }else if(adc_select==2){
	detect_VB = ADC0H;						   // 读取ADC0H数据，此处演示，仅取高8位
   detect_VB <<= 8;
   detect_VB |= ADC0L;
   detect_flag=1;
  }
   
}

void Power_FeedBack(uchar SetEnergy_data)
{
  	
		if( double_pluse_flag )
		{
			SetEnergy_data=SetEnergy_data/2;
		}
		
		Ener_AB_flag = 0;			 //A路
		if (SetEnergy_data==5||SetEnergy_data==13||SetEnergy_data==21||SetEnergy_data==29||SetEnergy_data==37||SetEnergy_data==45)
		{
			fnReadEnergyVoltage(SetEnergy_data, SetFrequency);
			Ener_SA_temp  = (float)uiEner_S;
			Ener_SA_temp = Ener_SA_temp/10.0;
		}
		else
		{
							/*2个变量复用说明*/
							/*Power		存储低端能量反馈电压*/
							/*uiEner_S	存储高端能量反馈电压*/
			if (SetEnergy_data>EnerStep05&&SetEnergy_data<EnerStep13)
			{
				fnReadEnergyVoltage(EnerStep05, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep13, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep05)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep13&&SetEnergy_data<EnerStep21)
			{
				fnReadEnergyVoltage(EnerStep13, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep21, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep13)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep21&&SetEnergy_data<EnerStep29)
			{
				fnReadEnergyVoltage(EnerStep21, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep29, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep21)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep29&&SetEnergy_data<EnerStep37)//
			{
				fnReadEnergyVoltage(EnerStep29, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep37, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep29)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep37&&SetEnergy_data<EnerStep45)//
			{
				fnReadEnergyVoltage(EnerStep37, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep45, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep37)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep45)//
			{
				fnReadEnergyVoltage(EnerStep45, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep53, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep45)*uiEner_S;
			}
			else if(SetEnergy_data>0&&SetEnergy_data<EnerStep05)
			{
				fnReadEnergyVoltage(EnerStep05, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep13, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power - (uint)(EnerStep05-SetEnergy_data)*uiEner_S;
			}
			Ener_SA_temp  = (float)uiEner_S;
			Ener_SA_temp = Ener_SA_temp/10.0;
		}
		
		Ener_AB_flag = 1;	    //B路
		if (SetEnergy_data==5||SetEnergy_data==13||SetEnergy_data==21||SetEnergy_data==29||SetEnergy_data==37||SetEnergy_data==45)
		{
			fnReadEnergyVoltage(SetEnergy_data, SetFrequency);
			Ener_SB_temp  = (float)uiEner_S;
			Ener_SB_temp = Ener_SB_temp/10.0;
		}
		else
		{
							/*2个变量复用说明*/
							/*Power		存储低端能量反馈电压*/
							/*uiEner_S	存储高端能量反馈电压*/
			if (SetEnergy_data>EnerStep05&&SetEnergy_data<EnerStep13)
			{
				fnReadEnergyVoltage(EnerStep05, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep13, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep05)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep13&&SetEnergy_data<EnerStep21)
			{
				fnReadEnergyVoltage(EnerStep13, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep21, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep13)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep21&&SetEnergy_data<EnerStep29)
			{
				fnReadEnergyVoltage(EnerStep21, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep29, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep21)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep29&&SetEnergy_data<EnerStep37)//
			{
				fnReadEnergyVoltage(EnerStep29, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep37, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep29)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep37&&SetEnergy_data<EnerStep45)//
			{
				fnReadEnergyVoltage(EnerStep37, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep45, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep37)*uiEner_S;
			}
			else if (SetEnergy_data>EnerStep45)//
			{
				fnReadEnergyVoltage(EnerStep45, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep53, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power + (uint)(SetEnergy_data-EnerStep45)*uiEner_S;
			}
			else if(SetEnergy_data>0&&SetEnergy_data<EnerStep05)
			{
				fnReadEnergyVoltage(EnerStep05, SetFrequency);
				Power = uiEner_S;
				fnReadEnergyVoltage(EnerStep13, SetFrequency);
				uiEner_S = (uiEner_S-Power)>>3;
				uiEner_S = Power - (uint)(EnerStep05-SetEnergy_data)*uiEner_S;
			}
			Ener_SB_temp  = (float)uiEner_S;
			Ener_SB_temp = Ener_SB_temp/10.0;
		}
		

		
}



#endif














