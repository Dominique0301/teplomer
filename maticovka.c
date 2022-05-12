#include "stm8s.h"
#include <stdio.h>
#include "assert.h"
#include "milis.h"
#include "keypad.h"
#include "delay.h"

//Max7219
#define CLK_PORT GPIOG
#define CLK_PIN GPIO_PIN_2

#define CS_PORT GPIOG
#define CS_PIN GPIO_PIN_3

#define DIN_PORT GPIOE
#define DIN_PIN GPIO_PIN_3


#define SET(BAGR) GPIO_WriteHigh(BAGR##_PORT, BAGR##_PIN)
#define CLR(BAGR) GPIO_WriteLow(BAGR##_PORT, BAGR##_PIN)

// makra adres/pr?kazu pro citelnej?? ovl?d?n? MAX7219
#define NOOP 		  0  	// No operation
#define DIGIT0 		1	// z?pis hodnoty na 1. cifru
#define DIGIT1 		2	// z?pis hodnoty na 1. cifru
#define DIGIT2 		3	// z?pis hodnoty na 1. cifru
#define DIGIT3 		4	// z?pis hodnoty na 1. cifru
#define DIGIT4 		5	// z?pis hodnoty na 1. cifru
#define DIGIT5 		6	// z?pis hodnoty na 1. cifru
#define DIGIT6 		7	// z?pis hodnoty na 1. cifru
#define DIGIT7 		8	// z?pis hodnoty na 1. cifru
#define DECODE_MODE 9	// Aktivace/Deaktivace znakov? sady (my vol?me v?dy hodnotu DECODE_ALL)
#define INTENSITY 	10	// Nastaven? jasu - argument je c?slo 0 a? 15 (vet?? c?slo vet?? jas)
#define SCAN_LIMIT 	11	// Volba poctu cifer (velikosti displeje) - argument je c?slo 0 a? 7 (my d?v?me v?dy 7)
#define SHUTDOWN 	12	// Aktivace/Deaktivace displeje (ON / OFF)
#define DISPLAY_TEST 	15	// Aktivace/Deaktivace "testu" (rozsv?t? v?echny segmenty)

// makra argumentu
// argumenty pro SHUTDOWN
#define DISPLAY_ON		1	// zapne displej
#define DISPLAY_OFF		0	// vypne displej
// argumenty pro DISPLAY_TEST
#define DISPLAY_TEST_ON 	1	// zapne test displeje
#define DISPLAY_TEST_OFF 	0	// vypne test displeje
// argumenty pro DECODE_MOD
#define DECODE_ALL		0b11111111 // (lep?? z?pis 0xff) zap?n? znakovou sadu pro v?echny cifry
#define DECODE_NONE		0 // vyp?n? znakovou sadu pro v?echny cifry


//UART komunikace
char putchar (char c)
{
  /* Write a character to the UART1 */
  UART1_SendData8(c);
  /* Loop until the end of transmission */
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);

  return (c);
}

char getchar (void) //funkce cte(prijï¿½mï¿½ data) vstup z UART
{
  int c = 0;
  /* Loop until the Read data register flag is SET */
  while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
	c = UART1_ReceiveData8();
  return (c);
}


//Povoleni UART1 (Vyuzivane na komunikaci s PC)
void init_uart1(void)
{
    UART1_DeInit();         // smazat starou konfiguraci
		UART1_Init((uint32_t)115200, //Nova konfigurace
									UART1_WORDLENGTH_8D, 
									UART1_STOPBITS_1, 
									UART1_PARITY_NO,
									UART1_SYNCMODE_CLOCK_DISABLE, 
									UART1_MODE_TXRX_ENABLE);
}

void max7219(uint8_t address, uint8_t data)
{
		uint16_t mask;
		CLR(CS);
		
		
		//adresa
		mask = 1<<7;
		while(mask){
		 CLR(CLK);
		 if(address & mask){
					 SET(DIN);
				} else {
					 CLR(DIN);
				}
				SET(CLK);
				mask >>=1;
				CLR(CLK);
			}
			
			
		//data
		mask = 1<<7;
		while(mask){
		 CLR(CLK);
		 if(data & mask){
					 SET(DIN);
				} else {
					 CLR(DIN);
				}
				SET(CLK);
				mask >>=1;
				CLR(CLK);
			}
			
			SET(CS);
}


void nic(void) { //Vypsání prázdných míst
	max7219(1, 15);
	max7219(2, 15);
	max7219(3, 15);
	max7219(4, 15);
	max7219(5, 15);
	max7219(6, 15);
	max7219(7, 15);
	max7219(8, 15);
}

void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovat MCU na 16MHz

    init_milis(); //Initializace milis
    init_uart1(); //Povoleni komunikace s PC
		init_keypad(); //Initializace klávesnice
		
		//max7219
		GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
		GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
		GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
		max7219(DECODE_MODE, DECODE_ALL); // zapnout znakovou sadu na v?ech cifr?ch
		max7219(SCAN_LIMIT, 7); // velikost displeje 8 cifer (poc?t?no od nuly, proto je argument c?slo 7)
		max7219(INTENSITY, 3); // vol?me ze zac?tku n?zk? jas (vysok? jas mu?e m?t velkou spotrebu - a? 0.25A !)
		max7219(DISPLAY_TEST, DISPLAY_TEST_OFF); // Funkci "test" nechceme m?t zapnutou
		max7219(SHUTDOWN, DISPLAY_ON); // zapneme displej
		nic();
}


int main(void)
{
    uint8_t key_now = 0xFF;
    uint8_t key_last = 0xFF;
    uint32_t mtime_key = 0;

		uint8_t pozice = 8;

    setup();
    printf("Dobry den,\n\r");
		printf("vitejte v programu poznamkovy blok.\n\r");
		printf("Tlacitky klavesnice muzete vypisovat postupne na jednotlive segmenty cisla.\n\r");
		printf("Tlacitkem * napisete prazdne misto, tlacitkem # napisete vsude prazdna mista a kurzor posunete na zacatek.\r\n");

    while (1) {
        if (milis() - mtime_key > 55) {// detekce stisknuté klávesy
           mtime_key = milis();
           key_now = check_keypad();
           if (key_last == 0xFF && key_now != 0xFF) {
								char x[2];
								sprintf(x, "%x", key_now);
								printf("Klavesa: %c\n\r", x[0]);
								if (x[0] == 'a') {
										max7219(pozice, 15);
								}
								else if (x[0] == 'b') {
									nic();
									pozice = 9;
								}
								else {
									max7219(pozice, x[0]);
								}
								pozice = pozice - 1;
								if(pozice == 0) {
									pozice = 8;
								}
            }
            key_last = key_now;
        }
    }
}