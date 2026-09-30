#include "include\includes.h"


unsigned char U8FLAG;

// DHT11   温湿度读取子程序    : /*   更换DS18B20    20260925  -Zterch   */
// 读一字节
 unsigned char read_one_byte1()
{
	  unsigned char i;
	  unsigned char bit_test;
	  static unsigned char dht11_temp;
	
		for(i=0;i<8;i++)
	  { 
				U8FLAG = 2;
				while((!D2302HJ)&&U8FLAG++);
				Delay(2);
				Delay(2);
				//Delay(2);
			  bit_test = 0;
			
			  if(D2302HJ)
				    bit_test =1;
				
	      U8FLAG=2;
				while(D2302HJ&&U8FLAG++);
			  if(U8FLAG==1)break;
			  dht11_temp <<= 1;
		    dht11_temp |= bit_test;
		}
		
	  return dht11_temp;
}

void  RH_HJ(void)   //室内
{  
	  unsigned char temp[4];
	  unsigned char data_sum;
	  unsigned char data_sum_temp;
	
	  D2302HJ = 0;//总线拉低
	  Delay (500);
	  //总线拉高。延时20US
	  D2302HJ = 1;
	  Delay (2);
	  Delay (2);
    Delay (2);
    //Delay (2);
    //D2302HJ=1;
	  if(!D2302HJ) 
	  {
	      U8FLAG=2;
	      while((!D2302HJ)&&U8FLAG++);
	      U8FLAG=2;
	      while(D2302HJ&&U8FLAG++);
	      temp[0]=read_one_byte1();
	      temp[1]=read_one_byte1();
	      temp[2]=read_one_byte1();
	      temp[3]=read_one_byte1();
	      data_sum=read_one_byte1();
	      //delayus(20);
	      //D2302HJ=1;
	      data_sum_temp=(temp[0]+temp[1]+temp[2]+temp[3]);
	      if(data_sum==data_sum_temp)
	      {
	          humidity_HJ=temp[0];
	          humidity_HJ=humidity_HJ*256+temp[1];
	          temperature_HJ=temp[2];
	          temperature_HJ=temperature_HJ*256+temp[3];
	      }   
	  }     
}






