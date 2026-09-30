#define uchar unsigned char 
#define uint  unsigned int 
#define ulong unsigned long
#define bool bit


//系统参数定义

#define SYSCLK 22118400
#define BAUDRATE 115200
/*触摸屏串口变量*/
#define rinterrupt_Yes 0xFF
#define rinterrupt_No 0x00
extern uchar idata rinterrupt;/*触摸屏按键接收完成标志*/
extern uchar idata rbuf[13];/*串口接收字符串数组*/
extern uchar idata rinterrupt_ld;/*触摸屏按键接收完成标志*/

//电机驱动
#define dj_1_5Frequency  144    //电机转速1.5hz
#define dj_2Frequency    108    //电机转速2hz
#define dj_2_5Frequency  86    //电机转速2.5hz
#define dj_3Frequency    72    //电机转速3hz

#define dj_3_5Frequency  247    //电机转速3.5hz
#define dj_4Frequency    216    //电机转速4hz
#define dj_4_5Frequency  192    //电机转速4.5hz
#define dj_5Frequency  173    //电机转速5hz
#define dj_5_5Frequency  157    //电机转速5.5hz
#define dj_6Frequency  144    //电机转速6hz
#define dj_6_5Frequency  133    //电机转速6.5hz
#define dj_7Frequency  123    //电机转速7hz
#define dj_7_5Frequency  115    //电机转速7.5hz
#define dj_8Frequency  108    //电机转速8hz
#define dj_8_5Frequency  102    //电机转速8.5hz
#define dj_9Frequency  96    //电机转速9hz
#define dj_9_5Frequency  91    //电机转速9.5hz
#define dj_10Frequency  86   //电机转速10hz
#define dj_11Frequency  78    //电机转速11hz
#define dj_12Frequency  72    //电机转速12hz
#define dj_13Frequency  67    //电机转速13hz
#define dj_14Frequency  62    //电机转速14hz
#define dj_15Frequency  58    //电机转速15hz
extern uchar Pre_CPH0;

extern uchar ucDianJiErrorTimes; 
#define DianJiErrorTimes 10
#define FrequencyError 0.03 /*调整范围0.01~0.1*/
#define Pca0_Error_p  1.1 /*调节最大值，超出则警报*/
#define Pca0_Error_n  0.9 /*调节最小值，超出则警报*/



//放电脉宽
extern uchar Pulse_time;
extern uchar Pulse_time2;
#define Address_Pulse_time 900 
extern uchar Pulse_time_left;
extern uchar Pulse_time_right;

extern uchar Pulse_jiange;


//射频识别
#define rinterrupt_Yesrf 0xFF
#define rinterrupt_Norf 0x00
extern uchar idata rinterruptrf;/*rf接收完成标志*/
extern uchar rbufrf[30];/*串口rf接收字符串数组*/
extern uchar data_rf[20];
extern bit g_bReceiveOK;
extern bit RF_writeflag;
extern bit T_2s_flag;
extern uint rf_jishu;
extern uchar xdata lot_data[4];		//序列号

extern unsigned char COMM_MIFARE1_PCD_SELECT_[];
#define Address_gx_info_first  7040	  //光纤历史信息的初始位置
#define Address_gx_info_weizhi 7840   
#define Address_guangxian_shouming 7846
extern uchar xdata gx_info_read[8];
extern uchar xdata gx_youxiao;
extern uchar xdata gx_weizhi_temp;
extern uchar xdata gx_weizhi;
extern uint xdata gx_jiaoyan;

extern uchar RF_errojishu;
extern uchar RF_errojishu2;
extern uchar RF_errojishu3;
extern uchar RF_errojishu4;
extern float Totalminutes_benci;
extern uchar guangxian_shouming;
extern uchar gx_ener_restrict;

extern uchar MB_SGX_jishu;
#define Address_MB_SGX_jishu 7847


//设备保养
extern bit baoyang_jishu_flag;
extern uint baoyang_jishu;
extern uint baoyang_cankao;
#define Address_baoyang_jishu  7842	  //保养计数信息位置
#define Address_baoyang_cankao 7844   //保养参考信息位置



//AM2302
extern uint humidity_SW;
extern uint temperature_SW;
extern uint humidity_HJ;
extern uint temperature_HJ;
extern uchar am2302_timeflg;
extern bit am2302flg;
extern uchar shuiwen_chuli;
extern uchar shuiwen_chuli_old;

//指示光 
extern bit LD_flg;	
extern uint pwm_ld_data;
extern uint pwm_ld_data_temp;
#define Address_pwm_ld_data 910 
//PWM风扇调速
extern uint pwm_fs_data;
// ad采集充电电压
extern uchar adc_select;
extern bit detect_flag;  
extern uint detect_VB;
extern uint detect_VA;


//系统检测信号  P4~P7  
extern uchar bdata P4status;
extern uchar bdata P5status;
extern uchar bdata P6status;


extern bit SDWA;
extern bit SGWA;
extern bit SDWB;
extern bit SGWB;
extern bit SGX;
extern bit SLS;
extern bit SJTA0;
extern bit SJTA1;

