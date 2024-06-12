/*
 ============================================================================
 Name        : CONTROL_ECU.c
 Author      : Ali Mamdouh
 Description : machine interface
 Date        : 29/10/2022
 ============================================================================
 */
// ----------------------------------------------------------------------------
/****************************************************************************
 **************************  Used Libraries  ********************************
 ***************************************************************************/ 
#include <util/delay.h> 
#include "timer1.h"
#include "common_macros.h" 
#include <avr/io.h>
#include <avr/interrupt.h> 
#include "gpio.h"
#include "DC_Motor.h"
#include "external_eeprom.h"
#include "twi.h"
#include "Buzzer.h"
#include "uart.h"
#include "CONTROL_ECU.h"
// ----------------------------------------------------------------------------
/****************************************************************************
 *************************  Global Variables  *******************************
 ***************************************************************************/
uint32 g_tick = 0;
uint8 Flag_Of_locking = 1;
// ----------------------------------------------------------------------------
/****************************************************************************
 ************************  Function Definitions  ****************************
 ***************************************************************************/
void init(void)
{
	/*Global Interrupt Enable to request interrupt*/
	SREG |= (1<<7); 

	/*UART initialization*/
	UART_ConfigType Uart;
	Uart.baud_rate = 9600;
	Uart.bit_data = Data_8bit;
	Uart.parity = Disable_Parity;
	Uart.stop_bit = Stop_1bit;
	UART_init(&Uart);

	/*I2C initialization*/
	TWI_ConfigType configTWI = {0b00000010, Bit_Rate_400K};
	TWI_init(&configTWI);

	/*Buzzer initialization*/
	Buzzer_init();

	/*Motor initialization*/
	DcMotor_Init();
}





uint32 InitializeCommunication(void)
{
	UART_sendByte(MC2_READY); //Send Ready Ack To MCU1 to alert it that MCU2 can receive data to prevent UART hardware buffer overflow.
	return UART_recieveByte(); //recieve from MC1 which function to enter	
}





void SelectAction(uint32  choose_function)
{
	switch (choose_function)
	{
	case CHECK_PASSWORD:
		Check_Password();
		break;

	case ALERT:
		Alert();
		break;

	case MOTOR_UNLOCKING:
		Motor_Unlock();
		break;

	case MOTOR_LOCKING:
		Motor_lock();
		break;

	case MOTOR_STOP:
		Motor_Stop();
		break;

	case CHECK_PASSWORDS_MATCHING:
		Check_Passwords_Matching();
		break;

	}
}





uint8 Check_Passwords_Matching(void)
{
	uint8 i, pass1[5], pass2[5];
	for(i = 0 ; i < 5 ; i++) //Recieve first password
	{
		UART_sendByte(MC2_READY); //Send Ready Ack To MCU1 to alert it that MCU2 can receive data to prevent UART hardware buffer overflow.
		pass1[i] = UART_recieveByte();
	}

	for(i = 0 ; i < 5 ; i++) //Recieve second password
	{
		UART_sendByte(MC2_READY); //Send Ready Ack To MCU1 to alert it that MCU2 can receive data to prevent UART hardware buffer overflow.
		pass2[i] = UART_recieveByte();
	}

	for(i = 0 ; i < 5 ; i++) //Check that the two passwords are equal
	{
		if(pass1[i] != pass2[i])
		{
			while(UART_recieveByte() != MC1_READY){} //Get Ready Ack from MCU1 to prevent UART hardware buffer overflow and send data to it.
			return UNMATCHED_PASS;
		}
	}

	for(i=0; i<5; i++) //store password in eeprom after checking that the two passwords are matched
	{
		EEPROM_writeByte(0x0311+i, pass1[i]);
		_delay_ms(10);
	}

	while(UART_recieveByte() != MC1_READY){} //Get Ready Ack from MCU1 to prevent UART hardware buffer overflow and send data to it.
	UART_sendByte(MATCHED_PASS);
	return MATCHED_PASS;
}





void Check_Password(void)
{
	uint8 i, pass[5], pass2[5];

	for(i = 0; i < 5; i++) //Taking password from MC1(from user)
	{
		UART_sendByte(MC2_READY); //Get Ready Ack from MCU1 to prevent UART hardware buffer overflow and send data to it.
		pass2[i] = UART_recieveByte();
	}

	for(i = 0; i < 5; i++) //Taking password from eeprom
	{
		EEPROM_readByte(0x0311+i, pass+i);
		_delay_ms(10);
	}

	for(i = 0; i < 5; i++) //Comparing the two passwords
	{
		if(pass[i] != pass2[i])
		{
			while(UART_recieveByte() != MC1_READY){} //Get Ready Ack from MCU1 to prevent UART hardware buffer overflow and send data to it.
			UART_sendByte(UNMATCHED_PASS);
			return;
		}
	}

	while(UART_recieveByte() != MC1_READY){} //Send Ready Ack To MCU1 to alert it that MCU2 can receive data to prevent UART hardware buffer overflow.
	UART_sendByte(MATCHED_PASS);
}






void Alert(void)
{
	uint8 i;
	Buzzer_on();
	for(i = 0; i < 12; i++)//delay for one minute = 12*5sec.
	{
		_delay_ms(5000); //we want all devices of system to stop during alert, so we use delay not timer1
	}
	Buzzer_off();
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





void Motor_Unlock(void)
{
	Timer_On(Timer_Door_locking_unlocking);
	DcMotor_Rotate(CW,100);
	Flag_Of_locking = 0;
	while(Flag_Of_locking == 0){}
	DcMotor_Rotate(STOP,0);
}





void Motor_lock(void)
{
	Timer_On(Timer_Door_locking_unlocking);
	DcMotor_Rotate(A_CW,100);
	Flag_Of_locking = 0;
	while(Flag_Of_locking == 0){}
	DcMotor_Rotate(STOP,0);
}





void Motor_Stop(void)
{
	Timer_On(Timer_Door_Open);
	DcMotor_Rotate(STOP,0);
	Flag_Of_locking = 0;
	while(Flag_Of_locking == 0){}
}
