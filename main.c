// STOPKY s 7segment MAX7219 
//Dominik �lehofer 3.B
#include "stm8s.h"
#include "assert.h"
#include "milis.h"
#include "spse_stm8.h"
#include "stm8_hd44780.h"

//kam zapoj�m CLK,CS a DIN 
#define CLK_PORT GPIOB
#define CLK_PIN GPIO_PIN_1

#define CS_PORT GPIOB
#define CS_PIN GPIO_PIN_0

#define DIN_PORT GPIOB
#define DIN_PIN GPIO_PIN_2


#define SET(x) GPIO_WriteHigh(x##_PORT, x##_PIN)//makro s parametrem, #slou�� pro odli�en� zbytku textu
#define CLR(x) GPIO_WriteLow(x##_PORT, x##_PIN)

// makra adres/pr�kazu pro lep�� ovl�d�n� MAX7219
#define NOOP 		  0  	
#define DIGIT0 		1	
#define DIGIT1 		2	
#define DIGIT2 		3	
#define DIGIT3 		4	
#define DIGIT4 		5	
#define DIGIT5 		6	
#define DIGIT6 		7	
#define DIGIT7 		8
#define DECODE_MODE 9	// Aktivace/Deaktivace znakov� sady 
#define INTENSITY 	10	// Nastaven� jasu 
#define SCAN_LIMIT 	11	// Volba poctu cifer
#define SHUTDOWN 	12	// Aktivace/Deaktivace displeje (ON / OFF)
#define DISPLAY_TEST 	15	// Aktivace/Deaktivace "testu" (rozsv�t� v�echny segmenty)

// makra argumentu
// argumenty pro SHUTDOWN
#define DISPLAY_ON		1	// zapne displej
#define DISPLAY_OFF		0	// vypne displej
// argumenty pro DISPLAY_TEST
#define DISPLAY_TEST_ON 	1	// zapne test displeje
#define DISPLAY_TEST_OFF 	0	// vypne test displeje
// argumenty pro DECODE_MOD
#define DECODE_ALL		0b11111111//(zap�n� znakovou sadu pro v�echny cifry
#define DECODE_NONE		0 // vyp�n� znakovou sadu pro v�echny cifry

uint8_t sek;//prom�nn� pro ukl�d�n� sekund
uint8_t min;//prom�nn� pro ukl�d�n� minut
uint8_t stav;//prom�nn� pro ukl�d�n� stavu tla��tka



void init(void){
  GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovat MCU na 16MHz
  init_milis();//spu�t�n� milis
  GPIO_Init(GPIOE, GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT);//tlac�tko pro mezi�as
	GPIO_Init(GPIOG, GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);//tla��tko pro zapnut� 
	GPIO_Init(GPIOG, GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);//tla��tko pro pozastaven�
	max7219(DECODE_MODE, DECODE_ALL); // zapnout znakovou sadu na v�ech cifr�ch
  max7219(SCAN_LIMIT, 7); // velikost displeje 8 cifer 
  max7219(INTENSITY, 3); // jas
  max7219(DISPLAY_TEST, DISPLAY_TEST_OFF); // Funkci "test" nechceme m�t zapnutou
  max7219(SHUTDOWN, DISPLAY_ON); // zapneme displej
}

void nuly(void)//Na v�ech digitech se na za��tku zobraz� nuly
{
	max7219(1,0);
	max7219(2,0);
	max7219(3,0);
	max7219(4,0);
	max7219(5,0);
	max7219(6,0);
	max7219(7,0);
	max7219(8,0);
}

void main(void){
 init(); // Inicializace pin�, displeje
 sek = 0; // V�choz� stav pro sekundy
 min = 0; // V�choz� stav pro minuty
 nuly(); // V�ude nastav�me nuly
 stav=0;
 
 
 while(1){
	 if (GPIO_ReadInputPin(GPIOE,GPIO_PIN_4)==RESET){ //tla��tko pro zobrazen� mezi�asu
			//Nastaven� mezi�asu na levou stranu displeje
			max7219(5, sek-(sek/10*10)); //jednotky od sekund
			max7219(6, sek/10); //des�tky od sekund
			max7219(7, min-(min/10*10)); //jednotky od minut
			max7219(8, min/10); //des�tky od minut
	 }
	
	}

}
