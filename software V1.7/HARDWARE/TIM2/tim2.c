#include "usart.h"
#include "tim2.h"
#include "usmart.h"
/*****************************************************************************************
函数功能：定时器2中断初始化函数
输入：u16 计数次数, u16 预分频
输出：无
日期：2017/4/21
*****************************************************************************************/
void TIM2_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //使能或者失能指定的TIM中断	TIM2  使能

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设

}
/*****************************************************************************************
函数功能：定时器2中断
输入：void
输出：无
日期：2017/4/21
*****************************************************************************************/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //溢出中断
    {
        float adcx3;
        adcx3 = Get_Adc_Average3();
        //printf("%.2f\r\n", adcx3);
        if(adcx3 > 1000)
        {
            PID_SWITCH(0x01);
        } else
        {
            PID_SWITCH(0x00);
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update );  //清除TIMx的中断待处理位:TIM 中断源
}


