// STOPKY s 7segment MAX7219 
//Dominik Šlehofer 3.B
#include "stm8s.h"
#include "assert.h"
#include "milis.h"
#include "spse_stm8.h"
#include "stm8_hd44780.h"

//kam zapojím CLK,CS a DIN 
#define CLK_PORT GPIOB
#define CLK_PIN GPIO_PIN_1

#define CS_PORT GPIOB
#define CS_PIN GPIO_PIN_0

#define DIN_PORT GPIOB
#define DIN_PIN GPIO_PIN_2


#define SET(x) GPIO_WriteHigh(x##_PORT, x##_PIN)//makro s parametrem, #slouží pro odlišení zbytku textu
#define CLR(x) GPIO_WriteLow(x##_PORT, x##_PIN)

// makra adres/príkazu pro lepší ovládání MAX7219
#define NOOP 		  0  	
#define DIGIT0 		1	
#define DIGIT1 		2	
#define DIGIT2 		3	
#define DIGIT3 		4	
#define DIGIT4 		5	
#define DIGIT5 		6	
#define DIGIT6 		7	
#define DIGIT7 		8
#define DECODE_MODE 9	// Aktivace/Deaktivace znakové sady 
#define INTENSITY 	10	// Nastavení jasu 
#define SCAN_LIMIT 	11	// Volba poctu cifer
#define SHUTDOWN 	12	// Aktivace/Deaktivace displeje (ON / OFF)
#define DISPLAY_TEST 	15	// Aktivace/Deaktivace "testu" (rozsvítí všechny segmenty)

// makra argumentu
// argumenty pro SHUTDOWN
#define DISPLAY_ON		1	// zapne displej
#define DISPLAY_OFF		0	// vypne displej
// argumenty pro DISPLAY_TEST
#define DISPLAY_TEST_ON 	1	// zapne test displeje
#define DISPLAY_TEST_OFF 	0	// vypne test displeje
// argumenty pro DECODE_MOD
#define DECODE_ALL		0b11111111//(zapíná znakovou sadu pro všechny cifry
#define DECODE_NONE		0 // vypíná znakovou sadu pro všechny cifry

uint8_t sek;//promìnná pro ukládání sekund
uint8_t min;//promìnná pro ukládání minut
uint8_t stav;//promìnná pro ukládání stavu tlaèítka



void init(void){
  GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovat MCU na 16MHz
  init_milis();//spuštìní milis
  GPIO_Init(GPIOE, GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT);//tlacítko pro mezièas
	GPIO_Init(GPIOG, GPIO_PIN_2,GPIO_MODE_IN_PU_NO_IT);//tlaèítko pro zapnutí 
	GPIO_Init(GPIOG, GPIO_PIN_3,GPIO_MODE_IN_PU_NO_IT);//tlaèítko pro pozastavení
	max7219(DECODE_MODE, DECODE_ALL); // zapnout znakovou sadu na všech cifrách
  max7219(SCAN_LIMIT, 7); // velikost displeje 8 cifer 
  max7219(INTENSITY, 3); // jas
  max7219(DISPLAY_TEST, DISPLAY_TEST_OFF); // Funkci "test" nechceme mít zapnutou
  max7219(SHUTDOWN, DISPLAY_ON); // zapneme displej
}

void nuly(void)//Na všech digitech se na zaèátku zobrazí nuly
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
 init(); // Inicializace pinù, displeje
 sek = 0; // Výchozí stav pro sekundy
 min = 0; // Výchozí stav pro minuty
 nuly(); // Všude nastavíme nuly
 stav=0;
 
 
 while(1){
	 if (GPIO_ReadInputPin(GPIOE,GPIO_PIN_4)==RESET){ //tlaèítko pro zobrazení mezièasu
			//Nastavení mezièasu na levou stranu displeje
			max7219(5, sek-(sek/10*10)); //jednotky od sekund
			max7219(6, sek/10); //desítky od sekund
			max7219(7, min-(min/10*10)); //jednotky od minut
			max7219(8, min/10); //desítky od minut
	 }
	
	}

}
