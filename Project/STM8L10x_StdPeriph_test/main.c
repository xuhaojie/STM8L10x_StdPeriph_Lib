/**
  ******************************************************************************
  * @file     Project/STM8L10x_StdPeriph_Templates/main.c
  * @author   MCD Application Team
  * @version V1.2.1
  * @date    30-September-2014
  * @brief    This file contains the firmware main function.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "stm8l10x_tim4.h"

/** @addtogroup STM8L10x_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
uint8_t     g_flag1ms=0;    // flag for 1ms interrupt (for TIM4 ISR)
uint32_t    g_count1ms=0;   // 1ms counter (for TIM4 ISR)


void main(void)
{
  /* init High speed internal clock prescaler: 1 */
  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);

  /* Enable TIM4 CLK */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Initialize LED pins in Output Mode */
  GPIO_Init(GPIOA, (GPIO_Pin_TypeDef) GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_SetBits(GPIOA, (GPIO_Pin_TypeDef) GPIO_Pin_7);

  // config 1ms clock
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
  TIM4_ClearFlag(TIM4_FLAG_Update);
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
  TIM4_Cmd(ENABLE);

  // enable interrupts
  enableInterrupts();

  // main loop
  while (1)
  {
    // every 1ms
    if (g_flag1ms)
    { 
      g_flag1ms=0;
    
      if ((g_count1ms % 500) == 0)
      {
        // toogle LED
        GPIO_ToggleBits(GPIOA, (GPIO_Pin_TypeDef) GPIO_Pin_7);

      } // every 500ms
    
    } // every 1ms
    
  } // main loop

} // main()


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
