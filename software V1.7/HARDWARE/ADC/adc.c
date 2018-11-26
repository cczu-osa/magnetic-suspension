#include "adc.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//ADC 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
vu32 ADC_value[6]={0};
vu16 Vx,Vy;

//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3
void  Adc1_Init(void)
{
/***********************************************初始化结构体*************************************************/
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

/***********************************************初始化时钟*************************************************/
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );	 //使能ADC1通道时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
/***********************************************初始化io*************************************************/
    //PA1\PA2 作为模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);
/***********************************************初始化ADC1*************************************************/
   
	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

    ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;									//ADC工作模式:同步规则模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;										//模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;									//模数转换工作在多次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;					//转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;												//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);													//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
/*********************************************同步规则组**************************************************/
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );  		//采样71.5周期

/***********************************************开启ADC1*************************************************/
	
	ADC_DMACmd(ADC1, ENABLE);//开启ADC DMA支持
    ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
    ADC_ResetCalibration(ADC1);	//使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
    ADC_StartCalibration(ADC1);	 //开启AD校准
    while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
/***********************************************开启转换*************************************************/	
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}


/*************************************************ADC2初始化****************************/
void  Adc2_Init(void)
{
/***********************************************初始化结构体*************************************************/
    ADC_InitTypeDef  ADC_InitStructure;

/***********************************************初始化时钟*************************************************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE );	 //使能ADC1通道时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M	
	
/***********************************************初始化ADC2************************************************/

    ADC_DeInit(ADC2);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

    ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC工作模式:同步规则模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在多次
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC2, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
	
/*********************************************同步规则组**************************************************/
	
	ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);

/***********************************************开启ADC2*************************************************/
   
	ADC_DMACmd(ADC1, ENABLE);//开启ADC DMA支持
	ADC_Cmd(ADC2, ENABLE);	//使能指定的ADC1
    ADC_ResetCalibration(ADC2);	//使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC2));	//等待复位校准结束
    ADC_StartCalibration(ADC2);	 //开启AD校准
    while(ADC_GetCalibrationStatus(ADC2));	 //等待校准结束
	
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		//使能指定的ADC2的软件转换启动功能
}

/*************************************************ADC3初始化****************************/
void  Adc3_Init(void)
{
    ADC_InitTypeDef  ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC3, ENABLE );	 //使能ADC3通道时钟

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

    //PA1 作为模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_DeInit(ADC3);  //复位ADC3,将外设 ADC3 的全部寄存器重设为缺省值

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC3, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器


    ADC_Cmd(ADC3, ENABLE);	//使能指定的ADC1
    ADC_ResetCalibration(ADC3);	//使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC3));	//等待复位校准结束
    ADC_StartCalibration(ADC3);	 //开启AD校准
    while(ADC_GetCalibrationStatus(ADC3));	 //等待校准结束

}


/**************************获得ADC1值  ch:通道值 1*****************************************/

//u16 Get_Adc_Average1(void)
//{
//    u32 temp_val = 0;
//    u8 t;
//    for(t = 0; t < 6; t++)
//    {
//		//设置指定ADC的规则组通道，一个序列，采样时间
//    ADC_RegularChannelConfig(ADC1,1, 1, ADC_SampleTime_71Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
//    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
//        temp_val += ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
//    }
//    return temp_val / 6;
//}


///**************************获得ADC2值  ch:通道值 2*****************************************/

//u16 Get_Adc_Average2(void)
//{
//    u32 temp_val = 0;
//    u8 t;
//    for(t = 0; t < 6; t++)
//    {    //设置指定ADC的规则组通道，一个序列，采样时间
//    ADC_RegularChannelConfig(ADC2, 2, 1, ADC_SampleTime_71Cycles5 );	//ADC2,ADC通道,采样时间为239.5周期
//    ADC_SoftwareStartConvCmd(ADC2, ENABLE);		//使能指定的ADC1的软件转换启动功能
//    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束
//        temp_val += ADC_GetConversionValue(ADC2);//返回最近一次ADC2规则组的转换结果
//    }
//    return temp_val / 6;
//}

/**************************获得ADC3值  ch:通道值 3*****************************************/

u16 Get_Adc_Average3(void)
{
    u32 temp_val = 0;
    u8 t;
    for(t = 0; t < 4; t++)
    {
        //设置指定ADC的规则组通道，一个序列，采样时间
        ADC_RegularChannelConfig(ADC3, 3, 1, ADC_SampleTime_71Cycles5 );	//ADC3,ADC通道,采样时间为239.5周期
        ADC_SoftwareStartConvCmd(ADC3, ENABLE);		//使能指定的ADC1的软件转换启动功能
        while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束
        temp_val += ADC_GetConversionValue(ADC3); //返回最近一次ADC2规则组的转换结果
    }
    return temp_val / 4;
}








