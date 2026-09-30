
extern void MF_1fnshow (void);
extern void Display_dianji_Fre(uchar cc);
extern void Display_feedback_volt(void);
extern void fnShowMaskByte(void);
extern void LCD_CycleCount(uint cc);
extern void fn_TotalPulseJoule(void);  
extern void Display_SetChangVolt_A(uint cc );
extern void Display_SetChangVolt_B(uint cc );
extern void MF_5Display_FeedBackEnergyVoltA(uint cc );
extern void MF_5Display_FeedBackEnergyVoltB(uint cc );
extern void MF_5Display_Frequency(uchar cc);
extern void fnshowDaiJi_JiaoZhun(void); 
extern void fnshowzhiliao_JiaoZhun(void); 
extern void LCDqudong6_unsaved (  );
extern void LCDqudong6_saved (  );
extern void LCDqudong6_double_only (  );
extern void LCDqudong6_AorB (  );
extern void LCDqudong6_LorS (  );
extern void LCDqudong6_nengliang (  );
extern void LCDqudong6_pinlv (  );
extern void LCDqudong6_fankui (  );
extern void LCD_rf (  );
extern void MF_Display_2302 (  );
extern void MF_Display_detectA (  );
extern void MF_Display_detectB (  );

extern void Display_Frequency_left ( uchar FrequencyByte );
extern void Display_Energy_left ( uchar EnergyByte );
extern void Display_power_left ( uint EnergyByte );
extern void Display_Frequency_right ( uchar FrequencyByte );
extern void Display_Energy_right ( uchar EnergyByte );
extern void Display_power_right ( uint EnergyByte );
extern void show_daiji_qiehuan ( void );
extern void show_zhunbei_qiehuan ( void );
extern void show_database_qiehuan ( uint cc );
extern void show_ldoff_qiehuan ( void );
extern void show_ldon_qiehuan ( void );
extern void show_ldshan_qiehuan ( void );
extern void show_ldheng_qiehuan ( void );
extern void Display_FeedBackEnergyVolt_left(uint cc,uint dd);
extern void Display_FeedBackEnergyVolt_right(uint cc,uint dd);
extern void LCD_caijiaota_left();
extern void LCD_caijiaota_right();
extern void LCD_clean_caijiaota ();
extern void LCDzhiliaojizijian (  );
extern void SysFault(uchar i);
extern void LCD_WatertemOk(void);
extern void LCD_CloseDoor(void);
extern void LCD_CheckFeetSwitch (void);
extern void LCD_InsertFiber (void);
extern void LCD_WaterSteamError (void);
extern void LCD_PowerError (void);										 
extern void LCD_SystemAdjust (void);
extern void LCD_Curing (void);
extern void LCD_A_ChargeVolt (uint uiVolt);
extern void LCD_B_ChargeVolt (uint uiVolt);
extern void eeprom_eror (void);
extern void change_picture(uchar pic_id);
extern void fnShowzhunbei(void);
extern void fnShowdaiji(void);
extern void UART0_Senddata(unsigned char buf);
extern void UART0_Send( unsigned char *buf, unsigned int size );
extern void UART0_Init( void );
extern void fnShowStatus(void);
extern void read_realtime(void);

extern void LCD_WatertemDanger (void);
extern void LCD_WatertemisFalling ( void );
extern void LCD_DianJi_TingZhuanorTaiMan (void);
extern void LCD_SetParameterNewly (void);
extern void show_double_qiehuan (void);
extern void show_single_qiehuan (void);
extern void show_long_qiehuan (void);
extern void show_short_qiehuan (void);
extern void show_fashe_left (void);
extern void show_fashe_right (void);
extern void show_fashe_no (void);

extern void fnShowwork_info_lishi(uchar cccc );	
extern void fnShowwork_info_lishiqingchu(uchar cccc );	
extern void fnShowwork_info_benci(void );
extern void fnShowwork_database(void );
extern void fnshowshuiliu_JiaoZhun();
extern void LCD_Pulsetime ();	 
extern void LCD_jiaozhengtime ();
extern void fnShowwork_RH_HJ();


extern void MF_1RFerro(void);
extern void RF_erroshow(void);
extern void RF_gxshow(void);
extern void RF_gxlianjieshow(void);
extern void RF_initshow(void);	  
extern void GX_shixiaoshow(void);
extern void RF_init_qingchu(void);
extern void GX_cishushanshuo(void);


extern void BY_tishi (void);
extern void baoyang_dangqianjishu();
extern void baoyang_cankaoshu();
extern void baoyang_gxshu();

extern void LCD_zengliangtiaojie_xiuzheng (uchar cccc);


