extern bit COVER;
extern bit SYM;
extern bit SLQSL_A;
extern bit SLQSL_B;
extern bit KAIHUAN;


extern bit SYR_B;
extern bit SYR_A;
extern bit SDY_B;
extern bit SDY_A;
extern bit SWGZ0;



//故障屏蔽
#define Address_ucMaskByte 809
extern uchar bdata ucMaskByte;
extern bit MB_SLQSL_AB;/*水流AB*/
extern bit MB_SYM;/*水位*/
extern bit MB_ST_YC;/*水温预警,超高*/
extern bit MB_SYR_AB;/*预燃AB*/
extern bit MB_SDY_AB;/*电源AB*/
extern bit MB_SGZ_ABW;/*三个光闸*/
extern bit MB_SLS;/*门联锁*/
extern bit MB_SGX;/*光纤*/
#define Check_Enable 1
#define Check_Disable 0


//首腔选择
extern uchar ucChoiceABlight;/*0--A light		1--B light*//*通过读取AT24C16  附值*/
#define ALight 0x00/*通光位置*/
#define BLight 0x01/*不通光位置*/
#define Address_ucChoiceABlight 808
extern uchar Select_Laser;    //标志发射激光的腔

/*显示反馈电压*/
#define Show_Enable 1
#define Show_Disable 0
#define Address_ShowFeedBackVoltage 814 
#define ShowFeedBack Show_Disable
extern bit Flag_ShowFeedBack;

//总脉冲总能量
extern ulong TotalPulse_U;/*总能量，周期ms*/
#define Address_TP_U_HH 800
#define Address_TP_U_HL 801
#define Address_TP_U_LH 802
#define Address_TP_U_LL 803
#define TP_U_AutoClear 1000000000//ys
extern ulong TotalJoule_U;
#define Address_TJ_U_HH 804
#define Address_TJ_U_HL 805
#define Address_TJ_U_LH 806
#define Address_TJ_U_LL 807

extern ulong TotalJoule_benci;
extern ulong TotalPulse_benci;
extern float Totalminutes_benci;
extern ulong TotalJoule_benci_1;


//频率
extern uchar SetFrequency;
#define AddressFreq_left 810	/*频率存储地址*/
extern uchar SetFrequency_right;
#define AddressFreq_right 812	/*频率存储地址*/
extern uchar SetFrequency_last;
#define maxFreq 60
#define minFreq 5
#define LimitFrequency 14

extern uchar SetFrequency_double;
extern uchar SetFrequency_only;
#define AddressFreq_left_double 815	/*频率存储地址*/



//能量
extern uchar SetEnergy_left;
#define AddressEner_left 811	/*能量存储地址*/
extern uchar SetEnergy_right;
#define AddressEner_right 813	/*能量存储地址*/
#define maxEner 70
#define minEner 5

extern uchar SetEnergy_left_double;
extern uchar SetEnergy_left_only;
#define AddressEner_left_double 816	/*能量存储地址*/

extern uchar SetEnergy_right_double;
extern uchar SetEnergy_right_only;
#define AddressEner_right_double 817	/*能量存储地址*/

//电压限制
#define Vcmin 450	   //450v		
#define Vcmax 800	  //800v
#define Vc0   500	  //500v   统一启动电压


//反馈曲线
#define EnerStep05 5
#define EnerStep13 13
#define EnerStep21 21
#define EnerStep29 29
#define EnerStep37 37
#define EnerStep45 45
#define EnerStep53 53

#define Address_BaseEnergyVoltage_08Hz 350
#define Address_BaseEnergyVoltage_10Hz 462
#define Address_BaseEnergyVoltage_24Hz 574
#define Address_BaseEnergyVoltage_99Hz 686

#define Address_BaseEnergyVoltage_03Hz_D 20
#define Address_BaseEnergyVoltage_06Hz_D 100
#define Address_BaseEnergyVoltage_10Hz_D 180
#define Address_BaseEnergyVoltage_15Hz_D 260



//语音地址，共8  条
#define sound7 0x00 /*按键有效*/					/*0x00,  0*/
#define sound3 0x02 /*提示音*/					/*0x20,  32*/
#define sound2 0x03 /*恢复工作/进行下一步*//*0x30,  48*/

#define sound5 0x01 /*按键无效*/					/*0x10,  16*/
#define sound1 0x04 /*故障音*/					/*0x40,  64*/
#define sound4 0x06 /*保留*/						/*0x60,  96*/
#define sound6 0x05 /*保留*/						/*0x50,  80*/
#define sound8 0x07 /*保留*/						/*0x70,  112*/


#define GZ_Open   0  /*控制光闸开*/
#define GZ_Close  1  /*控制光闸关*/
#define YR_Open   0  /*控制预燃开*/
#define YR_Close  1  /*控制预燃关*/
#define JDQ_Open  0  /*控制继电器开*/
#define JDQ_Close 1  /*控制继电器关*/

