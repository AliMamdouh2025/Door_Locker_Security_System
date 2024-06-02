/*
 ================================================================================================
 Name        : HMI_ECU.c
 Author      : Ali Mamdouh
 Description : human interface
 Date        : 4/11/2022
 ================================================================================================
 */
// ----------------------------------------------------------------------------
/****************************************************************************
 **************************  Used Libraries  ********************************
 ***************************************************************************/
#include "uart.h"
#include "common_macros.h"
#include "gpio.h"
#include "lcd.h"
#include "keypad.h"
#include <util/delay.h>
#include <avr/io.h>
#include "HMI_ECU.h"
#include "timer1.h"
// ----------------------------------------------------------------------------
/****************************************************************************
 *************************  Global Variables  *******************************
 ***************************************************************************/
uint8 Pass[5];
uint32 g_tick = 0;
uint8 Flag_Of_locking = 1;
uint8 Check_Matching;
uint8 num = 0;
// ----------------------------------------------------------------------------
/****************************************************************************
 ************************  Function Definitions  ****************************
 ***************************************************************************/
void init(void)
{
	SREG |= (1<<7); //Global Interrupt Enable to request interrupt

	UART_ConfigType Uart;
	Uart.baud_rate = 9600;
	Uart.bit_data = Data_8bit;
	Uart.parity = Disable_Parity;
	Uart.stop_bit = Stop_1bit;
	UART_init(&Uart);

	LCD_init();	
}

void EnterPasswordAgain(void)
{
	if(num != 0) //don't enter first time only
	{
		while(UART_recieveByte() != MC2_READY){}
		UART_sendByte(CHECK_PASSWORDS_MATCHING);
		num = 1;
	}
	Check_Matching  = Check_Passwords_Matching();		
}


uint8 EnterAndCheckPassword(void)
{
	Enter_Password(Pass);
	return Check_Passwords_Matching();	
}



void Enter_Password(uint8* pass)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"PLZ Enter Pass:");
	LCD_moveCursor(1,0);
	uint8 i,keypad;
	for(i = 0; i < 5; i++)
	{
		keypad = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		pass[i] = keypad;
		_delay_ms(500); //time of pressing button
	}

	while(KEYPAD_getPressedKey() != 13); //wait until pressing enter
}





uint8 Check_Passwords_Matching(void)
{
	uint8 pass[5];
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Please re-enter the");
	LCD_displayStringRowColumn(1,0,"same Pass:");
	uint8 i,keypad;
	for(i = 0; i < 5; i++)
	{
		keypad = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		pass[i] = keypad;
		_delay_ms(500); //time of pressing button
	}

	while(KEYPAD_getPressedKey() != 13); //wait until enter is pressed

	for(i = 0; i < 5; i++) //sending first password
	{
		while(UART_recieveByte() != MC2_READY){}
		UART_sendByte(Pass[i]);
	}

	for(i = 0; i < 5; i++) //sending second password
	{
		while(UART_recieveByte() != MC2_READY){}
		UART_sendByte(pass[i]);
	}

	UART_sendByte(MC1_READY);
	return UART_recieveByte(); //return matched or not
}





uint8 Check_Password(void)
{
	uint8 i;
	Enter_Password(Pass);
	while(UART_recieveByte() != MC2_READY){}
	UART_sendByte(CHECK_PASSWORD); //to notice MC2 to enter CHECK_PASSWORD() function

	for(i = 0; i < 5; i++) //sending password
	{
		while(UART_recieveByte() != MC2_READY){}
		UART_sendByte(Pass[i]);
	}

	UART_sendByte(MC1_READY);
	return UART_recieveByte(); //return matched or not
}


uint8 Alert(void)
{
	uint8 i;
	for(i = 0; i < 2; i++) //two more trials, so overall trials before alert = 3
	{
		if(Check_Password() == MATCHED_PASS)
		{
			return MATCHED_PASS; //end function and dont alert
		}
	}

	while(UART_recieveByte() != MC2_READY){}
	UART_sendByte(ALERT); //send to MC2 to enter Alert() function

	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"ERORR!!");
	for(i = 0; i < 12; i++)//delay for one minute = 12*2sec.
	{
		_delay_ms(5000); //we want all devices of system to stop during alert, so we use delay not timer1
	}
	return UNMATCHED_PASS;


}


