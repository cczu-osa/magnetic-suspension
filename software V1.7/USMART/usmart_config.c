#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加)



//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[] =
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
    (void*)read_addr, "u32 read_addr(u32 addr)",
    (void*)write_addr, "void write_addr(u32 addr,u32 val)",
#endif
    (void*)PID_setpara, "PID_setpara(u16 xKp, u16 xKi, u16 xKd,u16 yKp, u16 yKi, u16 yKd)",	//(void*)fun(int a);函数返回一个指针，是void类型
    (void*)Debug, "void Debug(u8 flag)",
    (void*)PID_SWITCH, "void PID_SWITCH(u8 flag)",
};
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev =
{
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), //函数数量
    0,	  	//参数数量
    0,	 	//函数ID
    1,		//参数显示类型,0,10进制;1,16进制
    0,		//参数类型.bitx:,0,数字;1,字符串
    0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
    0,		//函数的参数,需要PARM_LEN个0初始化
};

/*****************************************************************************************
函数功能：调试模式开关
输入：u8 flag
输出：无
日期：2017/4/22
*****************************************************************************************/
void Debug(u8 flag)
{
    if(0x01==flag)
    {
        TIM_Cmd(TIM5, ENABLE); //使能TIM5
    }
    else
    {
        TIM_Cmd(TIM5, DISABLE); //使能TIM5
    }
}

/*****************************************************************************************
函数功能：pid开关，用于开启整体功能，与低功耗模式
输入：u8 flag
输出：无
日期：2017/4/22
*****************************************************************************************/
u8 pow_state=0x01;       //系统运行状态值，0001代表正在运行，0000代表低功耗
void PID_SWITCH(u8 flag)
{
    if((flag==0x01)&&(pow_state!=flag))             
    {
		GPIOA->CRH&=0XFFFF0FF0; 
		GPIOA->CRH|=0X0000B00B;//PA8 复用推挽输出 
        TIM_Cmd(TIM3, ENABLE);  //使能TIM3外设
        TIM_Cmd(TIM1, ENABLE); //使能TIM1
		pow_state=0x01;
    }
    else if((flag==0x00)&&(pow_state!=flag))
    {
		TIM_Cmd(TIM3, DISABLE);  //关闭PID计算
        TIM_Cmd(TIM1, DISABLE); //使能TIM1
		GPIOA->CRH&=0XFFFF0FF0; 
		GPIOA->CRH|=0X00003003;//PA8 普通推挽输出 
		GPIOA->ODR|=0<<8;      //PA8 输出低
		GPIOA->ODR|=0<<1;      //PA11 输出低
        Xout=0;
		Yout=0;
		pow_state=0x00;
    }
}
















