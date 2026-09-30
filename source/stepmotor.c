

#include "include\includes.h"

uchar linshihi;

/*电机相关子程序*/

/*缓慢启动电机*/
void fnSlowStartupDrive ( )
{
	 /*启动电机*/
	PCA0CN   = 0x00;		      //停止计数器
	PCA0MD   = 0x02;          //时钟源为系统4分频，禁止溢出中断   总分频为128
	PCA0CPM0 = 0x46;		      //PCA0 配置为频率输出模式
	PCA0CPH0 = 216;		        //1HZ启动
	PCA0CN   = 0x40;		      //启动计数器	
	while ((PCA0CPH0-86) > 0)	//每次减小一个数值  频率上升至2.5hz
	{
		PCA0CPH0--;
		Delay (2000);	//20ms
	}

}


/*根据上次频率调整驱动电压*/
void fnGenJuShangCiPinLvTiaoZheng (uchar ucFset)
{
  if(one_orflag==0){	//多腔
	if(ucFset==27)
		ucFset=26;
	else if(ucFset==33)
		ucFset=32;
	else if(ucFset==45)
		ucFset=44;
	else if(ucFset==50)
		ucFset=48;
	else if(ucFset==55)
		ucFset=56;

		if(double_pluse_flag){ //双脉冲模式，
			if(ucFset>=6)
				 ucFset=ucFset/2; 
		}else{
			if(ucFset>=14)
				 ucFset=ucFset/2; 
		}
	
	 
  }
// 	else{ //单腔模式  
// 		if(double_pluse_flag){ //双脉冲模式，
// 			
// 				 ucFset=ucFset*2; 
// 		}
// 	}
	linshihi=ucFset;
	
	if(ucFset==3){
		Pre_CPH0 = dj_1_5Frequency;
	}else if(ucFset==4){
		Pre_CPH0  = dj_2Frequency;
	}else if(ucFset==5){
		Pre_CPH0 = dj_2_5Frequency;
	}else if(ucFset==6){
		Pre_CPH0  = dj_3Frequency;
	}else if(ucFset==7){
		Pre_CPH0  = dj_3_5Frequency;
	}else if(ucFset==8){
		Pre_CPH0  = dj_4Frequency;
	}else if(ucFset==9){
		Pre_CPH0  = dj_4_5Frequency;
	}else if(ucFset==10){
		Pre_CPH0  = dj_5Frequency;
	}else if(ucFset==11){
		Pre_CPH0  = dj_5_5Frequency;
	}else if(ucFset==12){
		Pre_CPH0  = dj_6Frequency;
	}else if(ucFset==13){
		Pre_CPH0  = dj_6_5Frequency;
	}else if(ucFset==14){
		Pre_CPH0  = dj_7Frequency;
	}else if(ucFset==15){
		Pre_CPH0  = dj_7_5Frequency;
	}else if(ucFset==16){
		Pre_CPH0  = dj_8Frequency;
	}else if(ucFset==17){
		Pre_CPH0  = dj_8_5Frequency;
	}else if(ucFset==18){
		Pre_CPH0  = dj_9Frequency;
	}else if(ucFset==19){
		Pre_CPH0  = dj_9_5Frequency;
	}else if(ucFset==20){
		Pre_CPH0  = dj_10Frequency;
	}else if(ucFset==22){
		Pre_CPH0  = dj_11Frequency;
	}else if(ucFset==24){
		Pre_CPH0  = dj_12Frequency;
	}else if(ucFset==26){
		Pre_CPH0  = dj_13Frequency;
	}else if(ucFset==28){
		Pre_CPH0  = dj_14Frequency;
	}else if(ucFset==30){
		Pre_CPH0  = dj_15Frequency;
	}
	fnSlowlyAddDriveVolt ();
	SetFrequency_last=linshihi;

	return;
}




/*缓慢增加、减小电机驱动频率*/
void fnSlowlyAddDriveVolt ( )
{
  if ( SetFrequency_last < 7 && linshihi >= 7 )			   //分频模式切换，由128改为32分频，
  {
	 while ( (PCA0CPH0-62)>0 )	   //每次减小一个数值  频率上升至3.5hz
	{
		PCA0CPH0--;
		Delay (2000);	//20ms
	}

	PCA0CN    = 0x00;		//停止计数器		  改变分频模式，保持3.5hz
	PCA0MD = 0x08;         //时钟源为系统1分频，禁止溢出中断   总分频为32
    PCA0CPM0  = 0x46;		  //配置为频率输出模式 
    PCA0CPH0  = dj_3_5Frequency; //电机频率为3.5HZ
    PCA0CN    = 0x40;		//启动计数器


  }else if ( SetFrequency_last >= 7 && linshihi < 7 )		//分频模式切换，由32改为128分频，
  {
	while ( (dj_3_5Frequency-PCA0CPH0)>0 )	   //每次增加一个数值  频率降低至3.5hz
	{
			PCA0CPH0++;
			Delay (1000);	//10ms
	}
	PCA0CN    = 0x00;		//停止计数器
	PCA0MD = 0x02;         //时钟源为系统4分频，禁止溢出中断   总分频为128
	PCA0CPM0  = 0x46;		  //PCA0 配置为频率输出模式
	PCA0CPH0  = 62;		   //3.5HZ启动
	PCA0CN    = 0x40;		//启动计数器	


  }else
  {

  }
	if ( PCA0CPH0>=Pre_CPH0 )			   //驱动频率增加时		 频率越高PCA0CPH0里的数值越小
	{

		while ( (PCA0CPH0-Pre_CPH0)>0 )	   //每次减小一个数值
		{
			PCA0CPH0--;
			Delay (2000);	//20ms
		}
	}else if ( Pre_CPH0>PCA0CPH0 )			   //驱动频率减小时
	{

		while ( (Pre_CPH0-PCA0CPH0)>0 )	   //每次增加一个数值
		{
			PCA0CPH0++;
			Delay (1000);	//10ms
		}
	}
 

	return;
}




