/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    main.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include "LPC17xx.H"                    /* LPC17xx definitions               */
#include "GLCD.h"
#include "LED.h"
#include "functions.h"

#define __FI        1                   /* Font index 16x24                  */

OS_TID t_movement;                        /* assigned task id of task: phase_a */
OS_TID t_movement_new;                        /* assigned task id of task: phase_a */
OS_TID t_phaseB;                        /* assigned task id of task: phase_b */
OS_TID t_phaseC;                        /* assigned task id of task: phase_c */
OS_TID t_phaseD;                        /* assigned task id of task: phase_d */
OS_TID t_clock_new;
OS_TID t_clock;                         /* assigned task id of task: clock   */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */

OS_MUT mut_GLCD;                        /* Mutex to controll GLCD access     */
OS_MUT mut_DuckPos;                        /* Mutex to controll GLCD access     */
OS_MUT mut_CrossPos;                        /* Mutex to controll GLCD access     */
OS_MUT mut_Time;
// OS_MUT mut_GLCD;                        /* Mutex to controll GLCD access     */

#define LED_A      0
#define LED_B      1
#define LED_C      2
#define LED_D      3
#define LED_CLK    7

__task void movement (void) {
	while(1){
		CheckPeripheral();
		UpdateScreen();
		os_dly_wait(1);
  }
}
/*----------------------------------------------------------------------------
  Task 5 'clock': Signal Clock
 *---------------------------------------------------------------------------*/
__task void clock (void) {
	
	while (1) {
	 time--;
	 os_dly_wait(100);
	}

	
}
/*----------------------------------------------------------------------------
  Task 7 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  os_mut_init(mut_GLCD);
	os_mut_init(mut_Time);
	
	os_tsk_prio_self(10);
	t_movement  = os_tsk_create (movement, 10);   /* start task clock                 */
	t_clock  = os_tsk_create (clock, 10);   /* start task clock                 */

  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
  Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  LED_Init ();                              /* Initialize the LEDs           */
  GLCD_Init();                              /* Initialize the GLCD           */

  initialSetup();
	
  os_sys_init(init);                        /* Initialize RTX and start init */
}

void checkCollision() {
	int i;
	
	if (((CrossPosX - DuckPosX) > -5 && (CrossPosX - DuckPosX) < 71) && ((CrossPosY - DuckPosY) > -5 && (CrossPosY - DuckPosY) < 55) ) {
		DuckState = 0;
		score += 5;
		bullets = 3;
	  if (duck != 8) {
			LED_On(duck);
			duck += 1;
			
			time = 10;
			
	  } else {
			round += 1;
			duck = 0;
			for (i = 0; i < 8; i++)
				LED_Off(i);
		}
  }
	else
		bullets -= 1;
}


void UpdateScreen() {

	
	//Bullets and Score
	GLCD_SetBackColor(Green);
	//GLCD_ClearLn(9,__FI);
	GLCD_SetTextColor(Black);											

	if (oldscore != score) {
		sprintf(text, "Score:%04u  ", score);       /* format text for print out     */
		GLCD_DisplayString(9, 0, 1, (unsigned char *)text);
		oldscore = score;
	}
	
	if (oldbull != bullets) {
		sprintf(text, "B:%01u  ", bullets);       /* format text for print out     */
		GLCD_DisplayString(9, 16, 1, (unsigned char *)text);
		oldbull = bullets;
	}
	
	//End of Bullets and Score
 	GLCD_SetBackColor(Blue);
 	GLCD_SetTextColor(White);	
	
 	sprintf(text, "%01u  ", (unsigned char *)time);       /* format text for print out     */
 	GLCD_DisplayString(1, 18, 1, text); 


	//if (oldround != round) {
		sprintf(text, "Round:%01u  ", (unsigned char *)round);       /* format text for print out     */
		GLCD_DisplayString(1, 1, 1, text);
		oldround = round;
	//} 
	
	
	if (DuckState)
		GLCD_Bitmap (DuckPosX + deltaX*speed*xscale, DuckPosY + deltaY*speed*yscale, 66, 51, (unsigned char*)duck_bitmap_flip_padd);		
	else
		GLCD_Bitmap (DuckPosX, DuckPosY + deltaY, 60, 52, (unsigned char*)duck_bitmap);

	updateDuckPosition();
	

	//Cross Hairs
	GLCD_Bitmap (CrossPosX , CrossPosY , 2, 12, (unsigned char*)cross_v_clear);						
	GLCD_Bitmap (CrossPosX -5, CrossPosY + 5 , 12, 2, (unsigned char*)cross_h_clear);	
	
	if ((CrossPosX > 310 && joy_deltaX > 0) || (CrossPosX < 10 && joy_deltaX < 0) ) 
		joy_deltaX = 0;
	
	if ((CrossPosY > 170 && joy_deltaY > 0) || (CrossPosY < 5 && joy_deltaY < 0) ) 
		joy_deltaY = 0;

	GLCD_Bitmap (CrossPosX + joy_deltaX, CrossPosY + joy_deltaY, 2, 12, (unsigned char*)cross_v);						
	GLCD_Bitmap (CrossPosX -5 + joy_deltaX, CrossPosY + 5 + joy_deltaY, 12, 2, (unsigned char*)cross_h);	
	
	CrossPosX += joy_deltaX;
	CrossPosY += joy_deltaY;

	
									
}

