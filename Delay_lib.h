//fonction a TMR0 a 4MHz
/* parametrage INTOSC et TMR0
	OPTION_REG prescaler pour une us par incrementation 
*/

void delay_us(int us){
	OPTION_REG = 0x00; //prescaler TMR0 1:2
	TMR0 = 18;
	do{
	}
	while(TMR0 < us);
} 

void delay_ms(long ms)
{
	OPTION_REG = 0x00; //prescaler TMR0 1:2
	long i;
	long MS = ms *4;  //4 * 255us (TMR0) =~ 1ms
	for(i = 0; i < MS; i++)
	{

	TMR0 = 0;
	do
	{	
	}
	while(TMR0 < 236);  //228
	} 

}

