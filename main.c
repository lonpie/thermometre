
/*
***Thermometre numerique VERSION 2 TEST CHECKSUM***
**********Test Bitfield**************
************01.05.2021***************
************PIC16F1503***************
*********Compilateur CC5X************
**********MPLAB IDE 8.92*************
*
*ERREUR DE CHECKSUM SANS ERREUR APPARENTE DE LECTURE DES DATA
* --> l'octet de checksum est a O a chaque fois que le probleme est rencontré.
*    -peut être une erreur de temps de reponse trop long du bus data DHT: vu a l'analyseur logique, 
*     les delais entre chaque octets transmits sont suseptible de varier (TYP = 50us  DERIVE_meusré = 67us)
*	 --> Ajouter une gestion des erreurs de temps de reponse pour valider cette hypothese.
*		 - création d'une variable globale DE "Delai Expiré" dans le  même but que les variables CS et TO,
*		 - la remonter et la tracabilité des erreur de fonctionnement.
*			/!\ le fait de passer des parametre lors de l'appel de la fonction data_read() a l'air de provoquer
*				encore plus d'erreur de CS. Peut être que le temps de passer les parametres à la fonction
*				fait rater des bits data.
*
*	--> Augmenter par 2 ou 4 INTOSC et mettre a 2 ou 4 le prescaler du TMR0 dans OPTION_REG
		-ceci permetrait de diminuer la durée des cycles d'instruction tout en gardant un TMR0 s'incrementant
*		 chaque us.
*			--> OSCCON = 0x70 (8MHz, SCS = Confi Bit) ou 0x78 (16MHz, SCS = Config Bit)
*			--> OPTION_REG = 0x00 (TMR0 prescler 1:2) ou 0x01 (TMR0 prescaler 1:4)
*		/!\/!\/!\ Ca a l'ai de fonctionner a 8MHz : aucune erreur de checksum générer depuis plusieurs minutes 
*				-peut étre que l'utilisation de la variable erreur DE peut être utilisable a 8 ou 16 Mhz
*					(interet??  --> après coup plus aucun intérêt)	
*
*AJOUT DE HEADERS: -Interuption_lib.h --> pour la l'initialisation et gestion des interuption 
*											(capteur mouvement, control retroeclairage)
*						--> création variable char "tours", afin de gerer l'activation des interuptions et ne pas être en permanece
							bloqué dans ces derniéres
*
*				   -Init_lib.h --> pour séparer la fonction init de main.c
*	
*/

#include <xc.h>

#include "Interupt_lib.h"
#include "Delay_lib.h"
#include "Lcd_lib.h"
#include "Dht22_lib.h"
#include "Init_lib.h"

int toto = _PCL_PCL_MASK;

char T_byte1 ; 
char T_byte2  ;
char RH_byte1 ;
char RH_byte2  ;
char SUM ;

char tours;

bit verif_data(){
	
    
    
    unsigned int check = ((T_byte1 + T_byte2 + RH_byte1 + RH_byte2) & 0xFF);

	if(SUM != check)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}

