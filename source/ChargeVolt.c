#include "include\includes.h"

void fnDischangVolt_OutPut_uiA(uint ss)   //最大输入为 2400
{
  float fldata;
   if (ss>800)
	 {
		ss = 800;//800v
	 }
  fldata = (float)ss/1152.0;
  ss = (uint)(4095.0*fldata);
  DAC0L = ss%256;    // 电压基准2.4v (ss*4096/24/4)%256		  由于AB放电顺序问题，将输出控制颠倒
  DAC0H = ss/256;
}

void fnDischangVolt_OutPut_uiB(uint ss)   //最大输入为 2400
{
  float fldata;
   if (ss>800)
	 {
		ss = 800;//800v
	 }
  fldata = (float)ss/1152.0;
  ss = (uint)(4095.0*fldata);
 
  DAC1L = ss%256;    // 电压基准2.4v (ss*4096/24/4)%256 
  DAC1H = ss/256;
}

void DAC0_Init(void)
{
   REF0CN |= 0x03;                  // 启用内部的电压基准源
   DAC0CN  = 0x80;                  // 启用 DAC0 更新数据高位时更新输出 格式为最高4位无效
   DAC1CN  = 0x80;                  // 启用 DAC1 更新数据高位时更新输出 格式为最高4位无效
}












