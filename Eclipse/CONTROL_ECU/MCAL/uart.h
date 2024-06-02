/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Ali Mamdouh
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MC1_READY 0xF0
#define MC2_READY 0xFF

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	Data_5bit,Data_6bit,Data_7bit,Data_8bit,Data_9bit = 7
}UART_BitData;

typedef enum
{
	Disable_Parity,Even_Parity = 2,Odd_Parity ///////////what is parity
}UART_Parity;

typedef enum
{
	Stop_1bit,Stop_2bits
}UART_StopBit;

typedef uint32 UART_BaudRate;

typedef struct{
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_BaudRate baud_rate;
}UART_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
