
/*

关键字	存储区	地址范围	访问方式	大小	速度
data	内部 RAM 直接寻址	0x00~0x7F	直接	128B	最快
idata	内部 RAM 间接寻址	0x00~0xFF	间接	256B	快
bdata	内部 RAM 位寻址区	0x20~0x2F	直接+位	16B	快
xdata	外部 RAM	0x0000~0xFFFF	DPTR 间接	64KB	慢
pdata	外部 RAM 低 256B	0x00~0xFF	R0/R1 间接	256B	中
code	Flash/ROM	0x0000~0xFFFF	MOVC	64KB	中

C8051F020 实际情况：
内部 RAM：256B（data + idata + bdata）
外部 RAM：64KB（xdata），需要 EMI0CF 配置
Flash：64KB（code）
*/


/*************P4status4 —— 系统状态信号组 1*************/
uchar bdata P4status;
sbit SDWA  = P4status^0;   // A 路水温低（<=5℃）
sbit SGWA  = P4status^1;   // A 路水温高（>35℃）
sbit SDWB  = P4status^2;   // B 路水温低
sbit SGWB  = P4status^3;   // B 路水温高
sbit SGX   = P4status^4;   // 光纤状态：0=插入，1=未插入
sbit SLS   = P4status^5;   // 门锁：0=门开，1=门关
sbit SJTA0 = P4status^6;   // 左脚踏：1=踩下，0=松开
sbit SJTA1 = P4status^7;   // 右脚踏：1=踩下，0=松开
/******************************************************/


/*************P5status5 —— 系统状态信号组 2*************/
uchar bdata P5status;     
sbit COVER   = P5status^2;  // 盖子状态   把 P5status 的第 2 位命名为 COVER。
sbit SYM     = P5status^3;  // 水位：1=正常，0=低
sbit SLQSL_A = P5status^4;  // A 路水流量：1=通畅，0=阻塞
sbit SLQSL_B = P5status^5;  // B 路水流量
sbit KAIHUAN = P5status^6;  // 开机模式：1=进入工程模式
/******************************************************/


/*************P5status6 —— 系统状态信号组 3*************/
uchar bdata P6status;
sbit SYR_B = P6status^0;  // B 路预燃：1=维持，0=撤销

//当氙灯被点亮后，输出给控制器的信号，低电平有效
sbit SYR_A = P6status^1;  // A 路预燃
sbit SDY_B = P6status^2;  // B 路电源：0=正常，1=故障
sbit SDY_A = P6status^3;  // A 路电源

//充满电信号：当电容充满电时同时输出高电平；
sbit SWGZ0 = P6status^4;  // 充满电：1=满，0=未满
/******************************************************/


/***************ucMaskByte —— 故障屏蔽字***************/
uchar bdata ucMaskByte = 0xFF;       // 默认全屏蔽（1=屏蔽，0=检测）
sbit MB_SLQSL_AB = ucMaskByte^0;     /*水流AB*/  
sbit MB_SYM      = ucMaskByte^1;     /*水位*/
sbit MB_ST_YC    = ucMaskByte^2;     /*水温预警,超高*/
sbit MB_SYR_AB   = ucMaskByte^3;     /*预燃AB*/
sbit MB_SDY_AB   = ucMaskByte^4;     /*电源AB*/
sbit MB_SGZ_ABW  = ucMaskByte^5;     /*三个光闸*/
sbit MB_SLS      = ucMaskByte^6;     /*门联锁*/
sbit MB_SGX      = ucMaskByte^7;     /*光纤*/
/******************************************************/


/*****************EEPROM 读写缓冲***************************
 *因为大部分 EEPROM 操作是单字节读写，用一个临时数组做中转。*/
uchar idata saveshuzu[1];  // 写 EEPROM 用的缓冲（1 字节）
uchar idata qushuzu[1];    // 读 EEPROM 用的缓冲（1 字节）
/******************************************************/


/*****************工作记录（历史信息）*****************
 *偏移	内容
 *[0]	年
 *[1]	月
 *[2]	日
 *[3]	时
 *[4]	分
 *[5]	秒
 *[6..8]	本次脉冲数（3 字节）
 *[9..11]	本次能量（3 字节）
 *[12..13]	本次打光时间（分钟，2 字节）
 *[14]	保留
 */
uchar xdata work_info_read[25];  // 读工作记录缓冲（25 字节）
uchar xdata work_info_write[25]; // 写工作记录缓冲
bit real_time_flag;							 // 实时时间读取完成标志
uchar xdata work_info_jishu;     // 工作记录总数（0~100）
uint xdata work_info_weizhi;     // 当前工作记录在 EEPROM 的地址
uchar xdata work_info_weizhishu; // 当前工作记录的位置序号（0~99）
/******************************************************/


/********总能量，周期ms 开机时从 EEPROM 800~803（脉冲）、
 *804~807（能量）读入，4 字节拼成 ulong*/ 
ulong TotalPulse_U;   // 总脉冲数（4 字节，最大 42 亿）
ulong TotalJoule_U;   // 总能量（4 字节，单位 0.1J）
/******************************************************/


