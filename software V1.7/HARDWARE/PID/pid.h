#ifndef __PID_H
#define __PID_H	 
#include "sys.h"
#include "led.h"
#include "usart.h"
#include "pwm.h"
#include "adc.h"
#include "math.h"

typedef struct
{
	float xK;                  //神经元比例系数
    float xP;                  //此处的P、I、D三个参数是学习速率
    float xI;
    float xD;
    float xError;  			   //当前误差
    float xError_1;			   //前一次误差
	float xError_2;			   //前前次误差
	float wX[3];               //x的权重
	float xX[3];               //x的状态值
	int xout;				   //输出
    
} XPID_typedef;


typedef struct
{
	float yK;                  //神经元比例系数
	float yP;
	float yI;
	float yD;
	float yError;  		   //当前误差
	float yError_1;  		   //上一次误差
	float yError_2;		   	   //上上次误差
	float wY[3];
	float yX[3];               //y的状态值
	int yout;
} YPID_typedef;

extern XPID_typedef XPID;
extern YPID_typedef YPID;

extern volatile int Xpwm,Ypwm;
extern volatile float PX,IX,DX,PY,IY,DY;
void TIM3_IRQHandler(void); 				//定时器初始化
void TIM3_Int_Init(u16 arr, u16 psc);                 //pid计算函数已经被调用到中断函数中
void PID_Init(void);															//pid初始化
void PID_setpara(u16 xKp, u16 xKi, u16 xKd,u16 yKp, u16 yKi, u16 yKd);


#endif


