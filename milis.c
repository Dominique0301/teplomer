// STOPKY s 7segment MAX7219 
//Dominik Šlehofer 3.B

#include "milis.h"
#include "stm8_hd44780.h"

#define CLK_PORT GPIOB
#define CLK_PIN GPIO_PIN_1

#define CS_PORT GPIOB
#define CS_PIN GPIO_PIN_0

#define DIN_PORT GPIOB
#define DIN_PIN GPIO_PIN_2


#define SET(x) GPIO_WriteHigh(x##_PORT, x##_PIN)
#define CLR(x) GPIO_WriteLow(x##_PORT, x##_PIN)

// makra adres/pøíkazu pro lepší ovládání MAX7219
#define NOOP 		  0  	
#define DIGIT0 		1	
#define DIGIT1 		2	
#define DIGIT2 		3	
#define DIGIT3 		4	
#define DIGIT4 		5	
#define DIGIT5 		6	
#define DIGIT6 		7	
#define DIGIT7 		8	

#if defined(STM8S208) || defined(STM8S103)
volatile uint16_t miliseconds=0; // global variable storing milis value

// return value of milisecond timestamp ("atomic" read)
uint16_t milis(void){
uint16_t tmp;
TIM4_ITConfig(TIM4_IT_UPDATE,DISABLE);
tmp = miliseconds;
TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);
return tmp;
}

//promìnné, které jsem si definoval v main.c teï musí mít pøed sebou extern
extern uint8_t sek; 
extern uint8_t min;
extern uint8_t stav;
uint32_t cas;



void max7219(uint8_t adresa, uint8_t data)
	{
	uint16_t mask;
	CLR(CS);//zaèínám vysílat, Chip Select(CS) dám do 0(CLEAR)
	
	//testuji jestli nejvyšší bit je 1 nebo 0
	//adresa
	mask = 1<<7;//posunutí 1 o 7 bitu do leva
	while(mask){
	 CLR(CLK);
	 if(adresa & mask){//logický souèin adresy a masky
				 SET(DIN);//když je nejvyšší bit 1 zavolám SET
			} else {
				 CLR(DIN);//když je nejvyšší bit 0 zavolám CLR
			}
			SET(CLK);//tikání nahoru
			mask >>=1;//posunutí masky o 1 bit do prava
			CLR(CLK);//tikání dolù 
		}
		
		
	//data
	mask = 1<<7;
	while(mask){
	 CLR(CLK);
	 if(data & mask){//logický souèin dat a masky
				 SET(DIN);
			} else {
				 CLR(DIN);
			}
			SET(CLK);
			mask >>=1;
			CLR(CLK);
		}
		
		SET(CS);//konèí vysílaní, chip select dám do 1
	}


//init_milis už bylo v milis.c
// init TIM4 as source for milis interrupt (low priority)
void init_milis(void){
TIM4_TimeBaseInit(PRESCALER,PERIOD); // (16MHz / 128) / 125 = 1000Hz
TIM4_ClearFlag(TIM4_FLAG_UPDATE);
TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);
ITC_SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_1); // nízká priorita pøerušení
TIM4_Cmd(ENABLE);
enableInterrupts();
}
// milis interrupt routine

INTERRUPT TIM4_UPD_OVF_IRQHandler(){
TIM4_ClearFlag(TIM4_FLAG_UPDATE);
miliseconds++;//sèítaní milisekund


if (GPIO_ReadInputPin(GPIOG,GPIO_PIN_2)==RESET){//tlaèítko pro zapnutí stopek
 stav=1;
 }
if (GPIO_ReadInputPin(GPIOG,GPIO_PIN_3)==RESET){//tlaèítko pro pozastavení stopek
 stav=0;
}

cas = miliseconds % 1000;//zbytek po dìlení musí být 0


if (cas == 0 & stav == 1)//pokud bude napoèítáno 1000 ms a stisknuto tlaèítko pro zapnutí, spustí se stopky
	{
		sek++; //sekundy se zvýší o 1
			if (sek == 60){ //Pokud se sekundy rovnají 60
				sek=0; //Sekundy se vynulují
				min++; //minuty se zvýší o jednu
			}
			if (min == 60){ //Pokud se minuty rovnají 60, vynulují se
				min = 0;
			}
			//Nastavení casu na pravou stranu displeje
			max7219(1, sek-(sek/10*10)); //jednotky od sekund, dìlení celými èísly napø.:25/10=2
			max7219(2, sek/10); //desítky od sekund
			
			max7219(3, min-(min/10*10)); //jednotky od minut
			max7219(4, min/10); //desítky od minut
	}
}

#else
#warning "millis not defined for this platform !"
#endif








