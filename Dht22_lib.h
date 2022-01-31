//dht22.h
#define DT_int PORTA.5 //broche data DHT exterieur

#define DT_int_dir TRISA.5 //broche data int direction




void DHT_start_signal(char pin){
	

	TRISA &= (pin ^ 0x3F);// mise a zero du bit pin dans le registre TRISA
	//	DT_int_dir = 0; //sorti

	
	PORTA &= (pin ^ 0x3F); // mise a zero du bit pin dans le registre PORTA
	//	DT_int = 0; //niveau bas

	delay_ms(2);
	PORTA |= pin; // mise a un du bit pin dans le registre PORTA
	//	DT_int = 1; // niveau haut
	
	delay_us(30); 
	TRISA |= pin; // mise a un du bit pin dans le registre PORTA
	//	DT_int_dir = 1; //input
	
}

bit DHT_check_rep_int(char pin){
	OPTION_REG = 0x00; //prescaler TMR0 1:2
	TMR0 = 0;

	while((PORTA & pin) != 0 && TMR0 < 100)  //40
	{
		if(TMR0 > 80)
		{
			return 0;
		}
	}
	
	TMR0 = 0;
	while((PORTA & pin) == 0 && TMR0 < 100) //200
	{	
		if(TMR0 > 90)//100
		{
		return 0;
		}
	}

	TMR0 = 0;
	while((PORTA & pin) != 0 && TMR0 < 100)
	{
		if(TMR0 > 90)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}


unsigned int read_data(char pin){ 
//unsigned int data_r; : 8 = 0x00;
    uint8_t data_r = 0x00;
    int i;
    uint8_t pos = 128;
    for (i = 0; i < 8; i++)
    {
     
    TMR0 = 0x00;
    while ((PORTA & pin) != 0 && TMR0 < 110){NOP();} //***********
    
    if (TMR0 > 120) //100
    {
     	//Pas de reponse capteur TLOW 1
		//	LCD_setcursor(1,16);
		//	LCD_char('D');
		   return data_r;
     }
      
    TMR0 = 0x00;
    
    while ((PORTA & pin) == 0 && TMR0 < 100 ){NOP();}   //80
    
    if (TMR0 > 90) //60
    {
		 //time out
		//	LCD_setcursor(1,16);
		//	LCD_char('D');
		 return data_r;
     
      
    }
     
    TMR0 = 0x00;
    
    while ((PORTA & pin) != 0 && TMR0 < 200){NOP();}
 

    if (TMR0 > 120) //100  
    {
		// Pas de reponse capteur TH1
		//	LCD_setcursor(1,16);
		//	LCD_char('D');
     	  return data_r;
  	
      
    }
    else if (TMR0 > 50)
    {
      data_r = data_r | pos;  // If time > 40us, Data is 1
      //TCCR2B = 0x01;    
    }    

	pos = pos / 2;
  }
  return data_r;

}

