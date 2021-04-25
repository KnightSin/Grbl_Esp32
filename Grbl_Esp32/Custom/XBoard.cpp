/*
	XBoard.cpp

	copyright (c) 2021	Xie Bin @Knight_sin

	Grbl is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Grbl is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../src/Grbl.h"

void machine_init()
{
	pinMode(STAT_LED_PIN, OUTPUT);
	digitalWrite(STAT_LED_PIN, HIGH);
}

#if defined(MACRO_BUTTON_0_PIN) || defined(MACRO_BUTTON_1_PIN) || defined(MACRO_BUTTON_2_PIN)
/*
  options.  user_defined_macro() is called with the button number to
  perform whatever actions you choose.
*/
void user_defined_macro(uint8_t index) {
	switch (index) {
#ifdef MACRO_BUTTON_0_PIN
	case 0:
		// grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Info, "0");
		// WebUI::inputBuffer.push("[ESP220]/file0.gcode\r");  // run SD card file0
		break;
#endif
#ifdef MACRO_BUTTON_1_PIN
	case 1:
		// grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Info, "1");
		// WebUI::inputBuffer.push("[ESP220]/file1.gcode\r");  // run SD card file1/
		break;
#endif
#ifdef MACRO_BUTTON_2_PIN
	case 2:
		// grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Info, "2");
		// WebUI::inputBuffer.push("[ESP220]/file2.gcode\r");  // run SD card file2
		break;
#endif
#ifdef MACRO_BUTTON_3_PIN
	case 3:
		// grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Info, "3");
		// WebUI::inputBuffer.push("[ESP220]/file3.gcode\r");  // run SD card file3
		break;
#endif
	default:
		break;
	}
}
#endif
