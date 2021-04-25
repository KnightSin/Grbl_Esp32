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
#include "ServoSpindle.h"

// ========================= Servo ==================================

namespace Spindles {
    /*
    This is a sub class of PWM but is a digital rather than PWM output
*/
    void Servo::init() {
        get_pins_and_settings();

        if (_output_pin == UNDEFINED_PIN) {
            grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Warning: Spindle output pin not defined");
            return;
        }

        _pwm_freq      = 50;
        _pwm_precision = 10;
        max_angle      = servo_max_angle->get();
        dir_invert     = servo_invert->get();

        ledcSetup(_pwm_chan_num, (double)_pwm_freq, _pwm_precision);  // setup the channel
        ledcAttachPin(_output_pin, _pwm_chan_num);                    // attach the PWM to the pin

        set_state(SpindleState::Disable, 0);

        use_delays = true;

        config_message();
    }

    // prints the startup message of the spindle config
    void Servo ::config_message() { grbl_msg_sendf(CLIENT_ALL, MsgLevel::Info, "Servo spindle Output:%s", pinName(_output_pin).c_str()); }

    // 0.5ms-----0°:  2.5%
    // 1.0ms----45°:  5.0%
    // 1.5ms----90°:  7.5%
    // 2.0ms---135°: 10.0%
    // 2.5ms---180°: 12.5%
    // duty = {[0.5ms + (Angle/45.0°)*0.5ms] / 20.0ms} * pow(2, pwm_precision)

    uint32_t Servo::angle2duty(float angle) {
        if (dir_invert == true)
            return (uint32_t)(((0.5 + ((max_angle - angle) / 45.0) * 0.5) / 20.0) * pow(2, _pwm_precision));
        else
            return (uint32_t)(((0.5 + (angle / 45.0) * 0.5) / 20.0) * pow(2, _pwm_precision));
    }

    uint32_t Servo::set_rpm(uint32_t rpm) {
        static uint32_t recoder_rpm = 0;

        if (_output_pin == UNDEFINED_PIN)
            return rpm;

        if (recoder_rpm == rpm)
            return rpm;  // 不增加此部分会报错：Guru Meditation Error: Core 1 panic'ed (Coprocessor exception)

        recoder_rpm = rpm;

        sys.spindle_speed = rpm;

        if (rpm < 1) {
            rpm = 1;
        } else if (rpm > max_angle) {
            rpm = max_angle;
        }
        ledcWrite(_pwm_chan_num, angle2duty(rpm));

        return rpm;
    }

    void Servo::set_state(SpindleState state, uint32_t rpm) {
        if (sys.abort)
            return;  // Block during abort.

        if (state == SpindleState::Disable) {  // Halt or set spindle direction and rpm.
            sys.spindle_speed = 0;
            set_rpm(0);
        } else {
            set_rpm(rpm);
        }

        sys.report_ovr_counter = 0;  // Set to report change immediately
    }
}