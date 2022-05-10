#include "stm8s.h"
#include "milis.h"
#include "spse_stm8.h"
#include "stm8_hd44780.h"
#include "stdio.h"

uint8_t x=25;
uint8_t text[32];
char ret1[]="smrdis jak Guvno";

//vlastní znak který můžeme nahrát na LCD
uint8_t time[8]={
0b00000,
0b00000,
0b01000,
0b01000,
0b01000,
0b00100,
0b00010,
0b00000
};

void main(void){
	
CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // 16MHz z interního RC oscilátoru

init_milis(); 
lcd_init();//inicializace LCD

lcd_store_symbol(1,time )//první argument je pozice 0-7, druhý argument je znak který chceme nahrát
lcd_clear();//touto funkcí vyčistíme displej
  while (1){
		
		sprintf(text,"%s","Nějaky věci");//převedeme text na řetězec
		lcd_gotoxy(5,0);//nastavíme polohu
		lcd_puts(text);//na LCD nahrajeme řetězec
		
		sprintf(text,"%u",x);//převedeme číslo na řetězec
		lcd_gotoxy(1,1);
		lcd_puts(text);
	
		sprintf(text,"%s",(uint16_t)ret1);
		lcd_gotoxy(2,1);
		lcd_puts(text);
		
		lcd_gotoxy(2,1);
		lcd_putchar(1);//vložíme custom znak
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