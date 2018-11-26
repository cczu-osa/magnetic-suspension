#include "tim5.h"
/*****************************************************************************************
函数功能：定时器5中断初始化函数
输入：u16 计数次数, u16 预分频
输出：无
日期：2017/4/21
*****************************************************************************************/
void TIM5_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); //使能或者失能指定的TIM中断	TIM2  使能

    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    //TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设

}
/*****************************************************************************************
函数功能：定时器5中断
输入：void
输出：无
日期：2017/4/21
*****************************************************************************************/
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //溢出中断
    {
	  //printf("Vx=%d  Vy=%d\r\n",Vx,Vy);
      //printf("Vx=%dXOUT=%dVy=%dYOUT=%d\r\n",Vx,XPID.xout,Vy,YPID.yout);
		printf("xw0=%.4f xw1=%.4f xw2=%.4f yw0=%.4f yw1=%.4f yw2=%.4f ",XPID.wX[0],XPID.wX[1],XPID.wX[2],YPID.wY[0],YPID.wY[1],YPID.wY[2]);
    }
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update );  //清除TIMx的中断待处理位:TIM 中断源
}


