#ifndef __OLED_H__
#define __OLED_H__
#include "main.h"

#define OLED_W_D0(x)		HAL_GPIO_WritePin(SPI1_CLK_D0_SOFT_GPIO_Port,SPI1_CLK_D0_SOFT_Pin,x)
#define OLED_W_D1(x)		HAL_GPIO_WritePin(SPI1_MOSI_D1_SOFT_GPIO_Port,SPI1_MOSI_D1_SOFT_Pin,x)
#define OLED_W_RES(x)		HAL_GPIO_WritePin(SPI1_RES_SOFT_GPIO_Port,SPI1_RES_SOFT_Pin,x)
#define OLED_W_DC(x)		HAL_GPIO_WritePin(SPI1_DC_SOFT_GPIO_Port,SPI1_DC_SOFT_Pin,x)
#define OLED_W_CS(x)		HAL_GPIO_WritePin(SPI1_CS_SOFT_GPIO_Port,SPI1_CS_SOFT_Pin,x)


void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t data, uint8_t Mode);
void OLED_Init(void);
void OLED_Fill(uint8_t DAT);
void OLED_SetCursor(uint8_t Y, uint8_t X);
#endif
