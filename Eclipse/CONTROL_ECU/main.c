/*
 ============================================================================
 Name        : main.c
 Author      : Ali Mamdouh
 Description : main file
 Date        : 29/10/2022
 ============================================================================
 */
// ----------------------------------------------------------------------------
/****************************************************************************
 **************************  Used Libraries  ********************************
 ***************************************************************************/
#include "CONTROL_ECU.h"
// ----------------------------------------------------------------------------
/****************************************************************************
 ****************************  Main code  ***********************************
 ***************************************************************************/
int main(void)
{
	uint8 Check_Matching = UNMATCHED_PASS; //initialize with unmatched to initially enter Check_Passwords_Matching() function
	uint8 choose_function; //to enter dedicated function

	init(); //initialize System
	
	while(1)
	{
		if(Check_Matching == UNMATCHED_PASS) //before saving password, passwords could be unmatched many times, so we enter this function (Check_Passwords_Matching())			// after saving password, passwords could be unmatched three times only, so we enter Check_Password() function in the switch statement
		{
			Check_Matching = Check_Passwords_Matching();
		}
		else if(Check_Matching == MATCHED_PASS) //will enter the if condition after checking that the two passwords matched(after storing password in eeprom)
		{
			choose_function = InitializeCommunication();
			SelectAction(choose_function); //Select which function to enter
		}
		else 
		{
			/*Do Nothing*/
		}
	}
}