/*********************本次开机数据*********************/
ulong TotalJoule_benci;  // 本次开机总能量（0.1J 单位）
ulong TotalPulse_benci;  // 本次开机总脉冲数
float Totalminutes_benci;// 本次开机总打光时间（分钟）
ulong TotalJoule_benci_1;// TotalJoule_benci/10（显示用）
/******************************************************/


/**********************用户设置参数********************/
uchar SetFrequency;	      // 当前设置频率（Hz）
uchar SetEnergy_left;     // A 路能量（0.1J，如 25 表示 2.5J）
uchar SetFrequency_right;	// 右踏板频率（旧版遗留，基本不用）
uchar SetEnergy_right;    // B 路能量
uchar SetFrequency_last;  // 上次频率（用于判断电机分频切换）
 
uchar SetFrequency_double;	 // 双脉冲模式下的频率
uchar SetFrequency_only;     // 单脉冲模式下的频率
uchar SetEnergy_left_double; // A 路双脉冲能量	
uchar SetEnergy_left_only;   // A 路单脉冲能量
uchar SetEnergy_right_double;// B 路双脉冲能量
uchar SetEnergy_right_only;  // B 路单脉冲能量
																		
uchar Pulse_time=10;  		 // 放点脉宽时间  Pulse_time*50 us
uchar Pulse_time2=10;      // 第二脉冲脉宽（双脉冲）
uchar Pulse_time_left=10;  // 左踏板脉宽
uchar Pulse_time_right=10; // 右踏板脉宽
uchar Pulse_jiange=46;     // 双脉冲间隔（Pulse_jiange*50us）
/******************************************************/
																		
																		
/**********************充电压控制*********************/
uint uiChangVolt_A;      // A 路当前充电压（V）
uint uiChangVolt_B;      // B 路当前充电压
uint uiChangVolt_A_MAX;  // A 路历史最高反馈点对应的充电压
uint uiChangVolt_B_MAX;  // B 路
bit Tiaojie_A=0;         // A 路电压调节触顶标志
bit Tiaojie_B=0;         // B 路
uint uiChangVolt_A_last; /*保存上次充电电压*/ // 上次打光结束时的充电压
uint uiChangVolt_B_last; /*保存上次充电电压*/ // 上次打光结束时的充电压
/******************************************************/


/*********************能量反馈*************************/
uchar Ener_AB_flag = 0;  //A路
float Ener_S;            // 当前目标能量对应的反馈值
float Ener_S_first;			 // 第一个脉冲的反馈能量
float Ener_S_second;		 // 第二个脉冲的反馈能量（双脉冲）
float Ener_Sleft_A;			 // 左踏板 A 路反馈
float Ener_Sright_A;	   // 右踏板 A 路反馈
float Ener_Sleft_B;			 // 左踏板 B 路反馈
float Ener_Sright_B;	   // 右踏板 B 路反馈
float Ener_SA_temp;	     // A 路临时反馈
float Ener_SB_temp;	     // B 路临时反馈
float Ener_A_MAX;				 // A 路历史最高反馈能量
float Ener_B_MAX;				 // B 路
uint idata uiEner_S;		 // 从 EEPROM 读出的目标反馈值
/******************************************************/


/*********************ADC采样**************************/
uint oneEnergy;     // 单次 ADC 采样值（0~4095）
uchar adc_select;   // ADC 通道选择：0=能量，1=A 路电压，2=B 路电压
bit detect_flag = 0;// 电压检测完成标志  
uint detect_VB;     // A 路充电电压 ADC 值
uint detect_VA;     // B 路充电电压 ADC 值
/******************************************************/


/***************充放电中断选择标志*********************/
uint idata count50us;  /*与counter  相等时，执行充放电程序，然后清零*/
uint idata count1ms;   /*用于定时器0  中断程序中非放电状态*/
uint idata CurPulse=0; /*当前发出的脉冲数，每一轮都重新计数*/
bit onecheck_flag=0;   // 反馈只采集一次标志
bit Pulse_time_flag=0; // 放电标志（双脉冲用）
/******************************************************/


/*********************电机控制*************************/
/*测速原理：T2 捕获模式，电机每转一圈产生一个脉冲，捕获 6 次取平均，算出周期。*/
uchar Pre_CPH0;                        // 目标 PCA0CPH0 值（决定电机频率）
uchar ucDianJiErrorTimes;              // 电机错误次数
uchar ucDischangingDianJi_T2overflows; // 放电中电机超时计数
uchar uiT2OverFlows = 0x00;            /*Timer2第二功能使用*/  // 电机故障检测用 T2 溢出计数
bit biWhetherHalt;                     /*0--未停转		1--已停转*/ //未使用

bit biChoiceT2Fun;                     // T2 功能选择：0=测速/定位，1=电机故障
bit capture;													 // 捕获模式：1=测速，0=放电定位
uint nb_overflows=0x0000;              /*number of overflow*/  // T2 溢出次数
uint nb_overflows_old=0x0000;          /*previous number of overflow*/ // 上次溢出次数
uchar ucSamples;											 // 采样计数
uint RCAP2_old=0x0000;								 /*previous 16 bits capture value*/
uint RCAP2=0x0000;										 /*16 bits capture value*/

