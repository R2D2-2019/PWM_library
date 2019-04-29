/// @file
#pragma once

#include <hwlib.hpp>
#include <cmath>

#include "sam3.h"

///\brief
/// Namespace for PWM library
///\details
/// This namespace contains all the functions for the PWM library.
namespace R2D2::pwm_lib {

///\brief
/// This enum contains the clocks for PWM, CLOCKA or CLOCKB.
    enum class clocks {
        CLOCKA,
        CLOCKB
    };

    ///\brief
    /// This Enum contains the clock frequency's each channel can have. \n
    ///\details
    /// Note: If you select clock A or B each channel that uses one of these clocks has the same frequency. \n
    /// The frequency of clock A and B can be set by using the set_clock function.
    enum class frequencies : uint32_t {
        CLOCKA = PWM_CMR_CPRE_CLKA,
        CLOCKB = PWM_CMR_CPRE_CLKB,
        f_327680hz = PWM_CMR_CPRE_MCK, //327.680hz
        f_163840hz = PWM_CMR_CPRE_MCK_DIV_2, //163.840hz
        f_81920hz = PWM_CMR_CPRE_MCK_DIV_4, //81.920hz
        f_40960hz = PWM_CMR_CPRE_MCK_DIV_8, //40.960hz
        f_20480hz = PWM_CMR_CPRE_MCK_DIV_16, //20.480hz
        f_10240hz = PWM_CMR_CPRE_MCK_DIV_32, //10.240hz
        f_5120hz = PWM_CMR_CPRE_MCK_DIV_64, //5120hz
        f_2560hz = PWM_CMR_CPRE_MCK_DIV_128, //2560hz
        f_1280hz = PWM_CMR_CPRE_MCK_DIV_256, //1280hz
        f_640hz = PWM_CMR_CPRE_MCK_DIV_512, //640hz
        f_320hz = PWM_CMR_CPRE_MCK_DIV_1024 //320hz
    };
    
    enum class polarity {
        POSITIVE,
        NEGATIVE
    };

    ///\brief
    /// hardware PWM control
    ///\details
    /// This is a PWM class to change the duty cycle and/or the frequency of a particular channel.
    /// this class uses the hardware PWM functionality of the sam3x8e chip.
    /// The appropriate constructors and functions are provided.
    class pwm_c {
    private:
        /// The PWM channel number.
        uint_fast8_t ch_nr;
        
        polarity ch_polarity;
        
        bool inverse;

        ///\brief
        /// enables PWM functionality
        ///\details
        /// enables hardware PWM functionality by stetting up PMC and all the IO lines to the correct pins
        void enable_pwm();
		
    public:
        ///\brief
        /// constructor to use a PWM pin.
        ///\details
        /// this constructor initializes the PWM object
        /// the parameters are the PWM channel number see list below for the corresponding pins per channel \n
        /// ch0 PIO: C2 perpheral: B arduino_board_pin: 34 \n
        /// ch1 PIO C4 perpheral: B arduino_board_pin: 36 \n
        /// ch2 PIO C6 perpheral: B arduino_board_pin: 38 \n
        /// ch3 PIO C8 perpheral: B arduino_board_pin: 40 \n
        /// ch4 PIO C21 perpheral: B arduino_board_pin: 9 \n
        /// ch5 PIO C22 perpheral: B arduino_board_pin: 8 \n
        /// ch6 PIO C23 perpheral: B arduino_board_pin: 7 \n
        /// ch7 PIO C24 perpheral: B arduino_board_pin: 6 \n
        pwm_c(uint_fast8_t ch_nr, bool inverse_enable = false, polarity polarity = polarity::POSITIVE);

        ///\brief
        /// Change duty cycle of given PWM channel
        ///\details
        /// Change the duty cycle of a PWM channel
        /// duty cycle should be given with a value between 0 and 255(100%) \n
        /// if a value below 0 is given it will be set to a 0, same for a value above 255 this will be set to 255
        ///\param new_duty_cycle the new duty cycle for the channel.
        void set_duty_cycle(uint8_t new_duty_cycle);

        ///\brief
        /// Select the frequency for a channel based on the frequencies enum.
        ///\details
        ///\param new_frequency the frequency based on the frequencies enum class.
        void select_frequency(frequencies new_frequency);
		
		///\brief
        /// Set frequency of CLOCKA or CLOCKB
        ///\details
        /// Set the frequency given in hz for one of the clocks
		/// Note this changes the CLOCKA and CLOCKB for all pwm channels using 1 of these clocks.
        ///\param frequency the wanted frequency in hz
        ///\param clock the clock you want to set (clocks::CLOCKA or clocks::CLOCKB)
        void set_global_pwm_clock(uint32_t frequency, clocks clock);
        
        void set_inverse(bool inverse_enable_disable);
        
        void set_polarity(polarity new_polarity);
    };
};

