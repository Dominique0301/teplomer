#include "stm8s.h"
#include "milis.h"
#include "stdio.h"
#include "keypad.h" // tady máme "knihovnu" pro skenování stavu klávesnice

void keypad(void);

void main(void){
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW);

init_milis(); 	
keypad_init();

  while (1){
		
		keypad();
				
	}
}
	
void keypad(void){

static uint8_t kombinace[5]={4,5,6,7,8};//správná kombinace
static uint8_t stisknuto;//funkce která sleduje stav na klávesnici
static uint16_t x=0,y=0,i=0,j=0;

	if(stisknuto==0xFF){y=0;}//když nic není stisknuto y=0
		stisknuto=keypad_scan(); //sleduje zda je něco stisknuto
		if(stisknuto!=0xFF && y==0){//jestli je stisknuto a zároveň předtím nic stisknuto nebylo
			y=1;//aby při dlouhém stisku se nepočítalo více stisknutí
			if(stisknuto == 1){
				GPIO_WriteReverse(GPIOC,GPIO_PIN_5);
			}
			if(stisknuto == 2){
				GPIO_WriteReverse(GPIOC,GPIO_PIN_5);
			}
		}
	}
}
	

// pod tímto komentářem nic neměňte 
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
