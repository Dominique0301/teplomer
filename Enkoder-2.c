void process_enc(void){
	static minule=1;
	
	if(GPIO_ReadInputPin(GPIOF,GPIO_PIN_7) == RESET && minule==1){// vpodstatě jako kontrola tlačítka
		minule = 0; 
		if(GPIO_ReadInputPin(GPIOF,GPIO_PIN_6) == RESET){//pokud je "stiskunuto" i druhé tak přičítám
			hodnota++;
		}else{//jinak odčítám
			hodnota--;
		}
	}
	if(GPIO_ReadInputPin(GPIOF,GPIO_PIN_7) != RESET){minule = 1;} 
}