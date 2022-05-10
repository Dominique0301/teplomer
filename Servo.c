#include "stm8s.h"
#include "milis.h"

void init_pwm(void);
void servo(void);

uint8_t x=0,y=0,z=0;
uint16_t last_time=0,time=0;

void main(void){
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_SLOW);

init_milis(); 
init_pwm(); 

  while (1){
		servo();
  }
}


void servo(void){
static uint16_t pulse=1500; // výchozí šířka pulzu
static uint16_t last_time=0;  

  if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_4)==RESET ){ 
		TIM2_SetCompare1(1437);
		/*
		pulse = pulse + 100; 					// zvětšíme polohu o 10%
		if(pulse>1999){pulse = 999;} 	// pokud jsme na konci rozsahu, začneme od začátku
		TIM2_SetCompare1(pulse); 			// zapíšeme šířku pulzu do timeru
		*/
  }
}


void init_pwm(void){
// nastavíme piny PD2,PD3,PD4 jako výstupy (kanály TIM2, kanál TIM_CH3 reampován z pinu PA3 na PD2)
TIM2_TimeBaseInit(TIM2_PRESCALER_16,19999);

TIM2_OC1Init( 							// inicializujeme kanál 1 (TM2_CH1)
	TIM2_OCMODE_PWM1, 				// režim PWM1
	TIM2_OUTPUTSTATE_ENABLE,	// Výstup povolen (TIMer ovládá pin)
	SERVO_DEFAULT_POSITION,		// výchozí hodnota šířky pulzu je 1.5ms
	TIM2_OCPOLARITY_HIGH			// Zátěž rozsvěcíme hodnotou HIGH 
	);
	
TIM2_OC1PreloadConfig(ENABLE);
TIM2_Cmd(ENABLE);
}

// pod tímto komentáoem nic nemiote 
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