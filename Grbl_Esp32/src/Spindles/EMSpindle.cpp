/*
    RelaySpindle.cpp

    This is used for a basic on/off spindle All S Values above 0
    will turn the spindle on.

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
#include "EMSpindle.h"

// ========================= EM ==================================

namespace Spindles {
/*
    This is a sub class of PWM but is a digital rather than PWM output
*/
#define PWM_ON 0
#define PWM_HOLD 1
#define PWM_OFF 2
    static TaskHandle_t EM_Hold_TaskHandle = 0;
    static uint32_t     pwm_hold_taskFlag;
    static uint32_t     pwm_hold_val;
    static uint32_t     pwm_chan_num;

    void EM_Hold_Task(void* pvParameters) {
        while (true) {
            if (spindle_type->get() == (int8_t)SpindleType::EM) {
                if (pwm_hold_taskFlag == PWM_ON) {
                    vTaskDelay(100);
                    if (pwm_hold_taskFlag == PWM_ON) {
                        ledcWrite(pwm_chan_num, pwm_hold_val);
                        pwm_hold_taskFlag = PWM_HOLD;
                    }
                }
            }
            vTaskDelay(10);
        }
    }

    void EM::EM_Hold_Task_Init() {
        xTaskCreatePinnedToCore(EM_Hold_Task,    // task
                                "EM_Hold_Task",  // name for task
                                4096,            // size of task stack
                                NULL,            // parameters
                                1,               // priority
                                &EM_Hold_TaskHandle,
                                SUPPORT_TASK_CORE  // core
        );
    }

    void EM::init() {
        get_pins_and_settings();

        if (_output_pin == UNDEFINED_PIN) {
            grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Warning: Spindle output pin not defined");
            return;
        }

        _pwm_freq      = 1000.0;
        _pwm_precision = 10;
        _pwm_invert    = em_invert->get();
        _pwm_hold_val  = em_hold_value->get();

        if (_pwm_invert == false) {
            _pwm_hold_val = (uint32_t)em_hold_value->get();
            _pwm_on_val   = pow(2, _pwm_precision);
            _pwm_off_val  = 0;
        } else {
            _pwm_hold_val = pow(2, _pwm_precision) - (uint32_t)em_hold_value->get();
            _pwm_on_val   = 0;
            _pwm_off_val  = pow(2, _pwm_precision);
        }
        _pwm_hold_taskFlag = PWM_ON;

        pwm_hold_taskFlag  = _pwm_hold_taskFlag;
        pwm_hold_val       = _pwm_hold_val;
        pwm_chan_num       = _pwm_chan_num;

        ledcSetup(_pwm_chan_num, (double)_pwm_freq, _pwm_precision);  // setup the channel
        ledcAttachPin(_output_pin, _pwm_chan_num);                    // attach the PWM to the pin

        set_state(SpindleState::Disable, 0);

        use_delays = true;
        EM_Hold_Task_Init();

        config_message();
    }

    // prints the startup message of the spindle config
    void EM ::config_message() {
        grbl_msg_sendf(CLIENT_ALL,
                       MsgLevel::Info,
                       "PWM spindle Output:%s, Invert:%s, Hold:%d",
                       pinName(_output_pin).c_str(),
                       _pwm_invert ? "On" : "Off",
                       (uint32_t)em_hold_value->get());
    }

    uint32_t EM::set_rpm(uint32_t rpm) {
        if (_output_pin == UNDEFINED_PIN)
            return rpm;

        sys.spindle_speed = rpm;

        if (rpm == 0) {
            ledcWrite(_pwm_chan_num, _pwm_on_val);
            _pwm_hold_taskFlag = PWM_ON;

            pwm_hold_taskFlag  = _pwm_hold_taskFlag;
        } else {
            ledcWrite(_pwm_chan_num, _pwm_off_val);
            _pwm_hold_taskFlag = PWM_OFF;
            
            pwm_hold_taskFlag  = _pwm_hold_taskFlag;
        }

        return rpm;
    }

    void EM::set_state(SpindleState state, uint32_t rpm) {
        if (sys.abort)
            return;  // Block during abort.

        if (state == SpindleState::Disable) {  // Halt or set spindle direction and rpm.
            sys.spindle_speed = 0;
            set_rpm(0);
        } else {
            set_rpm(1);
        }

        sys.report_ovr_counter = 0;  // Set to report change immediately
    }

    void EM::stop()
    {
        vTaskDelete(EM_Hold_TaskHandle);
    }
}