void getJoystickMovement() {
 int val = 0;
 val = (LPC_GPIO1->FIOPIN >> 20) & KBD_MASK;
 val = (~val & KBD_MASK); /* key pressed is read as a non '0' value*/
	
	joy_deltaX = 0;
  joy_deltaY = 0;
	
	if (val == right){
		joy_deltaX =  joy_sensitivity;
	}else if (val == left){
	  joy_deltaX =  -joy_sensitivity;
	}else if (val == down){
		joy_deltaY =  joy_sensitivity;
	}else if (val == up) {
		joy_deltaY = -joy_sensitivity;
	}else if (val == shoot)
		buttonCycle = PRESSED;
	else
		val = 0;
		
	switch (buttonCycle){
		case PRESSED:
			if (val != shoot)
				buttonCycle = RELEASED;
			break;
		case RELEASED:
				if (bullets)
					checkCollision();
				buttonCycle = OPEN;
			break;
	};
}
	

void getDuckMovement() {

	if (DuckPosX < 5)
		deltaX = 1;
	else if (DuckPosX > 240)
		deltaX = -1;
	
	if (DuckPosY < 5)
		deltaY = 1;
	else if (DuckPosY > 135) {
		deltaY = -1;
	} 	
}


void CheckPeripheral() {
		getDuckMovement();
  	getJoystickMovement();
}

void updateDuckPosition(){
	if (!DuckState) {
		deltaX = 0;
		deltaY= 4;

		if (DuckPosY > 135) {
		 	deltaX = 0;
		  deltaY= 0;
			
			DuckPosX = rand() % 260;
      DuckPosY = 135;
			
			if (rand() % 2)
				deltaX = 1;
			else
				deltaX = -1;
			if (rand() % 2)
				deltaY = 1;
			else
				deltaY = -1;
			xscale = ((float)(rand()%RAND_MAX)/(float)RAND_MAX)/2.0f + 0.5;
			yscale = ((float)(rand()%RAND_MAX)/(float)RAND_MAX)/2.0f + 0.5;
			
			DuckState = 1;
			
		} else 
				DuckPosY += deltaY;

	} else {
		DuckPosX += deltaX*speed;
		DuckPosY += deltaY*speed;	
	}
}

void initialSetup() {
	
	GLCD_Clear(Blue);                         /* Clear graphical LCD display   */
	GLCD_SetBackColor(Green);
	GLCD_ClearLn(8,__FI);
	GLCD_ClearLn(9,__FI);
	GLCD_SetTextColor(Black);											

	sprintf(text, "Score:%04u       ", (unsigned char *)score);       /* format text for print out     */
	GLCD_DisplayString(9, 0, 1, text);
	
	sprintf(text, "B:%01u  ", (unsigned char *)bullets);       /* format text for print out     */
	GLCD_DisplayString(9, 16, 1, text);
	
	GLCD_SetBackColor(Blue);
	 GLCD_SetTextColor(White);	
	
 sprintf(text, "%01u  ", (unsigned char *)time);       /* format text for print out     */
 GLCD_DisplayString(1, 18, 1, text);
	
	sprintf(text, "Round:%01u  ", (unsigned char *)round);       /* format text for print out     */
	GLCD_DisplayString(1, 1, 1, text);
	
	
	GLCD_SetTextColor(Green);											
	GLCD_Bitmap (DuckPosX, DuckPosY, 66, 51, (unsigned char*)duck_bitmap_flip_padd);	
	GLCD_Bitmap (CrossPosX, CrossPosY, 2, 12, (unsigned char*)cross_v);						
	//GLCD_Bitmap (CrossPosX -5, CrossPosY + 5, 12, 2, (unsigned char*)cross_h);	
		GLCD_SetBackColor(Green);
	GLCD_ClearLn(8,__FI);
}

