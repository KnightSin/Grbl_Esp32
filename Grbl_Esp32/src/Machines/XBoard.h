/*
    xboard_e01.h
    Part of Grbl_ESP32
    Pin assignments for the XBoard E0.1.
    2018    - Bart Dring
    2020    - Mitch Bradley
    Grbl_ESP32 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Grbl is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Grbl_ESP32.  If not, see <http://www.gnu.org/licenses/>.
*/

// Board type
// #define XBOARD_E0_2
// #define XBOARD_E0_3
// #define XBOARD_E0_4
// #define XBOARD_E0_5
#define XBOARD_E1_0

// Machine name
#ifdef XBOARD_E0_2
#    define MACHINE_NAME "XBoard E0.2"
#endif
#ifdef XBOARD_E0_3
#    define MACHINE_NAME "XBoard E0.3"
#endif
#ifdef XBOARD_E0_4
#    define MACHINE_NAME "XBoard E0.4"
#endif
#ifdef XBOARD_E0_5
#    define MACHINE_NAME "XBoard E0.5"
#endif
#ifdef XBOARD_E1_0
#    define MACHINE_NAME "XBoard E1.0"
#endif

// Board features
#define CoreXY
#ifndef CoreXY
#    define CUSTOM_CODE_FILENAME "../Custom/XBoard.cpp"
#else
#    define CUSTOM_CODE_FILENAME "../Custom/XBoard_CoreXY.cpp"
#    define USE_KINEMATICS      // there are kinematic equations for this machine
#    define USE_FWD_KINEMATICS  // report in cartesian
#    define USE_MACHINE_INIT    // There is some custom initialization for this machine
#    define USE_CUSTOM_HOMING
#endif

// Homing cycle
#ifdef HOMING_CYCLE_0
#    undef HOMING_CYCLE_0
#endif
#define HOMING_CYCLE_0 bit(X_AXIS) | bit(Y_AXIS)
#ifdef HOMING_CYCLE_1
#    undef HOMING_CYCLE_1
#endif
#ifdef HOMING_CYCLE_2
#    undef HOMING_CYCLE_2
#endif

#if (defined XBOARD_E0_2) || (defined XBOARD_E0_3) || (defined XBOARD_E0_4) || (defined XBOARD_E0_5)

#    define X_STEP_PIN GPIO_NUM_14
#    define X_DIRECTION_PIN GPIO_NUM_27
#    define Y_STEP_PIN GPIO_NUM_26
#    define Y_DIRECTION_PIN GPIO_NUM_25
#    define Z_STEP_PIN GPIO_NUM_33
#    define Z_DIRECTION_PIN GPIO_NUM_32

#    define X_LIMIT_PIN GPIO_NUM_16
#    define Y_LIMIT_PIN GPIO_NUM_4
#    define Z_LIMIT_PIN GPIO_NUM_15

#    define STEPPERS_DISABLE_PIN GPIO_NUM_12

#    define SPINDLE_OUTPUT_PIN GPIO_NUM_17

#    define USE_MACHINE_INIT

#    define MACRO_BUTTON_0_PIN GPIO_NUM_0  // button0
#    define INVERT_CONTROL_PIN_MASK B00010000

#    define SDCARD_DET_PIN GPIO_NUM_21

#    define STAT_LED_PIN GPIO_NUM_13

#    define SPINDLE_TYPE SpindleType::Servo

#endif

#if (defined XBOARD_E1_0)

#    define X_STEP_PIN GPIO_NUM_14
#    define X_DIRECTION_PIN GPIO_NUM_27
#    define Y_STEP_PIN GPIO_NUM_26
#    define Y_DIRECTION_PIN GPIO_NUM_25
#    define Z_STEP_PIN GPIO_NUM_33
#    define Z_DIRECTION_PIN GPIO_NUM_32

// #    define TMC_UART UART_NUM_2
// #    define TMC_UART_TX GPIO_NUM_22
// #    define TMC_UART_RX GPIO_NUM_4

// #    define X_TRINAMIC_DRIVER 2208
// #    define X_RSENSE TMC2208_RSENSE_DEFAULT
// #    define X_DRIVER_ADDRESS 0  // TMC 2208 does not use address, this field is 0
// #    define DEFAULT_X_MICROSTEPS 16


// #define TRINAMIC_DAISY_CHAIN
// #define TRINAMIC_RUN_MODE           TrinamicMode :: CoolStep
// #define TRINAMIC_HOMING_MODE        TrinamicMode :: CoolStep

// #define X_TRINAMIC_DRIVER       5160        // Which Driver Type?
// #define X_CS_PIN                GPIO_NUM_22  //chip select
// #define X_RSENSE                TMC5160_RSENSE_DEFAULT

// #define Y_TRINAMIC_DRIVER       5160        // Which Driver Type?
// #define Y_CS_PIN                GPIO_NUM_21  //chip select
// #define Y_RSENSE                TMC5160_RSENSE_DEFAULT

// #define Z_TRINAMIC_DRIVER       5160        // Which Driver Type?
// #define Z_CS_PIN                GPIO_NUM_2  //chip select
// #define Z_RSENSE                TMC5160_RSENSE_DEFAULT

#    define X_LIMIT_PIN GPIO_NUM_36
#    define Y_LIMIT_PIN GPIO_NUM_39
#    define Z_LIMIT_PIN GPIO_NUM_34

#    define STEPPERS_DISABLE_PIN GPIO_NUM_12

#    define SPINDLE_OUTPUT_PIN GPIO_NUM_17

#    define LASER_OUTPUT_PIN SPINDLE_OUTPUT_PIN

#    define USE_MACHINE_INIT

#    define MACRO_BUTTON_0_PIN GPIO_NUM_0

#    define INVERT_CONTROL_PIN_MASK B00010000

#    define SDCARD_DET_PIN GPIO_NUM_35

#    define STAT_LED_PIN GPIO_NUM_13

#    define SPINDLE_TYPE SpindleType::Servo

#endif