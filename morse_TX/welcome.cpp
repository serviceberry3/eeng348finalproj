  
/*
 * welcome.cpp
 *
 *  Created on: Jan 3, 2021
 *      Author: nodog
 */

#include "welcome.h"
#include "state_mach.h"

extern volatile state curr_state;

int welcome_user(LiquidCrystal_I2C lcd) {
	while (true) {
		//clear display at set cursor at (0, 0)
		lcd.clear();

		lcd.print(welcome);
		lcd.setCursor(0, 1);
		lcd.write(byte(0));
		lcd.print(cright);

        //check to make sure start button hasn't been pressed
		if (curr_state == TYPING)
			return 0;
		delay(3600);
		if (curr_state == TYPING)
			return 0;


		//check if scrolling is necessary
		if (strlen(welcome) > 16 || strlen(cright) + 1 > 16) {
			for (int i = 0; i < max(strlen(welcome), strlen(cright)+1) - 16; i++) {
				//Serial.println("Scrolling");
				lcd.scrollDisplayLeft();

                //check to make sure start button hasn't been pressed
				if (curr_state == TYPING)
					return 0;
				delay(600);
				if (curr_state == TYPING)
					return 0;
			}
		}

        //check to make sure start button hasn't been pressed
		if (curr_state == TYPING)
			return 0;
		delay(400);
		if (curr_state == TYPING)
			return 0;
	}

	return 0;
}