uint fnMeasureCycle( uchar ucChoiceedge )
{
//	uchar ucMeasurecount;/*周期测量次数*/
	ulong ulCyclecount;/*存储电机周期计数值*/
//	bit bCountcycle;/*计算周期使能位*/

	if ( ucChoiceedge==Adown )/*通光位置*/
	{
	   CA = 0;
	   CB = 1;	/*取单路沿,上升沿*/
		
	}
	else if ( ucChoiceedge==Bup )/*不通光位置*/
		{
		   CA = 1;
		   CB = 0;	/*取单路沿,下降沿*/
		}
		else if ( ucChoiceedge==ABupdown )
			{
			CA = 1;
		    CB = 1;	/*取双路沿,上升沿/下降沿*/
			}
			else
			{
			  CA = 1;
		      CB = 1;	/*取双路沿,上升沿/下降沿*/
			}

	/*初始化--测速功能，标志变量*/
	#if 1
		biChoiceT2Fun = T2Fun1;
		capture = T2MeasureSpeed;
		ulCyclecount = 0;
		ucSamples = StartSample;
		T2CON = 0x09;/*捕获*/
		ET2 = 1;
		TR2 = 1;
	#endif

	while (1)
	{
		if (ucSamples==7)
		{ 
			RCAP2 =  RCAP2H;
			RCAP2 =(RCAP2<<8)&0xFF00;
			RCAP2 |= RCAP2L;
			ulCyclecount =RCAP2 + 0x10000 * nb_overflows_old - RCAP2_old;
			ulCyclecount = ulCyclecount/6;    //
			ulCyclecount = ulCyclecount*100/18432; //0.5425us 的时钟周期有多少个100us 
			EXEN2=1;
			ET2 = 0;
			TR2 = 0;
			return ulCyclecount;
		}
		if (nb_overflows>30)/*这个条件在周期测量函数中是合适的，周期大于0.03555*20=0.7s
			,显然比5hz低得多*/
		{
			if (Flag_Flow==Flag_ZhengChang)
			{
				halt();/*故障处理*/
				SysFault(12);
				while (1)
				{
					Alarm(sound3);
					Delay_5s();
				}
			}else{
				/*清除溢出标志*/
				nb_overflows = 0;
				/*暂时停用Timer2*/

				ET2 = 0;
				TR2 = 0;
				LCD_DianJi_TingZhuanorTaiMan();
				
			}
//				Alarm(sound2);
				return 0;/*返回ulCyclecount = 0*/
		}
	}
}




/*步进电机监测调整程序*/  
void fnDianJiCheckAdjust(uchar Fset)
{
	ulong ulCycleCount;//电机转速周期计数平均值

  if(one_orflag==0){	//多腔
    if(Fset==27)
    	Fset=26;
    else if(Fset==33)
    	Fset=32;
    else if(Fset==45)
    	Fset=44;
    else if(Fset==50)
    	Fset=48;
    else if(Fset==55)
    	Fset=56;
    
    if(double_pluse_flag){ //双脉冲模式，
    	if(Fset>=6)
    		 Fset=Fset/2; 
    }else{
    	if(Fset>=14)
    		 Fset=Fset/2; 
    }
		
  }
// 	else{ //单腔模式  
// 		if(double_pluse_flag){ //双脉冲模式
// 			
// 				 Fset=Fset*2; 
// 		}
// 	}

	while (1)
	{
		//测量转速(周期检测法，算术平均值N=6)
		ulCycleCount = fnMeasureCycle(Adown);

		//可能需要修改，0.01 改为0.03
		if (ulCycleCount>=10000/(ulong)Fset+(ulong)((float)(10000/(ulong)Fset)*FrequencyError))  //
		{
			PCA0CPH0--;
			Delay (2000);	//20ms
	
		}
		else if (ulCycleCount<=10000/(ulong)Fset-(ulong)((float)(10000/(ulong)Fset)*FrequencyError))
		{
			PCA0CPH0++;
			Delay (1000);	//10ms
				
		}							  	   
		
		else//满足频率相对误差< 3%
		{
				
				return;	
		}

		if ( PCA0CPH0>=Pre_CPH0*Pca0_Error_p ||PCA0CPH0<=Pre_CPH0*Pca0_Error_n )   //	 PCA0CPH0的值超差10%则产生故障报警，有待验证
		{
			//故障处理
			PCA0CN    = 0x00; //电机停转
      		SysFault(11);
			while (1)
			{
				Alarm(sound3);
				Delay_5s();
			}				    
		}
		else
		{
					
		}

	}
}







