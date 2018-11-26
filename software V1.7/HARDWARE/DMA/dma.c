#include "dma.h"


void DMA_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;         //使用DMA1的通道1
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
/***********************************************初始化DMA1*************************************************/

	DMA_DeInit(DMA1_Channel1); 															//初始化DMA1通道1
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);						//外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_value;								//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 									//外设到内存
	DMA_InitStructure.DMA_BufferSize = 6; 												//数据通道大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 					//外设地址不增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 							//内存地址增加
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; 		//字=4字节=32bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; 				//字=4字节=32bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 									//循环工作模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 								//通道优先级高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 										//不是内存到内存
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 										//初始化
	
/***********************************************DMA中断配置*************************************************/	
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);            // Enable the DMA Interrupt 
	
	DMA_ClearFlag(DMA1_FLAG_TC1);
    DMA_ClearITPendingBit(DMA1_IT_TC1);
  
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);  //DMA传输完成中断
    DMA_Cmd(DMA1_Channel1, ENABLE);						//使能DMA1	
}

/**************************平均滤波*****************************************/
void filter(void)
{
	u8 i=0;
	u16 x=0;
	u16 y=0;
	do
	{
		y+=(u16)(ADC_value[i]&0x0000ffff);  //低位是ADC1
		x+=(u16)((ADC_value[i]&0xffff0000)>>16);
		i++;
	}
	while(i<6);
	Vy=y/6;
	Vx=x/6;
}

void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
    {
        filter();
        DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
}	






