

 #include "include\includes.h"




/*延时子程序*/
#if 1
/*增加各种延时函数，如，1秒，2秒。。。*/
/*可以在延时返回时加入喂狗函数，使其它函数比较易读。*/
void Delay (  uint delaycounter )
{
  	uint xdata i,j;
  	for(i=0;i<delaycounter;i++){	  //延时10us,实测
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	  j++;
	}	  
}	  

void Delay_100us(){
	Delay (10);
}

void Delay_1s ( void )
{
  Delay (50000);
  Delay (50000);
  
	return;
}

void Delay_5s ( void )
{
	Delay_1s();
	Delay_1s();
	Delay_1s();
	Delay_1s();
	Delay_1s();
	return;
}


#endif












