#include "spse_stm8.h"
#include "stm8s.h"
#include "milis.h"
#include "stm8s_gpio.h"
#include "stdio.h"


void ADC_init(void);

#define STAV_START 1 
#define STAV_STOP 2 
#define STAV_ZAVORA 3

uint32_t prevod=0,vysledek=0,y=0;
uint16_t x=0,z=0;
uint16_t last_time=0,time=0;


void main(void){
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

GPIO_Init(GPIOD,GPIO_PIN_4,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(GPIOC,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW);
init_milis(); 
ADC_init();
init_uart1();

  while (1){

    if(milis()-last_time == 500 && x==0){
      GPIO_WriteHigh(GPIOC, GPIO_PIN_5);
      last_time=milis();
      x=1;
    }
    
    if(milis()-last_time == 500 && x==1){
      GPIO_WriteLow(GPIOC, GPIO_PIN_5);
      last_time=milis();
      x=0;
      y= ADC_get(ADC2_CHANNEL_2); 
      vysledek= 5000*y/1024;
     printf("U= %ldmV\n\r",vysledek);

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

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"