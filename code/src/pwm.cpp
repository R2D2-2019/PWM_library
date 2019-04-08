#include "pwm.hpp"
#include "hwlib.hpp"
#include <cmath>

namespace R2D2::PWM_lib {
	PWM_c::PWM_c(const int & ch_nr):
	ch_nr(ch_nr)
	{ 
		  switch (ch_nr){
			 case 0:
				REG_PWM_CPRD0 = 256; // set period
				REG_PWM_CDTY0 = 128; // set dutycyle (50%)
				REG_PWM_CMR0 = PWM_CMR_CPRE_CLKA; // link clock and enable dualslope 
				REG_PWM_ENA = PWM_ENA_CHID0; // enable ch
				break;
			 case 1:
				REG_PWM_CPRD1 = 256;
				REG_PWM_CDTY1 = 128;
				REG_PWM_CMR1 = PWM_CMR_CPRE_CLKA;
				REG_PWM_ENA = PWM_ENA_CHID1;
				break;
			 case 2:
				REG_PWM_CPRD2 = 256;
				REG_PWM_CDTY2 = 128;
				REG_PWM_CMR2 = PWM_CMR_CPRE_CLKA;
				REG_PWM_ENA = PWM_ENA_CHID2;
				break;
			 case 3:
				REG_PWM_CPRD3 = 256;
				REG_PWM_CDTY3 = 128;
				REG_PWM_CMR3 = PWM_CMR_CPRE_CLKA;
				REG_PWM_ENA = PWM_ENA_CHID3;
				break;
			 case 4:
				REG_PWM_CPRD4 = 256;
				REG_PWM_CDTY4 = 128;
				REG_PWM_CMR4 = PWM_CMR_CPRE_CLKA;
				REG_PWM_ENA = PWM_ENA_CHID4;
				break;
			 case 5:
				REG_PWM_CPRD5 = 256;
				REG_PWM_CDTY5 = 128;
				REG_PWM_CMR5 = PWM_CMR_CPRE_CLKA;
				REG_PWM_ENA = PWM_ENA_CHID5;
				break;
			 case 6:
				REG_PWM_CPRD6 = 256;
				REG_PWM_CDTY6 = 128;
				REG_PWM_CMR6 = PWM_CMR_CPRE_CLKA;
				REG_PWM_ENA = PWM_ENA_CHID6;
				break;	
			 case 7:
				REG_PWM_CPRD7 = 256;
				REG_PWM_CDTY7 = 128;
				REG_PWM_CMR7 = PWM_CMR_CPRE_CLKA;
				REG_PWM_ENA = PWM_ENA_CHID7;
				break;
		  }
		 
	}

	void PWM_c::change_duty_cycle(const uint8_t & new_duty_cycle){
	   switch (ch_nr){
			 case 0:
				REG_PWM_CDTYUPD0 = new_duty_cycle;
				break;
			 case 1:
				REG_PWM_CDTYUPD1 = new_duty_cycle;
				break;
			 case 2:
				REG_PWM_CDTYUPD2 = new_duty_cycle;
				break;
			 case 3:
				REG_PWM_CDTYUPD3 = new_duty_cycle;
				break;
			 case 4:
				REG_PWM_CDTYUPD4 = new_duty_cycle;
				break;
			 case 5:
				REG_PWM_CDTYUPD5 = new_duty_cycle;
				break;
			 case 6:
				REG_PWM_CDTYUPD6 = new_duty_cycle;
				break;
			 case 7:
				REG_PWM_CDTYUPD7 = new_duty_cycle;
				break;
		  }
	}
	
	void PWM_c::select_frequency(frequencys new_frequency){
		switch (ch_nr){
			 case 0:
				REG_PWM_CMR0 = static_cast<volatile uint32_t>(new_frequency);
				break;
			 case 1:
				REG_PWM_CMR1 = static_cast<volatile uint32_t>(new_frequency);
				break;
			 case 2:
				REG_PWM_CMR2 = static_cast<volatile uint32_t>(new_frequency);
				break;
			 case 3:
				REG_PWM_CMR3 = static_cast<volatile uint32_t>(new_frequency);
				break;
			 case 4:
				REG_PWM_CMR4 = static_cast<volatile uint32_t>(new_frequency);
				break;
			 case 5:
				REG_PWM_CMR5 = static_cast<volatile uint32_t>(new_frequency);
				break;
			 case 6:
				REG_PWM_CMR6 = static_cast<volatile uint32_t>(new_frequency);
				break;
			 case 7:
				REG_PWM_CMR7 = static_cast<volatile uint32_t>(new_frequency);
				break;
		  }
	}
	
	
	void enable_pwm(){
	   REG_PMC_PCER1 |= PMC_PCER1_PID36; //enable PWM
	   REG_PWM_CLK = PWM_CLK_PREA(2) | PWM_CLK_DIVA(206); // set clock
	   REG_PWM_SCM = (0x01<<16); // enable auto update
	   
	   //set ch's to proper output pins
	   // ch0 PIO: C2 perpheral: B arduino_board_pin: 34
	   REG_PIOC_ABSR |= PIO_ABSR_P2;                     
	   REG_PIOC_PDR |= PIO_PDR_P2; 
	   //ch1 PIO C4 perpheral: B arduino_board_pin: 36
	   REG_PIOC_ABSR |= PIO_ABSR_P4;                     
	   REG_PIOC_PDR |= PIO_PDR_P4; 
	   //ch2 PIO C6 perpheral: B arduino_board_pin: 38
	   REG_PIOC_ABSR |= PIO_ABSR_P6;                     
	   REG_PIOC_PDR |= PIO_PDR_P6; 
	   //ch3 PIO C8 perpheral: B arduino_board_pin: 40
	   REG_PIOC_ABSR |= PIO_ABSR_P8;                     
	   REG_PIOC_PDR |= PIO_PDR_P8;
	   //ch4 PIO C21 perpheral: B arduino_board_pin: 9
	   REG_PIOC_ABSR |= PIO_ABSR_P21;                     
	   REG_PIOC_PDR |= PIO_PDR_P21;  
	   //ch5 PIO C22 perpheral: B arduino_board_pin: 8
	   REG_PIOC_ABSR |= PIO_ABSR_P22;                     
	   REG_PIOC_PDR |= PIO_PDR_P22;
	   //ch6 PIO C23 perpheral: B arduino_board_pin: 7
	   REG_PIOC_ABSR |= PIO_ABSR_P23;                     
	   REG_PIOC_PDR |= PIO_PDR_P23;  
	   //ch7 PIO C24 perpheral: B arduino_board_pin: 6
	   REG_PIOC_ABSR |= PIO_ABSR_P24;                     
	   REG_PIOC_PDR |= PIO_PDR_P24; 
	}
	
	void set_clock(int frequency, clocks clock){
		int divider =1024 ;
		uint8_t clk_div = 0;
		while( clk_div < 11){	
			divider = static_cast<int>(((CHIP_FREQ_CPU_MAX / (frequency*0xFF)) / std::pow(2, clk_div)));
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