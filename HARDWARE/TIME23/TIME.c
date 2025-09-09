#include "TIME.h"
#include "oled.h"
#include "bsp_ina226.h"
#include <stdio.h>

u8 string_V[20], string_I[20], string_W[20], string_mAh[20], string_Wh[20], string_T[20];
u16 V_int = 0, V_dec = 0, I_int = 0, I_dec = 0, W_int = 0, W_dec = 0;

static u8 time_num = 0;
static u32 All_Sec = 0;
static u8 Hour = 0;
static u8 Min = 0;
static u8 Sec = 0;
static u32 mAh = 0;
static u32 Wh = 0;

void TIM2_Getsample_Int(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ�ܶ�ʱ��2���´����ж�
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); 
}
//***************TIME2���ж�*******************/
void TIM2_IRQHandler(void)
{
	
		
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		get_power(); 
    V_int = (uint16_t)(ina226_data.voltageVal / 1000);
    V_dec = (uint16_t)((ina226_data.voltageVal / 1000 - V_int) * 1000);
    I_int = (uint16_t)(ina226_data.Shunt_Current / 1000);
    I_dec = (uint16_t)((ina226_data.Shunt_Current / 1000 - I_int) * 1000);
    W_int = (uint16_t)ina226_data.Power_Val;
    W_dec = (uint16_t)((ina226_data.Power_Val - W_int) * 1000);
    
		
    sprintf((char *)string_V, "%02d.%03d V ", V_int, V_dec);
    sprintf((char *)string_I, "%02d.%03d A ", I_int, I_dec);
    sprintf((char *)string_W, "%02d.%03d W ", W_int, W_dec);
		
		   
		float Capacity;
		float voltage;
		sscanf((char *)string_V, "%f", &voltage);

	// ���� voltage �ǵ�ǰ��ѹ, Capacity ��Ҫ���������

if (voltage >= 29.4f)
{
    // ��ѹ���ڻ������ߵ㣬��Ϊ100%
    Capacity = 100.0f;
}
else if (voltage <= 22.4f)
{
    // ��ѹ���ڻ������͵㣬��Ϊ0%
    Capacity = 0.0f;
}
else if (voltage > 25.0f)
{
    // ����1: ��ѹ�� (25.0V, 29.4V) ֮��
    // ������ 30% ���������� 100%
    Capacity = (voltage - 25.0f) * 70.0f / 4.4f + 30.0f;
}
else
{
    // ����2: ��ѹ�� [22.4V, 25.0V] ֮��
    // ������ 0% ���������� 30%
    Capacity = (voltage - 22.4f) * 30.0f / 2.6f;
}



char Cap[20];
sprintf((char *)Cap, "%.2f %%", Capacity);
OLED_ShowString(16, 52, (u8*)Cap, 16); // ��OLED����ʾ����
		
		
		
		
		
		
    OLED_ShowString(16, 4, string_V, 16);
    OLED_ShowString(16, 20, string_I,16);
    OLED_ShowString(16, 36, string_W, 16);
		
		//��ʱ
		time_num++;
		if(ina226_data.Shunt_Current > 1)
		{
		if(time_num >= 5)
			{
				All_Sec++;
				Hour = All_Sec/(60*60);
				Min = (All_Sec%(60*60))/60;
				Sec = (All_Sec%(60*60))%60;
				time_num = 0;
			  
				mAh = ina226_data.Shunt_Current/3600 + mAh;
		    Wh += (ina226_data.voltageVal / 1000) * (ina226_data.Shunt_Current/1000)/3600;	
		    
			}
		}
		else
		{
			if(time_num >= 5)
				time_num = 0;
		}
		sprintf((char *)string_mAh, "%06d mAh ", mAh);
		sprintf((char *)string_Wh, "%06d Wh ", Wh);
		sprintf((char *)string_T, "TIME:%02d:%02d:%02d ", Hour,Min,Sec);	
    
			OLED_ShowString(24, 76, string_mAh, 16);
			OLED_ShowString(24, 92, string_Wh, 16);
			OLED_ShowString(12, 108, string_T, 16);
	
}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}

