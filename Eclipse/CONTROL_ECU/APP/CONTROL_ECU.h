/*
 ============================================================================
 Name        : CONTROL_ECU.h
 Author      : Ali Mamdouh
 Description : machine interface
 Date        : 29/10/2022
 ============================================================================
 */
#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MATCHED_PASS 0x01
#define UNMATCHED_PASS 0x00
#define CHECK_PASSWORD 0x02
#define ALERT 0x03
#define MOTOR_UNLOCKING 0x04
#define MOTOR_LOCKING 0x05
#define MOTOR_STOP 0x06
#define CHECK_PASSWORDS_MATCHING 0x07

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint32 g_tick;
uint8 Flag_Of_locking;

/*******************************************************************************
 *                         Functions Prototypes                                *
 *******************************************************************************/
/*
 * Description : it check if the two passwords are matched or not
 *if the two passwords are matched the function store the password in the eeprom
 */
uint8 Check_Passwords_Matching(void);
/*
 * Description : it check if the entered password matches the password saved in eeprom or not
 */
void Check_Password(void);
/*
 * Description : it alert if the entered password doesnt match the password saved in eeprom for three times
 */
void Alert(void);

uint32 InitializeCommunication(void);
void init(void);
void SelectAction(uint32  choose_function);
void Timer_On(void(*a_ptr)(void));
void Timer_Door_locking_unlocking(void);
void Motor_Unlock(void);
void Motor_lock(void);
void Motor_Stop(void);

#endif /* CONTROL_ECU_H_ */
