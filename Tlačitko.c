TLAČÍTKO

if(GPIO_ReadInputPin(GPIOB,GPIO_PIN_0) == RESET && minule==1){
			x++; 
			minule=0;
		}
		if(GPIO_ReadInputPin(GPIOB,GPIO_PIN_0) != RESET){minule = 1;}

