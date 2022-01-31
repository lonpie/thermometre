//lcd_lib
#define RS RC0
#define EN RC1
#define D4 RC2
#define D5 RC3
#define D6 RC4
#define D7 RC5
/*
#define RS PORTC.0
#define EN PORTC.1
#define D4 PORTC.2 
#define D5 PORTC.3
#define D6 PORTC.4
#define D7 PORTC.5
*/

//LCD FONCTION************************************************************************
void LCD_setbit(unsigned char data_bit){
	if (data_bit & 1) {
	D4 = 1;
	}
	else {
	D4 = 0;
	}
	
	if (data_bit & 2) {
	D5 = 1;
	}
	else {
	D5 = 0;
	}

	if (data_bit & 4) {
	D6 = 1;
	}
	else {
	D6 = 0;
	}

	if (data_bit & 8) {
	D7 = 1;
	}
	else {
	D7 = 0;
	}

}

void LCD_cmd(unsigned char cmd){
	char cmd_faible;
	char cmd_fort;
	RS = 0;
	
	cmd_faible = cmd & 0x0f;
	cmd_fort = cmd & 0xF0;
	
	LCD_setbit(cmd_fort>>4);
	EN = 1;
	delay_ms(4);
	EN=0;
	
	LCD_setbit(cmd_faible);
	EN = 1;
	delay_ms(4);
	EN = 0;
}


void LCD_char(unsigned char data){
	char data_faible;
	char data_fort;
	RS = 1;
	
	data_faible = data & 0x0f;
	data_fort = data & 0xF0;
	
	LCD_setbit(data_fort>>4);
	EN = 1;
	delay_ms(4);
	EN=0;

	LCD_setbit(data_faible);
	EN = 1;
	delay_ms(4);
	EN = 0;
}

/* syntax declaration tableau char pour LCD_string_data
char string[5];
 	string[0] = 'h';
 	string[1] = 'e';
 	string[2] = 'l';
 	string[3] = 'l';
	string[4] = 'o';
*/

void LCD_string_data(int *data_string, int long_tab){
	uint8_t i = 0;
	for(i=0;i < long_tab; i++)
{
	LCD_char(data_string[i]);
}

}

void LCD_setcursor(char a, char b){
	char x,y;
	char temp;

	if (a==1){
	temp = 0x80 +b-1; //0x80 = premiére ligne
	LCD_cmd(temp);	
	}
	else if(a == 2)
	{
	temp = 0xC0 +b-1; //0xC0 = deuxiéme ligne
	LCD_cmd(temp);
	}
}

void init_LCD(void){
	//--------------Pin_LCD sur port C---------------
	TRISC = 0;  //port C en sortie
	ANSELC = 0; //port C numerique
	PORTC = 0;	//initialisation a 0

	delay_ms(50);

	LCD_cmd(0x02); //forcer mode 4bit

	LCD_cmd(0x23); //forcer en mode 4bit
	LCD_cmd(0x23);
	LCD_cmd(0x23);

	//LCD_cmd(0x01);	//clear LCD	

	LCD_cmd(0x2b);  // function set; mode 4bit, 2 ligne, 5x7 pixels
	LCD_cmd(0xc);	// Dispaly on/off: ecran on, curseur off, clignotement off
	//LCD_cmd(0xF);	// Dispaly on/off: ecran on, curseur on, clignotement on
	LCD_cmd(0x06);  // le curseur se deplace vers la gauche
	LCD_cmd(0x01);	//clear LCD

}
