#include <pwm.hpp>
#include <hwlib.hpp>
#include <cmath>

namespace R2D2::PWM_lib {
	PWM_c::PWM_c(uint_fast8_t ch_nr):
	ch_nr(ch_nr)
	{ 
		if (ch_nr >7){
			return;
		}
		PWM->PWM_CH_NUM[ch_nr].PWM_CPRD = 256;
		PWM->PWM_CH_NUM[ch_nr].PWM_CDTY = 128;
		PWM->PWM_CH_NUM[ch_nr].PWM_CMR = PWM_CMR_CPRE_CLKA;
		REG_PWM_ENA = (1<<ch_nr); 
		
		switch (ch_nr){
			case 0:
			// ch0 PIO: C2 perpheral: B arduino_board_pin: 34
				REG_PIOC_ABSR |= PIO_ABSR_P2;                     
				REG_PIOC_PDR |= PIO_PDR_P2;
				break;
			case 1:
			//ch1 PIO C4 perpheral: B arduino_board_pin: 36
				REG_PIOC_ABSR |= PIO_ABSR_P4;                     
				REG_PIOC_PDR |= PIO_PDR_P4;
				break;
			case 2:
			//ch2 PIO C6 perpheral: B arduino_board_pin: 38
				REG_PIOC_ABSR |= PIO_ABSR_P6;                     
				REG_PIOC_PDR |= PIO_PDR_P6; 
				break;
			case 3:
			//ch3 PIO C8 perpheral: B arduino_board_pin: 40
				REG_PIOC_ABSR |= PIO_ABSR_P8;                     
				REG_PIOC_PDR |= PIO_PDR_P8;
				break;
			case 4:
			//ch4 PIO C21 perpheral: B arduino_board_pin: 9
				REG_PIOC_ABSR |= PIO_ABSR_P21;                     
				REG_PIOC_PDR |= PIO_PDR_P21;  
				break;
			case 5:
			//ch5 PIO C22 perpheral: B arduino_board_pin: 8
				REG_PIOC_ABSR |= PIO_ABSR_P23;                     
				REG_PIOC_PDR |= PIO_PDR_P23;
				break;				
			case 6:
			//ch6 PIO C23 perpheral: B arduino_board_pin: 7
				REG_PIOC_ABSR |= PIO_ABSR_P23;                     
				REG_PIOC_PDR |= PIO_PDR_P23; 
				break;				
			case 7:
			//ch7 PIO C24 perpheral: B arduino_board_pin: 6
				REG_PIOC_ABSR |= PIO_ABSR_P24;                     
				REG_PIOC_PDR |= PIO_PDR_P24; 
				break;
		}
	}

	void PWM_c::change_duty_cycle(const uint8_t & new_duty_cycle){
	  PWM->PWM_CH_NUM[ch_nr].PWM_CDTYUPD = new_duty_cycle;
	}
	
	void PWM_c::select_frequency(frequencies new_frequency){
		PWM->PWM_CH_NUM[ch_nr].PWM_CMR = static_cast<volatile uint32_t>(new_frequency);
	}
	
	
	void enable_pwm(){
	   REG_PMC_PCER1 |= PMC_PCER1_PID36; //enable PWM
	   REG_PWM_SCM = (0x01<<16); // enable auto update
	   set_clock(50, clocks::CLOCKA); //set CLOCKA to 50hz
	   set_clock(200, clocks::CLOCKB); //set CLOCKB to 200hz	
	}	
	
	void set_clock(int frequency, clocks clock){
		uint16_t divider = 0;
		uint8_t clk_div = 0;
		while( clk_div < 11){	
			// calculation stolen from llib
			divider = static_cast<uint16_t>(((CHIP_FREQ_CPU_MAX / (frequency*0xFF)) / std::pow(2, clk_div)));
			if(divider <= 0xFF){			
				break;
			}  
			++clk_div;
        }
		
		if(clock == clocks::CLOCKA){
			REG_PWM_CLK = PWM_CLK_PREA(clk_div) | PWM_CLK_DIVA(divider);	
		}
		else if(clock == clocks::CLOCKB){
			REG_PWM_CLK = PWM_CLK_PREB(clk_div) | PWM_CLK_DIVB(divider);
		}
	}		
}