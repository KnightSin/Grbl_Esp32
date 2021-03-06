#pragma once

/*
	RelaySpindle.h

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
#include "PWMSpindle.h"

namespace Spindles {
    // This is for an on/off spindle all RPMs above 0 are on
    class EM : public PWM {
    public:
        EM() = default;

        EM(const EM&) = delete;
        EM(EM&&)      = delete;
        EM& operator=(const EM&) = delete;
        EM& operator=(EM&&) = delete;

        void        init() override;
        void        config_message() override;
        uint32_t    set_rpm(uint32_t rpm) override;
        void        set_state(SpindleState state, uint32_t rpm) override;
        void        stop() override;
        virtual ~EM() {}
    private:
        bool _pwm_invert;
        uint32_t _pwm_hold_taskFlag;
        uint32_t _pwm_hold_val;
        uint32_t _pwm_on_val;
        uint32_t _pwm_off_val;

        uint32_t angle2duty(float angle);
        void EM_Hold_Task_Init();
    };
}
