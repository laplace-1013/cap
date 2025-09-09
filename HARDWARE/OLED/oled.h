#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	

#define USE_HORIZONTAL 0  //设置显示方向 0：正向显示，1：旋转180度显示

//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SCL
#define OLED_SCL_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_SDA_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)//SDA
#define OLED_SDA_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define OLED_RES_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//RES
#define OLED_RES_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_DC_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//DC
#define OLED_DC_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_3)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_3)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_WR_Byte(u8 dat,u8 cmd);
void Column_Address(u8 a, u8 b);
void Row_Address(u8 a, u8 b);
void OLED_Clear(u8 x1,u8 y1,u8 x2,u8 y2,u8 color);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey);
void OLED_ShowString(u8 x,u8 y,u8 *dp,u8 sizey);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 sizey);
void OLED_DrawBMP(u8 x,u8 y,u8 width,u8 height,const u8 *BMP);
void Init_ssd1327(void);


#endif

