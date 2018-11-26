#include "pid.h"


volatile int Xpwm, Ypwm;
volatile float PX, IX, DX, PY, IY, DY;
XPID_typedef XPID;
YPID_typedef YPID;
u8 Xdirection, Ydirection;


//PID_setpara(22,0,135,24,0,148)
//PID_setpara(22,0,150,23,0,150)
//#define XP 2.2f                  			//PID参数的值
//#define XI 0.0f               		    //加f表示用浮点的形式保存
//#define XD 15.0f                   		//
//#define YP 2.3f
//#define YI 0.0f
//#define YD 15.0f
#define X_DIRECTION_FLAG	0x01         //方向变量标志位
#define Y_DIRECTION_FLAG	0x01         //


/*****************************************************************************************
函数功能：PID控制初始化，对各参数进行初始化并启动定时器
输入：无
输出：无
日期：2016/7/15
*****************************************************************************************/
void PID_Init(void)
{					// 
	XPID.xK=3; 		//5		5
	XPID.xP=0.8;	//0.8	0.8
	XPID.xI=0.8;	//0.8	0.8
	XPID.xD=0.8;	//0.8	6
	XPID.xError=0;
	XPID.xError_1=0;
	XPID.xError_2=0;
	XPID.wX[0]=1;
	XPID.wX[1]=1;
	XPID.wX[2]=1;
	//y
	YPID.yK=3;       //5	5
	YPID.yP=0.8;      //0.8	0.8
	YPID.yI=0.8;	 //0.8	0.8
	YPID.yD=0.8;      //0.8	6
	YPID.yError=0;          
	YPID.yError_1=0;
	YPID.yError_2=0;
	YPID.wY[0]=1;
	YPID.wY[1]=1;
	YPID.wY[2]=1;
    
}

/*****************************************************************************************
????:XPID????????
??:Current:???,PID梔ata
??:??????
??:2017/4/25
*****************************************************************************************/
int XNNPIDcalc(float current,XPID_typedef* xPID)
{
	float sumw=0;
	xPID->xError=2048-current;      

/***********************改进Hebb学习算法************************************************************/	
	xPID->wX[0]=xPID->wX[0]+(xPID->xP)*(xPID->xError)*(xPID->xout)*(xPID->xX[0]);
	xPID->wX[1]=xPID->wX[1]+(xPID->xI)*(xPID->xError)*(xPID->xout)*(xPID->xX[1]);
	xPID->wX[2]=xPID->wX[2]+(xPID->xD)*(xPID->xError)*(xPID->xout)*(xPID->xX[2]);
	
	sumw=fabs(xPID->wX[0])+fabs(xPID->wX[1])+fabs(xPID->wX[2]);  
	
	xPID->wX[0]=fabs(xPID->wX[0]/=sumw);   
	xPID->wX[1]=fabs(xPID->wX[1]/=sumw);   
	xPID->wX[2]=fabs(xPID->wX[2]/=sumw);   
	
	xPID->xX[0]=xPID->xError;         			//P 改用位置试PID控制
	xPID->xX[1]=xPID->xError-xPID->xError_1;	 //I	   
	xPID->xX[2]=xPID->xError-2*xPID->xError_1+xPID->xError_2;    //D
	
	xPID->xout=xPID->xK*((xPID->wX[0]*xPID->xX[0])+(xPID->wX[2]*xPID->xX[2]));  
	//xPID->xout=2*xPID->xX[0]+12*xPID->xX[1]+2*xPID->xX[2];	
	
	xPID->xError_2=xPID->xError_1;   
	xPID->xError_1=xPID->xError;	

	return xPID->xout<-2000?(-2000):(xPID->xout>2000?2000:xPID->xout);
}


