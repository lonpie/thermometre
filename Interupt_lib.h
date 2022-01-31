#pragma origin 4 //OK????????????????
void interuption(void)
{

//sauvegarde registres-------------------------------------------------
	char SVR_W = 0x00;
	char SVR_STATUS = 0x00;
	char SVR_PCLATH = 0x00;

	SVR_W = WREG;

	SVR_STATUS = swap(STATUS);  //####################remplacer swap

	SVR_PCLATH = PCLATH;
//---------------------------------------------------------------------

	PORTA |= 0x04;
	if((PIR2 & 32) != 0)
	{
		PORTA |= 0x04; // mise a 1 de la broche A2
		PIE2 = 0x00;
	}
	PIR2 = 0x00;
	
//chargement registres-------------------------------------------------
	PCLATH = SVR_PCLATH;

	STATUS = swap(SVR_STATUS);      //####################remplacer swap
    
	SVR_W = swap(SVR_W);        //####################remplacer swap
	WREG = swap(SVR_W);     //####################remplacer swap
//---------------------------------------------------------------------

    
	//#asm  //non fonctionnelle? pourtant syntaxe indiqué dans doc XC8
	asm(RETFIE); //syntax OK
	//#endasm

}	

void init_interuption(void)
{
	INTCON = 0xC0; // global interupt enable, Peripheral interupt enable
	PIE2 = 0x20;
	
}

