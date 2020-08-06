/*
    XBoard_ElectromagnetSpindle.cpp

	使用M3、M5指令控制电磁铁，M5导通，M3断开。

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

// ========================= XBoard_ElectromagnetSpindle ==================================
/*
    This is a sub class of PWMSpindle but is a digital rather than PWM output
*/
#define PWM_ON		0
#define PWM_HOLD	1
#define PWM_OFF		2

static uint8_t output_pin;
static uint8_t spindle_pwm_chan_num;
static uint32_t pwm_freq;
static uint8_t pwm_precision;
static uint32_t pwm_hold_val;
static uint32_t pwm_on_val;
static uint32_t pwm_off_val;
static uint32_t pwm_hold_taskFlag;
static TaskHandle_t Electromagnet_Hold_TaskHandle = 0;

// The communications task
void Electromagnet_Hold_Task(void* pvParameters) 
{
	while (true)
	{
		if (pwm_hold_taskFlag == PWM_ON)
		{
			//Serial.println("PWM_ON");
			vTaskDelay(1000);
			if (pwm_hold_taskFlag == PWM_ON)
			{
				ledcWrite(spindle_pwm_chan_num, pwm_hold_val);
				pwm_hold_taskFlag = PWM_HOLD;
				//Serial.println("PWM_HOLD");
			}
		}
		vTaskDelay(10);
		//Serial.print(".");
	}
}

void XBoard_ElectromagnetSpindle::Electromagnet_Hold_Task_Init()
{
	xTaskCreatePinnedToCore(Electromagnet_Hold_Task,	// task
		"Electromagnet_Hold_TaskHandle",				// name for task
		4096,   // size of task stack
		NULL,   // parameters
		1,		// priority
		&Electromagnet_Hold_TaskHandle,
		0		// core
	);
}

void XBoard_ElectromagnetSpindle::init() {
#ifndef SPINDLE_OUTPUT_PIN
	return;
#else
	output_pin = SPINDLE_OUTPUT_PIN;
	spindle_pwm_chan_num = 0; // Channel 0 is reserved for spindle use
	pwm_freq = 1000.0;
	pwm_precision = 10;
	pwm_hold_val = (uint32_t)xboard_em_pwm_hold_val->get();
	pwm_on_val = 1024;
	pwm_off_val = 0;
	pwm_hold_taskFlag = PWM_ON;
#endif

	ledcSetup(spindle_pwm_chan_num, (double)pwm_freq, pwm_precision); // setup the channel
	ledcAttachPin(output_pin, spindle_pwm_chan_num); // attach the PWM to the pin

    config_message();
	Electromagnet_Hold_Task_Init();

	set_state(SPINDLE_DISABLE, 0);
}

// prints the startup message of the spindle config
void XBoard_ElectromagnetSpindle :: config_message() {    
    grbl_msg_sendf(CLIENT_SERIAL,
                   MSG_LEVEL_INFO,
				   "XBoard Electromagnet Spindle Output:%s",
				   pinName(output_pin).c_str());
}

uint32_t XBoard_ElectromagnetSpindle::set_rpm(uint32_t rpm) {
    if (output_pin == UNDEFINED_PIN)
        return rpm;

    sys.spindle_speed = rpm;

    if (rpm == 0) {
		ledcWrite(spindle_pwm_chan_num, pwm_on_val);
		pwm_hold_taskFlag = PWM_ON;
		//Serial.println("rpm == 0");
    } else {
		ledcWrite(spindle_pwm_chan_num,pwm_off_val);
		pwm_hold_taskFlag = PWM_OFF;
		//Serial.println("rpm != 0");
    }

    return rpm;
}

void XBoard_ElectromagnetSpindle::set_state(uint8_t state, uint32_t rpm) {
	if (sys.abort)
		return;   // Block during abort.

	if (state == SPINDLE_DISABLE) { // Halt or set spindle direction and rpm.
		sys.spindle_speed = 0;
		set_rpm(0);
	}
	else {
		set_rpm(1);
	}

	sys.report_ovr_counter = 0; // Set to report change immediately
}