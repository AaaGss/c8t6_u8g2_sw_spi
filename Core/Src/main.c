/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @author         :AaaGss-YYKF
  * @date           :2023/4/12
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "gpio.h"

#include "OLED.h"
#include "u8g2.h"

void SystemClock_Config(void);

/*官方提供的Logo绘制demo*/
void draw(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
    u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*字库选择*/
    u8g2_DrawStr(u8g2, 0, 20, "U");

    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21, 8, "8");

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51, 30, "g");
    u8g2_DrawStr(u8g2, 67, 30, "\xb2");

    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);

    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1, 54, "github.com/olikraus/u8g2");
}


uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT: /*GPIO的初始化，在main中已经初始化完成了*/
            break;
        case U8X8_MSG_GPIO_SPI_DATA:
            OLED_W_D1(arg_int); //SPI - MOSI
            break;
        case U8X8_MSG_GPIO_SPI_CLOCK: //SPI - CLK
            OLED_W_D0(arg_int);
            break;
        case U8X8_MSG_DELAY_MILLI: /*延时函数*/
            HAL_Delay(arg_int);
            break;
        case U8X8_MSG_GPIO_CS: /*片选信号*/
            OLED_W_CS(arg_int);
            break;
        case U8X8_MSG_GPIO_DC: /*设置DC引脚,表明发送的是数据还是命令*/
            OLED_W_DC(arg_int);
            break;
        case U8X8_MSG_GPIO_RESET:
            break;
    }
    return 1;
}

void u8g2Init(u8g2_t *u8g2)
{
    /*
    U8G2_R0： 顺时针旋转0度
    U8G2_R1： 顺时针旋转90度
    U8G2_R2： 顺时针旋转180度
    U8G2_R3： 顺时针旋转270度
    U8G2_MIRROR：U8G2_R0的镜像
    */
    u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(u8g2); // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
    u8g2_SetPowerSave(u8g2, 0); // 打开显示器
    u8g2_ClearBuffer(u8g2);

    HAL_Delay(500);

    u8g2_DrawLine(u8g2, 0, 0, 127, 63);
    u8g2_DrawLine(u8g2, 127, 0, 0, 63);
    u8g2_DrawCircle(u8g2, 63, 32, 15, 0x05);
    u8g2_SendBuffer(u8g2);
    HAL_Delay(1000);
}

int main(void)
{

    u8g2_t u8g2;
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    OLED_Init();

    u8g2Init(&u8g2);
    u8g2_FirstPage(&u8g2);
    do
    {
        draw(&u8g2);
    }
    while (u8g2_NextPage(&u8g2));

    while (1)
    {
    }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}
