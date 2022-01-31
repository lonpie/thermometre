// init_lib.h

void init(){

	OSCCON = 0x70;
	OPTION_REG = 0x00;

	//OSCCON = 0x68; //IRCF = 4MHz ; System clock select by <FOSC> config bytes
	//OPTION_REG = 0x08; //pas de prescaler sur le TMR0
	ANSA4 = 0;
    
    
	//ANSELA.4 = 0;
	//ANSELA.5 = 0; //verouiller a 0 le bit ANSELA.5 n'existe pas

//COMPARATEUR-----------------------------------------------------------------
	TRISA0 = 1;
    TRISA1 = 1;
    TRISA2 = 0;
   /* 
    TRISA.0 = 1;
	TRISA.1 = 1;
	TRISA.2 = 0;
    */
    ANSA0 = 1;
    ANSA1 = 1;
    ANSA2 = 1;
    /*
	ANSELA.0 = 1;
	ANSELA.1 = 1;
	ANSELA.2 = 0;
    */
	CM1CON0 = 0x94;  
	CM1CON1 = 0x40;  //interupt OK
///----------------------------------------------------------------------------

	init_LCD();
	init_interuption();

	delay_ms(1000);
}