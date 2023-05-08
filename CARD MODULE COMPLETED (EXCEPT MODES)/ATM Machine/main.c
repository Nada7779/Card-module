
#include "application/app.h"
#include <util/delay.h>
#define F_CPU   8000000u
uint8_t ad=0x00,ad2=0x10,str1[17]={0},str2[5]={0},ad3=0b1010000,ad4=0b1010001;
	
int main(void)
{
//*****************************************************************************************************
	SPI_MasterInit(PRESCALER_16);
	// Programming mode
	DIO_INITPIN(pinb0,OUTPUT);
	DIO_WRITEPIN(pinb0,HIGH);
	
	LCD_INIT();
	USART_init();
	I2C_init(I2C_PRESCALER_16);
//********************************************************************************************************



//********************************************************************************************************
   // pan SEND AND RECIEVE
   uint8_t panRecieved='0';
   char *pan;
   while (panRecieved=='0')
   {
	   	// CHECK PAN CONDITION
	   	unsigned char arr_pan[16]={0};
	   	USART_Transmit_string ("Please Enter Card PAN:");
	  pan = USART_receive_string(arr_pan);
	if (arr_pan[16] == 0 && arr_pan[15]!= 0) 
	{
	 panRecieved='1';
	 	 for (int y=0;y<16;y++)
	 	 {
		 	 if (arr_pan[y]<48 || arr_pan[y]>57) 
			  {
				  USART_transmit('\n');  
				  panRecieved='0';
			  }
	 	 }
		  
	}
	 if (panRecieved=='0') USART_Transmit_string ("Wrong PAN ");
   }
   char arr_pan[17]={0};
	  for (int i=0;i<16;i++)
	  {
		  arr_pan[i]=pan[i];
	  }
	  
   
   
//**************************************************************************************************************
	// PIN RECIEVE AND SEND
	uint8_t pinRecieved='0';
	char *pin;
	while (pinRecieved=='0')
	{
	// PIN RECIEVE 
	unsigned char arr_pin[7]={0};
	unsigned char arr_npin[7]={0};
	USART_Transmit_string ("Please Enter New PIN:");
	pin =  USART_receive_string(arr_pin);
	//**************************************************
	// CONFIRM PIN RECIEVE
	USART_Transmit_string ("Please Confirm New PIN:");
	char *new_pin = USART_receive_string(arr_npin);
   //***********************************************
   // CHECK PIN CONDITION
	if (arr_pin[4] == 0 && arr_pin[3]!= 0) 
	{
	 pinRecieved='1';
	 	 for (int y=0;y<4;y++)
	 	 {
		 	 if (arr_pin[y]<48 || arr_pin[y]>57) 
			  {
				  USART_transmit('\n');  
				  pinRecieved='0'; 
			  }
			 
	 	 }
	}
	
	//**************************************************************
	// CHECK PIN CONFIRMATION
		uint8_t npin = *(new_pin);
		uint8_t pin_in = *(pin);
		int c=0;
		for (c=0;c<4;c++)
		{
			if ((*(new_pin+c)) != (*(pin+c)) ) 
			{ 
				pinRecieved='0';
			}
		}
		if (pinRecieved=='0') USART_Transmit_string ("Wrong PIN "); 	 
	}
	   char arr_pin[5]={0};
	   for (int i=0;i<4;i++)
	   {
		   arr_pin[i]=pin[i];
	   }
//*******************************************************************************************************************
		EEPROM_Write_WithSize(arr_pan,&ad,16); //--- pan sent to EEPROM	
		_delay_ms(100);
		 //--- pan receive to EEPROM
		EEPROM_Write_WithSize(arr_pin,&ad2,16);// PAN sent to EEPROM
		_delay_ms(100);
		EEPROM_Read_N(0x00,str1,16);
		EEPROM_Read_N(0x10,str2,4);
		//EEPROM_Read_N(0x10,str2);
	   USART_Transmit_string(str1);
	   USART_Transmit_string(str2);
// user mode
	DIO_WRITEPIN(pinb0,LOW);
	
	// sending 
	uint8_t index = 0;
	DIO_WRITEPIN(SS, LOW);
	

	while (str1[index] != '\0')
	{
		SPI_SendReceive(str1[index]);
		index++;
		_delay_ms(800);
	}
	SPI_SendReceive('\0');

	
	
index = 0;
	while (str2[index] != '\0')
	{
		SPI_SendReceive(str2[index]);
		index++;
		_delay_ms(800);
	}
	SPI_SendReceive('\0');

	DIO_WRITEPIN(SS, HIGH);
	
	
//USART_init();
//USART_Transmit_string(str2);



	
}
	
