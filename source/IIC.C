
#include "include\includes.h"

/*用来读写AT24C512外设EEPROM 64KBytes大小*/


bit             SM_BUSY = 0;             //SMB0忙标志位.
unsigned char   SMBMODE = 0;             //SMB0读写控制字CHIP_SELECT+R/W.
unsigned char   SMB_num = 0;             //SMB0一次读写的字节数..
unsigned int    start_adr = 0;           //SMB0读写字节在器件中的起始地址.
unsigned char   *SMB_Dat = 0;            //SMBUS中断传输数据指针

uchar  addr_num = 0;                     //两个字节地址计数
uchar  read_sign = 0;                    //选择读标志
uchar  chip_addr = 0;
uchar  read_flg = 0;
uchar eeprom_err=0;


void init_smbus(void)
{
    SMB0CN    = 0x44;        //允许SMBUS, 应答返回AA(低电平有效)
    //SMB0CR    = 0xF0;        //配置传送速度,-(SYSCLK/2*SCL_BAUD);IIC总线速率寄存器SCL=100K  
    SMB0CR    = 116;  		   //	配置传送速度,SYSCLK/(2*(256-SMB0CR+2.5)) = 77.6K
    EIE1     |= 0x02;        //使能SMBUS中断
    EIP1     |= 0x02;        //设置SMBUS中断为高优先级
} 

void write_data(unsigned char chip, unsigned int wordadr, uchar *dat, unsigned char number)
{ 
	addr_num = 0;
  SI = 0;
  SMB_num=number;          // 确定写数据大小
  while (SM_BUSY);         // 若SMBUS忙碌就等待
  SM_BUSY = 1;             // SM_BUSY是忙碌标志位 
  SMBMODE = chip & 0xfe;
  start_adr = wordadr;       // 装载写字节起始地址
  SMB_Dat = dat;
  STO = 0;
  STA = 1;  
  while (SM_BUSY);
  if(eeprom_err){
   	eeprom_eror();
   	eeprom_err=0;
   }	 
 }

void   read_data(unsigned char chip, unsigned int wordadr, uchar *dat, unsigned char number)
{	
	 SI = 0;
   SMB_num = number;          //确定写数据大小
   while (SM_BUSY);           // 若SMBUS忙碌就等待
   SM_BUSY = 1;               // SM_BUSY是忙碌标志位 
   read_sign = 1;             //置"1"开始伪写操作
   read_flg = 1;
   chip_addr = chip & 0xfe;  
   start_adr = wordadr;       //装载写字节起始地址
   SMB_Dat = dat;    
   STO = 0;
   STA = 1; 
   AA=1;
   while (SM_BUSY); 
   if(eeprom_err){
   	eeprom_eror();
   	eeprom_err=0;
   }
}



//SMBUS中断服务函数
void SMBUS_ISR (void) interrupt 7
{	
   EA = 0;	 
   switch (SMB0STA)
   {                  
      case  0x08:				              
              SMB0DAT = SMBMODE;    //发送从器件地址
          STA = 0;                      
         break;
         
      case  0x10:		
      	if (read_sign == 1)		       
         SMB0DAT = chip_addr | 0x01; //重复起始位
        else 
         SMB0DAT = SMBMODE;
         STA = 0;
         break;
         
      case 0x18:	
      			SMB0DAT = (uchar)(start_adr >> 8);
      			addr_num = 2;       
         break;
         
      case  0x28:		
      	 if (read_sign == 1){
      	   if (read_flg == 1)       //片内低字节伪写       
      		 { 
      			 read_flg = 2;
      			 SMB0DAT = (uchar)start_adr; 
      		 }  
      	   else if (read_flg == 2)               //为读操作则重新启动总线.
           { 
             STO = 0;
             STA = 1;
             break;
           }
         }
      	 else	
      	 	{    	
      	   if (addr_num == 2)               //片内地址第二个字节
        	   {
        	  	 addr_num = 0;
        	  	 SMB0DAT = (uchar)start_adr; 
        	   }	
           else if (addr_num == 0)
             { 	  		  
              if(SMB_num)
		           {							    
                 SMB0DAT = *SMB_Dat;   //如果为写操作则将数据写入SMB0DAT.
                 SMB_Dat++;		
				         SMB_num--;                 
               }
		          else                           //如果数据写入完毕则发送停止信号.
		           { 
                 STO = 1;
                 SM_BUSY = 0; 
               }
             } 
           }         	
         break;
                       
      case  0x40:            //发送从地址 + R 后收到 ACK              //选择地址读
         if(SMB_num == 1)    //如果只读一个字节则将应答位AA清0，
		        AA = 0;          //收到数据以后不发送应答位               
		                   	  	       	  
         break; 
         
      case  0x50:		         //收到数据字节 后已发送ACK    
		    if(SMB_num>2)
		        {								 
              *SMB_Dat = SMB0DAT;  
              SMB_Dat++;
			        SMB_num--;
			        AA=1;           
            }
		     else if(SMB_num == 2)  //最后一个字节
		        { 
			        *SMB_Dat = SMB0DAT;  
              SMB_Dat++;
			        SMB_num--;
			        AA=0;              //清除AA
            }         	                  
          break;
          
      case  0x58:	               //收到数据字节 后已发送NACK 
         *SMB_Dat = SMB0DAT;
         STO = 1;
         SM_BUSY = 0; 
         read_sign = 0;          //数据读完之后清零标志位              
         break;
         
      default:                  //报警
         STO = 1;                         
         SM_BUSY = 0;
         eeprom_err=1;
         break;
     }
	 SI=0;            // 清除中断标志
	 EA = 1;
}










