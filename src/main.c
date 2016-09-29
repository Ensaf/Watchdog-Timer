
//**********************************************************************/
/*
   $Author: Ensaf Atef Hassan
   $Email:ensafatef@gmail.com

**********************************************************************/
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_wwdg.h>
 
void setup_Periph(void);
void Delay(unsigned long ms);
 
 
void Delay(unsigned long ms)
{ unsigned long i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<1450;j++);
}
 
 
void setup_Periph(void)
{
 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
 
    //port initialization
    GPIO_InitTypeDef GPIO_InitStructure;
 
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOD,&GPIO_InitStructure);
 
    }
 
void ResetWatchdog(void)
{WWDG_SetCounter(80); }
 
 
void WWDG_IRQHandler(void)
{
 
if (WWDG_GetFlagStatus())
  {
  WWDG_SetCounter(0x7f);
  WWDG_ClearFlag();
  }
 
}
 
 
void FeedDog(float round)
{
    while(round)
    { Delay (65);
    WWDG_SetCounter(127);
    round--;}
}
 
 
 int main(void)
{
     setup_Periph();
     //make sure the clk is stable
       RCC_HSEConfig(RCC_HSE_ON);
       while(!RCC_WaitForHSEStartUp());
 
 
       WWDG_SetPrescaler(WWDG_Prescaler_8);    //WWDG clock counter = (PCLK1(30MHz)/4096)/1 = 7324 Hz (~137 us)
       WWDG_SetCounter(120);           //Werte 0x40 und 0x7F
       WWDG_SetWindowValue(80);         //0x80
 
                           //Reset  < 120  > 64
 
       WWDG_Enable(127);            //WWDG timeout = ~137 us * (127-64) = 8.6ms
 
       WWDG_ClearFlag();
       WWDG_EnableIT();
 
     GPIO_SetBits(GPIOD, GPIO_Pin_1);
     Delay(10000); //10 ms
     GPIO_ResetBits(GPIOD, GPIO_Pin_1);
     Delay(10000); //100 ms
 
      while (1)
      {
              GPIO_SetBits(GPIOD, GPIO_Pin_0);
                Delay(10000); //10 ms
 
                GPIO_ResetBits(GPIOD, GPIO_Pin_0);
                Delay(10000); //100 ms
 
                void ResetWatchdog(void);
                WWDG_SetCounter(120);
                FeedDog(8);
                RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
                WWDG_Enable(127);
 
                for(;;) {}
 
          }
}
