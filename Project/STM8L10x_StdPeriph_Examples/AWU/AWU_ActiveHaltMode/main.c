/**
  ******************************************************************************
  * @file    AWU/AWU_ActiveHaltMode/main.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    30-September-2014
  * @brief   This file is used to configure the Library.
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
#include "stm8l101_eval.h"
#include "stm8l_eval_lcd.h"

/**
  * @addtogroup AWU_ActiveHaltMode
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void AWU_Config(void);
static void CLK_Config(void);
void Delay (uint16_t nCount);
uint32_t LSIMeasurment(void);
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
bool ButtonPressed = FALSE;

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  uint16_t i = 0;
  uint16_t j = 0;

  /* Clock configuration -----------------------------------------*/
  CLK_Config();
   
  /* Initialize LEDs mounted on the EVAL board */
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* Initialize KEY pushbutton mounted on the EVAL board */
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
    
  /* Configure LCD mounted on the EVAL board ----------------*/
  STM8L_EVAL_LCD_Init();
    
  /* Clear LCD */
  LCD_Clear();
 
  /*AWU configuration --------------------------------------------*/
  AWU_Config();
    
  /* Print "Running..." on LCD line1*/
  LCD_SetCursorPos(LCD_LINE1, 0);
  LCD_Print("  Running...  ");
  /* Print "Press Key" on LCD line2*/
  LCD_SetCursorPos(LCD_LINE2, 0);
  LCD_Print("  Press Key   ");
    
  /* Enable general interrupts */
  enableInterrupts();    
    
  while (1)
  {

    /* Check button status */
    if (ButtonPressed == TRUE) /* Button is pressed */
    {

      ButtonPressed = FALSE;
        
      /* Clear LCD lines */
     LCD_Clear();
     LCD_SetCursorPos(LCD_LINE1, 0);
     LCD_Print(" Preparing to ");
     LCD_SetCursorPos(LCD_LINE2, 0);
     LCD_Print("    sleep...  ");

      /* LEDs toggle quickly */
      for (j = 0; j < 20; j++)
      {
        STM_EVAL_LEDToggle(LED2);
        STM_EVAL_LEDToggle(LED3);
        STM_EVAL_LEDToggle(LED4);
        for (i = 0; i < 2; i++)
  {
          Delay((uint16_t)60000);
        }
       }

      /* Switch LEDs OFF */
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED3);
      STM_EVAL_LEDOff(LED4);

      LCD_Clear();

      halt(); /* Program halted */

      /* Program re-starts here, thanks to AWU */
      LCD_Clear();
      LCD_SetCursorPos(LCD_LINE1, 0);
      LCD_Print("  Running...  ");
      /* Print "Press Key" on LCD line2*/
      LCD_SetCursorPos(LCD_LINE2, 0);
      LCD_Print("  Press Key   ");

    }
    else
    {
      /* Toggle LEDs slowly */
      STM_EVAL_LEDToggle(LED2);
      STM_EVAL_LEDToggle(LED3);
      STM_EVAL_LEDToggle(LED4);
      for (i = 0; i < 15; i++)
      {
        Delay((uint16_t)60000);
      }
    }
  }
}

/**
  * @brief  Configure system clock to run at 16Mhz
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Initialization of the clock */
  /* Clock divider to HSI/1 */
  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
    
  /* Enable SPI clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_AWU, ENABLE);
}

/**
  * @brief  Configure the AWU time base to 12s
  * @param  None
  * @retval None
  */
static void AWU_Config(void)
{
  /* Initialization of AWU */
   /* LSI calibration for accurate auto wake up time base*/
  AWU_LSICalibrationConfig(LSIMeasurment());

  /* The delay corresponds to the time we will stay in Halt mode */
  AWU_Init(AWU_Timebase_12s);
}


/**
  * @brief Measure the LSI frequency using timer IC1 and update the calibration registers.
  * @note   It is recommended to use a timer clock frequency of at least 10MHz in order 
  *         to obtain a better in the LSI frequency measurement.
  * @param  None
  * @retval None
  */
uint32_t LSIMeasurment(void)
{

  uint32_t lsi_freq_hz = 0x0;
  uint32_t fmaster = 0x0;
  uint16_t ICValue1 = 0x0;
  uint16_t ICValue2 = 0x0;

  /* Get master frequency */
  fmaster = CLK_GetClockFreq();

  /* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 */
  AWU->CSR |= AWU_CSR_MSR;

  /* Capture only every 8 events!!! */
  TIM2_ICInit(  TIM2_Channel_1, TIM2_ICPolarity_Rising, TIM2_ICSelection_DirectTI, TIM2_ICPSC_Div8, 0x0);

  /* Enable TIM2 */
  TIM2_Cmd(ENABLE);

  /* wait a capture on cc1 */
  while ((TIM2->SR1 & (uint8_t)TIM2_FLAG_CC1) != TIM2_FLAG_CC1);
  /* Get CCR1 value*/
  ICValue1 = TIM2_GetCapture1();
  TIM2_ClearFlag(TIM2_FLAG_CC1);

  /* wait a capture on cc1 */
  while ((TIM2->SR1 & (uint8_t)TIM2_FLAG_CC1) != TIM2_FLAG_CC1);
  /* Get CCR1 value*/
  ICValue2 = TIM2_GetCapture1();
  TIM2_ClearFlag(TIM2_FLAG_CC1);

  /* Disable IC1 input capture */
  TIM2->CCER1 &= (uint8_t)(~TIM_CCER1_CC1E);
  /* Disable TIM2 */
  TIM2_Cmd(DISABLE);

  /* Compute LSI clock frequency */
  lsi_freq_hz = (8 * fmaster) / (ICValue2 - ICValue1);

  /* Disable the LSI measurement: LSI clock disconnected from timer Input Capture 1 */
  AWU->CSR &= (uint8_t)(~AWU_CSR_MSR);
  
  return (lsi_freq_hz);

}

/**
  * @brief  Delay.
  * @param  nCount
  * @retval None
  */
void Delay(uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

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