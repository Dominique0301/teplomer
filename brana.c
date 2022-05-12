#include "stm8s.h"
#include "assert.h"
#include "milis.h"

#define LEDPORT GPIOE
#define LEDPIN0 GPIO_PIN_0
#define LEDPIN1 GPIO_PIN_1
#define LEDPIN2 GPIO_PIN_2
#define LEDPIN3 GPIO_PIN_3
#define LEDPIN4 GPIO_PIN_4
#define LEDPIN5 GPIO_PIN_5
#define LEDPIN6 GPIO_PIN_6
#define LEDPIN7 GPIO_PIN_7
#define BRANAPORT GPIOB
#define BRANA1 GPIO_PIN_0
#define BRANA2 GPIO_PIN_1

int cislo[10];
uint8_t i=0;
uint16_t time=0;
uint8_t minuly_stav1=1;
uint8_t aktualni_stav1=1;
uint8_t minuly_stav2=1;
uint8_t aktualni_stav2=1;

void main(void){
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
init_milis();

GPIO_Init(LEDPORT,LEDPIN0,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(LEDPORT,LEDPIN1,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(LEDPORT,LEDPIN2,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(LEDPORT,LEDPIN3,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(LEDPORT,LEDPIN4,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(LEDPORT,LEDPIN5,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(LEDPORT,LEDPIN6,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(LEDPORT,LEDPIN7,GPIO_MODE_OUT_PP_LOW_SLOW);
GPIO_Init(BRANAPORT, BRANA1,GPIO_MODE_IN_PU_NO_IT);
GPIO_Init(BRANAPORT, BRANA2,GPIO_MODE_IN_PU_NO_IT);
//port B piny 0-7 nastavim jako výstupy

cislo[0] = 0b11000000;
cislo[1] = 0b11111001;
cislo[2] = 0b10100100;
cislo[3] = 0b10110000;
cislo[4] = 0b10011001;
cislo[5] = 0b10010010;
cislo[6] = 0b10000010;
cislo[7] = 0b11111000;
cislo[8] = 0b10000000;
cislo[9] = 0b10011000;
// Jaké diody rosvítím 0bhgfedcba
//rozsvicuju 0

  while (1){
		if (milis() - time> 50){
		time = milis();
		if(GPIO_ReadInputPin(BRANAPORT, BRANA1)==RESET) { 
			aktualni_stav1=1;
		}
		else{
			aktualni_stav1=0;
		} 
		if(minuly_stav1==1 && aktualni_stav1==0){
			i+=1;
		}
		minuly_stav1 = aktualni_stav1;
		if (i>9){
			i=1;
		}

		if(GPIO_ReadInputPin(BRANAPORT, BRANA2)==RESET) { 
		aktualni_stav2=1;
		}
		else{
			aktualni_stav2=0;
		} 
		if(minuly_stav2==1 && aktualni_stav2==0){
			i-=1;
		}
		minuly_stav2 = aktualni_stav2;
		if (i>9){
			i=0;
		}
		GPIO_Write(LEDPORT, cislo[i]);
		}
	}
}