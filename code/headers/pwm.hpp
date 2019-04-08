/// @file
#pragma once
#include <hwlib.hpp>
///\brief
/// namespace for PWM library
///\details
/// this namespace contains all the functions for the PWM class
namespace R2D2::PWM_lib {
	
enum class clocks {
	CLOCKA,
	CLOCKB
};

enum class frequencys {
	CLOCKA = PWM_CMR_CPRE_CLKA,
	CLOCKB = PWM_CMR_CPRE_CLKB,
	f_327680hz = PWM_CMR_CPRE_MCK, //327.680hz
	f_163840hz = PWM_CMR_CPRE_MCK_DIV_2,//163.840hz
	f_81920hz = PWM_CMR_CPRE_MCK_DIV_4,//81.920hz
	f_40960hz = PWM_CMR_CPRE_MCK_DIV_8,//40.960hz
	f_20480hz = PWM_CMR_CPRE_MCK_DIV_16,//20.480hz
	f_10240hz = PWM_CMR_CPRE_MCK_DIV_32 ,//10.240hz
	f_5120hz = PWM_CMR_CPRE_MCK_DIV_64,//5120hz
	f_2560hz = PWM_CMR_CPRE_MCK_DIV_128,//2560hz
	f_1280hz = PWM_CMR_CPRE_MCK_DIV_256,//1280hz
	f_640hz = PWM_CMR_CPRE_MCK_DIV_512,//640hz
	f_320hz = PWM_CMR_CPRE_MCK_DIV_1024//320hz
};

///\brief
/// hardware PWM control to change dutycycle 
///\details
/// This is a PWM class to change the duty cycle of a particular channel 
/// this class uses the hardware pwm functionality of the sam3x8e chip
/// The appropriate constructors and functions are provided
   class PWM_c
   {
	  private:
		 int ch_nr;
	  public:
		  ///\brief 
		 /// constructor 
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
		  PWM_c(const int & ch_nr);
		  
		  ///\brief
		  /// change duty cycle of given pwm channel
		  ///\details
		  /// change the duty cycle of a pwm channel 
		  /// duty cycle should be given with a value between 0 and 100 \n
		  /// if a value below 0 is given it will be set to a 0, same for a value above 100 this will be set to 100
		  void change_duty_cycle(const uint8_t & new_duty_cycle);
		  
		  void select_frequency(frequencys new_frequency);
	  };

   ///\brief
   /// enables PWM functionality
   ///\details
   /// enables hardware PWM functionality by stetting up PMC and all the IO lines to the correct pins
	void enable_pwm();
	
	void set_clock(int frequency,clocks clock);
};

