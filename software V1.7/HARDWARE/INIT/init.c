#include "init.h"


void all_init(void)
{
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
    uart_init(115200);	 	//串口初始化为115200
    LED_Init();		  		//初始化与LED连接的硬件接口	
    PID_Init();         	//PID初始化
	DMA_init();             //DMA初始化  优先级3，3
    Adc1_Init();		  	//ADC初始化    
    Adc2_Init();
    Adc3_Init();    
	TIM5_Int_Init(499, 7199);		//50ms------3,2----用于打印调试信息
    usmart_dev.init(72); 	//初始化USMART  00ms-----1,3----用于扫描串口
	TIM1_PWM_Init(1999, 8); 		//0.25ms----pwm频率 4Khz
    TIM3_Int_Init(499, 71);		//0.5ms------0,3----用于PID计算，配置定时器3    2Khz    
    TIM2_Int_Init(3999, 35999);  	//4000ms----3,1----用于低功耗控制---定时器2，用于设定低功耗  APB1 36Mhz*2 

}