extern bit gzall_flag;
extern bit yr_flag;	
extern bit yra_flag; 
extern bit yrb_flag; 
extern bit jdq_flag;
extern bit onf_flag; 
extern bit fls_flag;
extern bit adc0_flag;
extern bit one_orflag;
extern bit fs_flag; 
extern bit jsy_flag;
extern bit double_pluse_flag;
extern bit long_short_pluse_flag;

/*以下是故障信号状态位，一般不修改*/
#define SLQSL_expedite 1/*水流通畅状态*/
#define SLQSL_obstruct 0/*水流阻塞状态*/
#define SDY_ok 0/*水流通畅状态*/
#define SDY_error 1/*水流阻塞状态*/
#define SYR_keep 1/*预燃维持中*/
#define SYR_cancel 0/*预燃已撤销*/
#define SYM_low 0/*水位低*/
#define SYM_high 1/*水位高*/
#define SLS_open 0/*门开*/
#define SLS_close 1/*门关*/
#define SGX_inserted 0/*光纤插入*/
#define SGX_renegade 1/*光纤未插入*/
#define SWGZ_open 1/*电机停转*/
#define SWGZ_close 0/*电机转动*/


extern uchar Count_F50V1000;/*超差-50%、充电电压超1000V，计次,累计超过200次，报故障73*/



extern uint Power;
/*增加功率显示常量，限制平均功率显示上限*/
#define maxShowPower 900
/*各个频率下最高可设置能量*/
extern uchar maxLimitSetEnergy_Ho;



#define TestDianji 1
#define notTestDianji 0
extern bit biTestDianji;

#define Dischang_A 0
#define Dischang_B 1
extern bit biDischang_AB;

extern uint uiChangVolt_A;
extern uint uiChangVolt_B;
extern uint uiChangVolt_A_MAX;
extern uint uiChangVolt_B_MAX;
extern bit Tiaojie_A;
extern bit Tiaojie_B;


extern uchar Ener_AB_flag;
extern uint oneEnergy;/*能量采样值*/
#define KEVLimited 50/*开机能量检测0.xxV 限制*/
extern float Ener_S;/*设置能量*/
extern float Ener_S_first;/*设置能量*/
extern float Ener_S_second;/*设置能量*/
extern float Ener_Sleft_A;/*设置能量*/
extern float Ener_Sright_A;/*设置能量*/
extern float Ener_Sleft_B;/*设置能量*/
extern float Ener_Sright_B;/*设置能量*/
extern float Ener_SA_temp;/*设置能量*/
extern float Ener_SB_temp;/*设置能量*/
extern float Ener_A_MAX;
extern float Ener_B_MAX;
extern uint idata uiEner_S;


extern uchar ucDischangingDianJi_T2overflows;


#define Flag_ZhengChang 1
#define Flag_JiaoZhun 0
extern bit Flag_Flow;


#define Dischanging_Yes 1
#define Dischanging_No 0
extern bit biDischanging;



/*充放电中断选择标志*/

extern uint idata count50us;/*与counter  相等时，执行充放电程序，然后清零*/
extern uint idata count1ms;/*用于定时器0  中断程序中非放电状态*/
extern uint idata CurPulse;/*当前发出的脉冲数，每一轮都重新计数*/
extern bit onecheck_flag;	 


/*定时器2  的变量，编译预处理*/
#define T2Fun1 0 /*测速，定位，触发放电*/
#define T2Fun2 1 /*电机是否故障*/
extern bit biChoiceT2Fun;
#define T2MeasureSpeed 1
#define T2SpringDischarge_DingWei 0
extern bit capture;
extern uint nb_overflows;/*number of overflow*/
extern uint nb_overflows_old;/*previous number of overflow*/
#define StartSample 0/*这两个标志的值是一样的*/
#define EndSample 0
extern uchar ucSamples;
extern uint RCAP2_old;/*previous 16 bits capture value*/
extern uint RCAP2;/*16 bits capture value*/
extern uchar uiT2OverFlows;/*Timer2第二功能使用*/
#define ABupdown 0x00
#define Adown 0x01
#define Bup 0x02



extern uchar idata saveshuzu[1];
extern uchar idata qushuzu[1];

extern uchar xdata work_info_read[25];
extern uchar xdata work_info_write[25];
#define Address_work_info_first 960	  //第一个工作信息地址，每50个字节存储一条工作信息记录
#define Address_work_info_state_jishu 6000
#define Address_work_info_state_weizhi 6001
extern bit real_time_flag;
extern uchar xdata work_info_jishu;
extern uint xdata work_info_weizhi;
extern uchar xdata work_info_weizhishu;

extern uchar xdata tuijian_database[6][3];

#define Left 0
#define Right 1
extern bit JT_L_R;/*0--左踏板		1--右踏板*/

extern uchar Status;

extern bit Clamp_flag; 

#define parachang 1
#define paraunchang 0
extern uchar FlagPara;/*记录是否由频率、能量参数的改变，用于选择初始充电电压*/