void main()
{
	tours = 0x00;
	init();
	
	char temperature_int[16];
 	temperature_int[0] = 'I';
 	temperature_int[1] = 'N';
 	temperature_int[2] = 'T';
	temperature_int[3] = ' ';
	temperature_int[4] = 'X';
	temperature_int[5] = 'X';
 	temperature_int[6] = '.';
 	temperature_int[7] = 'X';
 	temperature_int[8] = 'C';
	temperature_int[9] = ' ';
	temperature_int[10] = 'X';
	temperature_int[11] = 'X';
	temperature_int[12] = '.';
	temperature_int[13] = 'X';
	temperature_int[14] = '%';
	temperature_int[15] = ' ';

	char temperature_ext[16];
  	temperature_ext[0] = 'E';
 	temperature_ext[1] = 'X';
 	temperature_ext[2] = 'T';
	temperature_ext[3] = ' ';
	temperature_ext[4] = 'X';
	temperature_ext[5] = 'X';
 	temperature_ext[6] = '.';
 	temperature_ext[7] = 'X';
 	temperature_ext[8] = 'C';
	temperature_ext[9] = ' ';
	temperature_ext[10] = 'X';
	temperature_ext[11] = 'X';
	temperature_ext[12] = '.';
	temperature_ext[13] = 'X';
	temperature_ext[14] = '%';
	temperature_ext[15] = ' ';

	delay_ms(2000); //wait 2s pour passer l'etat instable du DHT22


	while(1)
	{	
		char pin = 16;
		
		char TO = 0x00;
		char CS = 0x00;

		INTCON = 0x00;  //désactivation toutes interuptions

		int i = 0;
		for(i = 0; i < 2; i++)
		{
			T_byte1 = 0x00;
			T_byte2 = 0x00;
			RH_byte1 = 0x00;
			RH_byte2 = 0x00;
			SUM = 0x00;
			
			DHT_start_signal(pin); //signal start MCU -> dht22

			if(DHT_check_rep_int(pin)  == 1)   //DHT_check_rep_int()
			{   	
				RH_byte1 = read_data(pin);
				RH_byte2 = read_data(pin);
				T_byte1 = read_data(pin);
				T_byte2 = read_data(pin);
				SUM = read_data(pin);
				
				delay_ms(2);
				if(verif_data() == 0)
				{
					CS |= pin;
				}


				uint16_t temp = 0x00;
//************************************************** TEMP INT
				if(pin == 16)
				{
					temp = T_byte1;
					temp = (temp << 8);
					temp = temp | T_byte2;
					
					if(T_byte1.7 == 1)  //si temperature negative
					{
						temperature_ext[5] = '-';
						T_byte1.7 = 0;
					}

					temperature_int[4] = temp / 100;
					temperature_int[4] = temperature_int[4]  % 10;  
					temperature_int[4] = temperature_int[4] + 48;
	
					temperature_int[5] = temp / 10;
					temperature_int[5] = temperature_int[5] % 10;
					temperature_int[5] = temperature_int[5] + 48;

					temperature_int[7] = temp % 10 ;
					temperature_int[7] = temperature_int[7] + 48;
//****************************************************************************
					uint16_t hr = 0x00;
					hr = RH_byte1;
					hr = (hr << 8);
					hr = hr | RH_byte2;

					temperature_int[10] = (hr / 100);
					temperature_int[10] = temperature_int[10]  % 10;   
					temperature_int[10] = temperature_int[10] + 48;

					temperature_int[11] = (hr / 10);
					temperature_int[11] = temperature_int[11] % 10;
					temperature_int[11] = temperature_int[11] + 48;
		
					temperature_int[13] = hr % 10 ;
					temperature_int[13] = temperature_int[13] + 48;

					

/*
					temperature_int[11] = (SUM / 100);			//affichage checksum
					temperature_int[11] = temperature_int[11]  % 10;   
					temperature_int[11] = temperature_int[11] + 48;

					temperature_int[12] = (SUM / 10);
					temperature_int[12] = temperature_int[12] % 10;
					temperature_int[12] = temperature_int[12] + 48;
		
					temperature_int[14] = SUM % 10 ;
					temperature_int[14] = temperature_int[14] + 48;
*/
				
//************************************************************************
				}
				else
				{
//****************************************************  TEMP EXT
					temp = T_byte1;
					temp = (temp << 8);
					temp = temp | T_byte2;
					
					if(T_byte1.7 == 1)  //si temperature negative
					{
						temperature_ext[5] = '-';
						T_byte1.7 = 0;
					}

					temperature_ext[4] = temp / 100;
					temperature_ext[4] = temperature_ext[4]  % 10;  
					temperature_ext[4] = temperature_ext[4] + 48;
	
					temperature_ext[5] = temp / 10;
					temperature_ext[5] = temperature_ext[5] % 10;
					temperature_ext[5] = temperature_ext[5] + 48;

					temperature_ext[7] = temp % 10 ;
					temperature_ext[7] = temperature_ext[7] + 48;

//****************************************************************************
					uint16_t hr = 0x00;
					hr = RH_byte1;
					hr = (hr << 8);
					hr = hr | RH_byte2;

					temperature_ext[10] = (hr / 100);
					temperature_ext[10] = temperature_ext[10]  % 10;   
					temperature_ext[10] = temperature_ext[10] + 48;

					temperature_ext[11] = (hr / 10);
					temperature_ext[11] = temperature_ext[11] % 10;
					temperature_ext[11] = temperature_ext[11] + 48;
		
					temperature_ext[13] = hr % 10 ;
					temperature_ext[13] = temperature_ext[13] + 48;

				
				/*	temperature_ext[11] = (SUM / 100);            //affichage checksum
					temperature_ext[11] = temperature_ext[11]  % 10;   
					temperature_ext[11] = temperature_ext[11] + 48;

					temperature_ext[12] = (SUM / 10);
					temperature_ext[12] = temperature_ext[12] % 10;
					temperature_ext[12] = temperature_ext[12] + 48;
		
					temperature_ext[14] = SUM % 10 ;
					temperature_ext[14] = temperature_ext[14] + 48;
				*/
//************************************************************************
				}
			
				//LCD_cmd(0x01);		
			}
			else
			{		
				TO |= pin;
			//	LCD_char('x'); //**************debug************ pas de reponse
			}
		pin *=2;
		}
		INTCON = 0xC0;  //Activation interuptions

	/*	LCD_setcursor(1,0);
		LCD_string_data(temperature_int, 13);	
		LCD_setcursor(2,0);
		LCD_string_data(temperature_ext, 13);
*/

		//LCD_cmd(0x01);

//-------------------------------------------------------- Gestion variable ERREUR 16----------------------
	/*
		if((DE & 16) == 1)
		{
			LCD_setcursor(1,10);
			LCD_char('D');
		}
*/
		if((TO & 16) == 0 )
		{
			if((CS & 16) == 0)
			{		
				LCD_setcursor(1,1);
				LCD_string_data(temperature_int, 15);	
				LCD_setcursor(1,16);
				LCD_char(' ');
			}
			else
			{	// CHECK SUM
		
			LCD_setcursor(1,1);  				 // DEBUG CHECKSUM
			LCD_string_data(temperature_int, 15);	// DEBUG CHECKSUM
	
			LCD_setcursor(1,16);
			LCD_char('C');
			}
		}
		else
		{	// TIME OUT
			LCD_setcursor(1,10);
			LCD_char('T');
		}


//-------------------------------------------------------- Gestion variable ERREUR 32----------------------
	/*	if((DE & 32) == 1)
		{
			LCD_setcursor(2,10);
			LCD_char('D');
		}
*/

		if((TO & 32) == 0)
		{
			if((CS & 32) == 0)
			{	
				LCD_setcursor(2,0);
				LCD_string_data(temperature_ext, 15);
				LCD_setcursor(2,16);
				LCD_char(' ');
				}
			else
			{	LCD_setcursor(2,0);					//DEBUG CHECK SUM
				LCD_string_data(temperature_ext, 15);  //DEBUG CHECK SUM
				LCD_setcursor(2,16);
				LCD_char('C');
			}
		}
		else
		{	// TIME OUT
			LCD_setcursor(2,10);
			LCD_char('T');
		}		


		tours ++;
		if(tours > 21) // 7  =  version fonctionelle avant 27.07.21
		{
			tours = 0;
			PORTA &= 0xFB;
			PIE2 = 0x20;	
		}

	delay_ms(8000);

	}
	
}