void Main_Options(void)
{
	num++;
	uint8 check,keypad;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");
	keypad = KEYPAD_getPressedKey();
	while(KEYPAD_getPressedKey() != 13); //wait until pressing enter

	if(keypad == '+')
	{
		check = Check_Password();

		if(check == MATCHED_PASS)
		{
			Door_unlocking();
			Door_Open();
			Door_locking();
		}
		else
		{
			if(Alert() == MATCHED_PASS) //if user enter the write password in second or third trial door unlock,else system alert then return to main options
			{
				Door_unlocking();
				Door_Open();
				Door_locking();
			}
		}
	}
	else if(keypad == '-')
	{
		check = Check_Password();

		if(check == MATCHED_PASS)
		{
			Enter_Password(Pass);
			while(UART_recieveByte() != MC2_READY){}
			UART_sendByte(CHECK_PASSWORDS_MATCHING);
			Check_Matching  = Check_Passwords_Matching();
		}
		else
		{
			if(Alert() == MATCHED_PASS) //if user enter the write password in second or third trial door unlock,else system alert then return to main options
			{
				Enter_Password(Pass);
				while(UART_recieveByte() != MC2_READY){}
				UART_sendByte(CHECK_PASSWORDS_MATCHING);
				Check_Matching  = Check_Passwords_Matching();
			}
		}
	}
}







void Timer_On(void(*a_ptr)(void)) //send to timer needed call_back function
{
	Timer1_ConfigType Config_Timer;
	Config_Timer.prescaler = F_CPU_8;
	Config_Timer.mode = compare;
	Config_Timer.initial_value = 1;
	Config_Timer.compare_value = 10000; //which means interupt every 10ms
	Timer1_init(&Config_Timer);
	Timer1_setCallBack(a_ptr);
}





void Timer_Door_locking_unlocking(void)
{
	g_tick++;
	if(g_tick == 1500)  //Enter if condition every 1500*10ms(interrupt time = 10ms) = 15seconds
	{
		g_tick = 0;
		Timer1_deInit(); //Turn off timer after displaying on screen for 15seconds
		Flag_Of_locking = 1; //To exit while loop of Door_unlocking() function
	}
}




void Timer_Door_Open(void)
{
	g_tick++;
	if(g_tick == 300)  //Enter if condition every 300*10ms(interrupt time = 10ms) = 3seconds
	{
		g_tick = 0;
		Timer1_deInit(); //Turn off timer after displaying on screen for 15seconds
		Flag_Of_locking = 1; //To exit while loop of Door_unlocking() function
	}
}





void Door_unlocking(void)
{
	Timer_On(Timer_Door_locking_unlocking); //set Timer_Door_Unlocking() as call_back function
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door Unlocking");
	Flag_Of_locking = 0; //to not overwrite main options over "Door Unlocking" message on LCD
	while(UART_recieveByte() != MC2_READY){}
	UART_sendByte(MOTOR_UNLOCKING); //send to MC2 to unlock door by enter Motor_Unlock() function
	while(Flag_Of_locking == 0){}
}





void Door_locking(void)
{
	Timer_On(Timer_Door_locking_unlocking); //set Timer_Door_Unlocking() as call_back function
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door locking");
	Flag_Of_locking = 0; //to not overwrite main options over "Door Unlocking" message on LCD
	while(UART_recieveByte() != MC2_READY){}
	UART_sendByte(MOTOR_LOCKING); //send to MC2 to lock door by enter Motor_lock() function
	while(Flag_Of_locking == 0){}
}




void Door_Open(void)
{
	Timer_On(Timer_Door_Open); //set Timer_Door_Unlocking() as call_back function
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door is Open");
	Flag_Of_locking = 0; //to not overwrite main options over "Door Unlocking" message on LCD
	while(UART_recieveByte() != MC2_READY){}
	UART_sendByte(MOTOR_STOP); //send to MC2 to open door by enter Motor_Stop() function
	while(Flag_Of_locking == 0){}
}
