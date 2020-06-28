/*****************************************************************************
M31S1322_STM32_V1 2015年5月	 www.lcdsoc.com
使用说明：
1、本DEMO使用F103VCT6单片机 8m外部晶体
2、M47SP1322 采用双1322控制器，每个控制器控制256*64像素，通过CS1,CS2区分 一个4位表示1列的值，一次要写入2个字节，4列
3、供电电压3.3v IO高电平为 0.8*3.3v   IO低电平为 0.2*3.3v
4、主要函数见oled.c 文件
5、ASCII字库 自定义汉字库见oled.c根据自己需要取舍
6、定义、函数声明见oled.h
7、采用IO口模拟SPI通讯方式驱动oled模块，接线如下：
OLED端口定义 4线SPI
CS1	~ PE6
RES ~ PE5
DC   ~ PE4
CLK  ~ PE3
DIN ~ PE2
CS2 ~ PE1

8、x坐标和字间距d最好要为4的倍数，
******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "can.h"
#include "usart.h"
#include "LED.h"
#include "timer.h"
#include "exti.h"

void Int_To_Str(int x,char *Str);
extern uint32_t CAN_ID1,CAN_ID2;//标识符
extern uint8_t CAN_DATA0,CAN_DATA1,CAN_DATA2,CAN_DATA3,CAN_DATA4,CAN_DATA5,CAN_DATA6,CAN_DATA7;//数据
extern uint8_t CanFlag,Display,IDE;//中间变量
double velocity;
extern int velocity_flag;
extern int number;
int main(void)
{	int tem=99,voltage=0;
	unsigned char a[10];
	int m=100;
 	delay_init();			 //延时函数初始化
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//	OLED_Init();			//初始化OLED
	CAN_Configuration();  //CAN初始化
	USART_Configuration();
 	//LED_Init();				 //LED端口初始化
	EXTIX_Init();
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
 
	printf("welcome!\r\n"); 
//	LED_Configuration();

	//Asc12_24(72,50 ,"Emotion") ;  // 12*24点阵ASCII码  Every english character takes up 8
	//delay_ms(1864);					   // 延时
//	delay_ms(1864);
//	Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏

	//Int_To_Str(m,a);
	while(1)
	{	//CanWriteData(0x03);
	//	printf("ok");
		//printf("velocity:%d km/h\r\n",velocity);//这是轮子角速度//线速度在不考虑滑动摩擦下应该乘上轮子半径
		//TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标?
		if(velocity_flag==1)
		{
			printf("\r\n velocity:%f km/h\r\n",velocity);
			printf("\r\n number:%d \r\n",number);
			CanWriteData(0x8888);
			velocity_flag=0;
		}
		if(CanFlag == ENABLE ) //收到一帧正确的数据，并打印到串口
		{	CanFlag = DISABLE;
			if(IDE==CAN_ID_EXT)
			{
				printf("%x",CAN_ID2);
				printf("%x",CAN_DATA0);
				printf("%x",CAN_DATA1);
				printf("%x",CAN_DATA2);
				printf("%x",CAN_DATA3);
				printf("%x",CAN_DATA4);
				printf("%x",CAN_DATA5);
				printf("%x",CAN_DATA6);
				printf("%x",CAN_DATA7);
			}
//			{	if(CAN_ID2 == 0x186540f3)
//				{	tem=CAN_DATA0&0x04;
//					if(tem==0)
//					{	GPIO_SetBits(GPIOB ,GPIO_Pin_7);
//						GPIO_ResetBits(GPIOB ,GPIO_Pin_5);
//					}
//					else
//						GPIO_ResetBits(GPIOB ,GPIO_Pin_5|GPIO_Pin_7);
//				}
//				if(CAN_ID2 == 0x186040f3)
//				{
//					voltage=(CAN_DATA0<<8)+CAN_DATA1;
//					Int_To_Str(voltage,a);
//				}
//			}
			else
			{	
				printf("%x",CAN_ID1);
				printf("%x",CAN_DATA0);
				printf("%x",CAN_DATA1);
				printf("%x",CAN_DATA2);
				printf("%x",CAN_DATA3);
				printf("%x",CAN_DATA4);
				printf("%x",CAN_DATA5);
				printf("%x",CAN_DATA6);
				printf("%x",CAN_DATA7);
				//if(CAN_ID1 == 0x06)
//				{	int value = CAN_DATA2;
//					if (value <= 0x19)
//					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0);
//						GPIO_ResetBits(GPIOB ,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_6);
//					}
//					else if (value <= 0x32&&value>0x19)
//					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0|GPIO_Pin_1);
//						GPIO_ResetBits(GPIOB ,GPIO_Pin_2|GPIO_Pin_6);
//					}
//					else if (value <= 0x4B&&value > 0x32)
//					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);
//						GPIO_ResetBits(GPIOB ,GPIO_Pin_6);
//					}
//					else if (value >=0x4B)
//					{	GPIO_SetBits(GPIOB ,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_6);
//					}
//				}
//				else if(CAN_ID1 == 0x05)
//				{	if(CAN_DATA4==0x83)
//					{	GPIO_SetBits(GPIOB ,GPIO_Pin_5);
//						GPIO_ResetBits(GPIOB ,GPIO_Pin_7);
//					}
//					else
//						GPIO_ResetBits(GPIOB ,GPIO_Pin_7|GPIO_Pin_5);
//				}
			}
		}
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);//清接收中断标志


//  CanWriteData(0x1314);
//  		//ASCII码演示
//
//		char power[]="";
//		char speed[]="";
//		Asc12_24(72,50 ,"Emotion") ;  // 12*24点阵ASCII码  Every english character takes up 8
//		delay_ms(1864);					   // 延时
//	 	delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏
//
// 	  Asc5_8( 45,50 ,"Speed:") ;	   // 5*8点阵ASCII码
//	Asc8_16( 8*8,58 ,"voltage:") ;
//// 	  Asc8_16( 84,45 ,"8*16 ASCII") ;	   // 8*16点阵ASCII码
//		Asc20_40( 8*8+9*8,40 ,a) ;  //
//		Asc8_16( 8*8+9*8+24*3,58 ,"V") ;	   // 6*12点阵ASCII码
//		Asc8_16( 16*8,16*6 ,"speed:") ;	   //
//		Asc8_16( 16*8+8*6,16*6 ,"99") ;
//		Asc8_16( 16*8+8*6+8*2,16*6 ,"km/h") ;
//		delay_ms(1864);					   // 延时
//	 	delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏
//
//		Asc20_40( 0,43 ,"20*40ASCII") ;  // 20*40点阵ASCII码
//  		delay_ms(1864);
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏
//
//		//汉字演示
//	Show_HZ12_12(4,12,0,0,3);	// 12*12宋体
// 	  Show_HZ16_16(64,44,4,0,3);	// 坐标（64，17），字间距4，字库号0~3“晶奥科技”，16*16宋体
//		Show_HZ24_24(140,80,4,0,3); //24*24宋体
//		delay_ms(1864);
//		delay_ms(1864);
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏
  		//图片显示
//		Show_Pattern(&lcdsoc256128[0],0,63,0,127);	 //显示单色图像，取模采用16灰度取模
//	 	delay_ms(1864);
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏
//		delay_ms(1864);
	}

//		Grayscale(CS1);	  //上半屏灰度显示
//		delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏
//		Grayscale(CS2);	//下半屏灰度显示
//	 	delay_ms(1864);
//		Fill_Block(0x00,0x00,0x3F,0x00,0x7F); //清全屏
 }
void Int_To_Str(int x,char *Str)
{
  int t;
	char *Ptr,Buf[5];
	int i = 0;
	Ptr = Str;
	if(x < 10)
	{
		*Ptr ++ = '0';
		*Ptr ++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;	// ?????????ASCII???
		}
		i -- ;
		for(;i >= 0;i --) 		// ?????????
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}
