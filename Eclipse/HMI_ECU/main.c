/*
 ================================================================================================
 Name        : main.c
 Author      : Ali Mamdouh
 Description : main file
 Date        : 4/11/2022
 ================================================================================================
 */
// ----------------------------------------------------------------------------
/****************************************************************************
 **************************  Used Libraries  ********************************
 ***************************************************************************/
#include "HMI_ECU.h"
// ----------------------------------------------------------------------------
/****************************************************************************
 ****************************  Main code  ***********************************
 ***************************************************************************/
int main(void)
{
	uint8 Check_Matching = UNMATCHED_PASS; //initialize with unmatched to initially enter Check_Passwords_Matching() function

	init(); //initialize System
	Check_Matching = EnterAndCheckPassword(); //Take Password and return if it is correct or not

	while(1)
	{
		if(Check_Matching == MATCHED_PASS)
		{
			Main_Options();
		}
		else if(Check_Matching == UNMATCHED_PASS)
		{
			EnterPasswordAgain();
		}
		else
		{
			/*Do Nothing*/
		}
	}
}