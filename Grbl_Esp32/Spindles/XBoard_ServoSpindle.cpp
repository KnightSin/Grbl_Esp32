/*
    XBoard_ServoSpindle.cpp

	使用M3、M5指令控制SG90舵机，M5满转，M3不转。

    Part of Grbl_ESP32
    2020 -	Bart Dring

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
#include "SpindleClass.h"
#include "math.h"
// ========================= XBoard_ServoSpindle ==================================
/*
    This is a sub class of PWMSpindle but is a digital rather than PWM output
*/

void XBoard_ServoSpindle::init() {
#ifndef SPINDLE_OUTPUT_PIN
	return;
#else
	output_pin = SPINDLE_OUTPUT_PIN;
	spindle_pwm_chan_num = 0; // Channel 0 is reserved for spindle use
	pwm_freq = 50;
	pwm_precision = 10;
	max_angle = xboard_servo_max_angle->get();
#endif

	ledcSetup(spindle_pwm_chan_num, (double)pwm_freq, pwm_precision); // setup the channel
	ledcAttachPin(output_pin, spindle_pwm_chan_num); // attach the PWM to the pin

    config_message();
	set_state(SPINDLE_DISABLE, 0);
}

// prints the startup message of the spindle config
void XBoard_ServoSpindle:: config_message() {
    grbl_msg_sendf(CLIENT_SERIAL,
                   MSG_LEVEL_INFO,
				   "XBoard Servo Spindle Output:%s",
				   pinName(output_pin).c_str());
}


// 0.5ms-----0°:  2.5% 
// 1.0ms----45°:  5.0% 
// 1.5ms----90°:  7.5%
// 2.0ms---135°: 10.0%
// 2.5ms---180°: 12.5%
// duty = {[0.5ms + (Angle/45.0°)*0.5ms] / 20.0ms} * pow(2, pwm_precision)

uint32_t XBoard_ServoSpindle::angle2duty(float angle)
{
	return (uint32_t)(((0.5 + (angle/45.0)*0.5) / 20.0)*pow(2, pwm_precision));
}

uint32_t XBoard_ServoSpindle::set_rpm(uint32_t rpm) {
    static uint32_t recoder_rpm = 0;
	
	if (output_pin == UNDEFINED_PIN)
        return rpm;

	if(recoder_rpm == rpm)return rpm; // 不增加此部分会报错：Guru Meditation Error: Core 1 panic'ed (Coprocessor exception)

	recoder_rpm = rpm;

	sys.spindle_speed = rpm;

	if (rpm < 1){rpm = 1;}
	else if (rpm > max_angle){rpm = max_angle;}
	ledcWrite(spindle_pwm_chan_num, angle2duty(rpm));

    return rpm;
}

void XBoard_ServoSpindle::set_state(uint8_t state, uint32_t rpm) {
	if (sys.abort)
		return;   // Block during abort.

	if (state == SPINDLE_DISABLE) { // Halt or set spindle direction and rpm.
		sys.spindle_speed = 0;
		set_rpm(0);
	}
	else {
		set_rpm(rpm);
	}

	sys.report_ovr_counter = 0; // Set to report change immediately
}