uint pwm_ld_data=0;										 // 指示光 PWM 占空比（0~65535）
uint pwm_ld_data_temp=0;               // 闪烁前的亮度缓存
uint pwm_fs_data=0;										 // 风扇 PWM（未使用）
/******************************************************/


/***********************标志位*************************/
bit biTestDianji;                  // 是否启用电机检测
bit biDischang_AB;								 // 放电腔选择
bit Flag_Flow = Flag_ZhengChang;   // 流程：正常/校准
bit biDischanging = Dischanging_No;// 是否正在充放电
bit JT_L_R;												 // 脚踏：0=左，1=右
bit gzall_flag;										 // 光闸开关
bit yr_flag;											 // 预燃开关
bit yra_flag;											 // A 路预燃
bit yrb_flag;											 // B 路预燃
bit jdq_flag;											 // 继电器开关
bit fls_flag;											 // 指示光闪/恒
bit onf_flag;											 // 指示光开/关
bit adc0_flag;										 // ADC 采样完成
bit one_orflag;									   // 单腔/多腔：1=单腔，0=多腔
bit fs_flag;											 // 风扇（未使用）
bit jsy_flag;											 // 警示音开关
bit double_pluse_flag;						 // 单/双脉冲：0=单，1=双
bit long_short_pluse_flag = 0;		 // 长/短脉冲：0=短，1=长
bit Clamp_flag=0;									 // 钳位标志
bit baoyang_jishu_flag = 0;				 // 保养计数已增加标志

/*准备状态修改能量参数标志 FlagEnergy_L/R 取值：*/
#define decEnergy 2      	//能量减少
#define incEnergy 1				//能量增加
#define unChangEnergy 0		//未变
#define ChangEnergy 1			//改变
uchar FlagPara = parachang;						  /*记录是否由频率、能量参数的改变，用于选择初始充电电压*/
uchar Count_F50V1000 = 0;								/*超差-50%、充电电压超1000V，计次,累计超过200次，报故障73*/
uchar idata FlagEnergy_L = ChangEnergy; // A 路能量改变标志
uchar idata FlagEnergy_R = ChangEnergy; // B 路

/******************************************************/


/***********************选择与显示*********************/
uchar ucChoiceABlight;     // 优先腔：0=A，1=B
bit Flag_ShowFeedBack;     // 是否显示反馈值
uchar Select_Laser;        // 标志发射激光的腔
uchar maxLimitSetEnergy_Ho;// 当前频率下最大可设能量
uint Power;								 // 显示用功率 = 频率 × 能量
/******************************************************/


/************************温湿度************************/
uint humidity_SW;				   // 水温（未使用）							
uint temperature_SW;		   // 水温
uint humidity_HJ;					 // 环境湿度
uint temperature_HJ;			 // 环境温度
uchar am2302_timeflg=0;    // AM2302 采样计时（10ms 计数）
uchar shuiwen_chuli;       // 水温处理值（影响充电压）
uchar shuiwen_chuli_old;   // 上次水温处理值
bit am2302flg=0;					 // 采样完成标志
bit LD_flg=0;							 // 指示光闪烁状态
/******************************************************/


/*******************RFID/光纤**************************/
bit RF_writeflag=0;					 // 正在写卡标志
bit T_2s_flag=0;					   // 2 秒定时标志
uint rf_jishu=0;						 // RFID 计数
uchar xdata lot_data[4];		 // 序列号  // 当前卡的 4 字节 UID
uchar xdata gx_info_read[8]; // 从 EEPROM 读的光纤记录
uchar xdata gx_youxiao=0;    // 光纤是否有效
uchar xdata gx_weizhi_temp=0;// 临时位置
uchar xdata gx_weizhi=0;     // 当前光纤记录位置
uint xdata gx_jiaoyan=0;     // 校验字
uchar guangxian_shouming;		 // 光纤最大寿命（次数）
uchar gx_ener_restrict;			 // 光纤规格限制：1=1000um，2=600um，3=400um，4=200um
uchar MB_SGX_jishu;					 // 屏蔽光纤次数										
/******************************************************/


/**********************其他****************************/
uint baoyang_jishu;					 // 保养计数（当前打光时间/5分钟）
uint baoyang_cankao;		     // 保养参考（多少分钟后提示保养）

uchar linshi_jishu=0;				 // 临时计数
uchar last_times=0;					 // 上次次数
uchar Status;								 // 主状态：1=待机，2=准备，3=数据库，4=信息

uchar xdata tuijian_database[6][3] = {   //专家库参数
	  20, 10, 15,		// 频率 20Hz，A 路 1.0J，B 路 1.5J
	  20, 15, 25,		// 20Hz，1.5J，2.5J
	  12,  8, 15,		// 12Hz，0.8J，1.5J
	  20, 10, 20,		// 20Hz，1.0J，2.0J
	  24, 30, 10,		// 24Hz，3.0J，1.0J
	  20, 20, 10		// 20Hz，2.0J，1.0J
};
/******************************************************/


















