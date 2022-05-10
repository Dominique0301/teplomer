#include "stm8s.h"
#include "milis.h"
#include "stdio.h"
#include "spse_stm8.h"

void init_pwm(void);
void ADC_init(void);
void uart_putchar(char data); 
void uart_puts(char* retezec);

char text[32];
uint16_t last2;
uint16_t last;
uint32_t volt=0;
uint16_t adc_value;
uint16_t adc_value2;


void main(void){
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); 
init_milis(); 
UART1_Init(115200,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TX_ENABLE);
UART1_Cmd(ENABLE);
GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
init_pwm(); 
ADC_init();
  while (1){
		adc_value = ADC_get(ADC2_CHANNEL_2);
		TIM2_SetCompare2(adc_value);
		
		adc_value2 = ADC_get(ADC2_CHANNEL_3);
		if(adc_value > adc_value2){GPIO_WriteHigh(GPIOD, GPIO_PIN_6);}
		else{GPIO_WriteLow(GPIOD, GPIO_PIN_6);}
		 
	
		if((milis() - last) >= 1000){
			last = milis();
			volt=(adc_value*50 + 512)/1024;
			sprintf(text,"Stisky %3u \n\r",volt,"(100mV)");
			uart_puts(text);
		}
		if((milis() - last2) >= 1900){
			last2 = milis();
			volt=0;
		}
	}
}
				
  
void ADC_init(void){
// na pinech/vstupech ADC_IN2 (PB2) a ADC_IN3 (PB3) vypneme vstupní buffer
ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL2,DISABLE);
ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL3,DISABLE);
// nastavíme clock pro ADC (16MHz / 4 = 4MHz)
ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4);
// volíme zarovnání výsledku (typicky vpravo, jen vyjmečně je výhodné vlevo)
ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
// nasatvíme multiplexer na některý ze vstupních kanálů
ADC2_Select_Channel(ADC2_CHANNEL_2);
// rozběhneme AD převodník
ADC2_Cmd(ENABLE);
// počkáme než se AD převodník rozběhne (~7us)
ADC2_Startup_Wait();
}

void init_pwm(void){
TIM2_TimeBaseInit(TIM2_PRESCALER_1,1500-1);

TIM2_OC2Init( 	
	TIM2_OCMODE_PWM1, 			
	TIM2_OUTPUTSTATE_ENABLE,
	0,										
	TIM2_OCPOLARITY_HIGH	
	);
	
	

TIM2_OC2PreloadConfig(ENABLE);
TIM2_Cmd(ENABLE);
}

void uart_putchar(char data){
 while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET){ 
 UART1_SendData8(data); 
 }
}


void uart_puts(char* retezec){ 
 while(*retezec){ 
  uart_putchar(*retezec); 
  retezec++; 
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