/*****************************************************************************************
????:YPID????????
??:Current:???,PID梔ata
??:??????
??:2017/4/25
*****************************************************************************************/
int YNNPIDcalc(float current,YPID_typedef* yPID)
{
	float sumw=0;
	yPID->yError=2048-current;      //??????
/***********************改进Hebb学习算法************************************************************/
	yPID->wY[0]=yPID->wY[0]+(yPID->yP)*(yPID->yError)*(yPID->yout)*(yPID->yX[0]);
	yPID->wY[1]=yPID->wY[1]+(yPID->yI)*(yPID->yError)*(yPID->yout)*(yPID->yX[1]);//wkp_1+xiteP*error(k)*u_1*(2*error(k)-error_1)
	yPID->wY[2]=yPID->wY[2]+(yPID->yD)*(yPID->yError)*(yPID->yout)*(yPID->yX[2]);

	sumw=fabs(yPID->wY[0])+fabs(yPID->wY[1])+fabs(yPID->wY[2]);   
	
	yPID->wY[0]=fabs(yPID->wY[0]/=sumw);   //???? ???????  0?P
	yPID->wY[1]=fabs(yPID->wY[1]/=sumw);	//1?i
	yPID->wY[2]=fabs(yPID->wY[2]/=sumw);	//2?d
	
	yPID->yX[0]=yPID->yError;   //P
	yPID->yX[1]=yPID->yError-yPID->yError_1;				   //I
	yPID->yX[2]=yPID->yError-2*yPID->yError_1+yPID->yError_2;   //D
	
	yPID->yout=yPID->yK*((yPID->wY[0]*yPID->yX[0])+(yPID->wY[2]*yPID->yX[2]));  //????
	//yPID->yout=2*yPID->yX[0]+12*yPID->yX[1]+2*yPID->yX[2];
	
	yPID->yError_2=yPID->yError_1;   
	yPID->yError_1=yPID->yError;	
	
	return yPID->yout<-2000?(-2000):(yPID->yout>2000?2000:yPID->yout);
}



/*****************************************************************************************
函数功能：PID控制参数调试，修改Kp，Ki,Kd的参数
输入：u16 Kp,u16 Ki,u16 Kd
输出：无
日期：2017/4/8
*****************************************************************************************/
void PID_setpara(u16 xKp, u16 xKi, u16 xKd, u16 yKp, u16 yKi, u16 yKd)
{
    XPID.xP = (float)xKp / 10;
    XPID.xI = (float)xKi / 10;
    XPID.xD = (float)xKd / 10;
    YPID.yP = (float)yKp / 10;
    YPID.yI = (float)yKi / 10;
    YPID.yD = (float)yKd / 10;
    printf("xP=%.2fxI=%.2fxD=%.2fyP=%.2fyI=%.2fyD=%.2f\r\n", XPID.xP, XPID.xI, XPID.xD, YPID.yP, YPID.yI, YPID.yD);
}
/*****************************************************************************************
函数功能：定时器3中断初始化函数
输入：u16 计数次数, u16 预分频
输出：无
日期：2016/12/13
*****************************************************************************************/
void TIM3_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能或者失能指定的TIM中断	TIM3  使能

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设

}


/*****************************************************************************************
函数功能：中断服务程序，用于AD采电压和电流和PID控制PWM输出
输入：无
输出：无
日期：2016/7/15
*****************************************************************************************/

void TIM3_IRQHandler(void)      //TIM1中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update );  //清除TIMx的中断待处理位:TIM 中断源
        /********************************PID计算*******************************/
        Xpwm = XNNPIDcalc(Vx, &XPID);  						//调用PID算法
        Ypwm = YNNPIDcalc(Vy, &YPID);  						//调用PID算法
        if(Xpwm >= 0)
        {   //Xpwm>0时，说明浮子在左，所以要向右吸
            Xdirection = 0x01;
            TIM_OC1PolarityConfig(TIM1, TIM_OCPolarity_Low);
        }
        else                                                      //Xpwm<0时，说明浮子在右，所以要向左吸
        {
            Xdirection = 0x02;
            TIM_OC1PolarityConfig(TIM1, TIM_OCPolarity_High);
            Xpwm = -Xpwm;
        }
        if(Ypwm >= 0)   											 //Ypwm>0时，说明浮子在上，所以要向下吸
        {
            Ydirection = 0x01;
            TIM_OC4PolarityConfig(TIM1, TIM_OCPolarity_Low);
        }
        else                                                      //Ypwm<0时，说明浮子在下，所以要向上吸
        {
            Ydirection = 0x02;
            TIM_OC4PolarityConfig(TIM1, TIM_OCPolarity_High);
            Ypwm = -Ypwm;
        }
        /*******************************方向控制*******************************/

        if(Xdirection & X_DIRECTION_FLAG)
        {
            Xout = 0;        //x轴向右吸
        }
        else
        {
            Xout = 1;        //x轴向左吸
        }
        if (Ydirection & Y_DIRECTION_FLAG)
        {
            Yout = 0;			//y轴向下吸
        }
        else
        {
            Yout = 1;		  //y轴向上吸
        }
        TIM_SetCompare1(TIM1, Xpwm); //设置pwm值
        TIM_SetCompare4(TIM1, Ypwm); //设置pwm值
    }
}



