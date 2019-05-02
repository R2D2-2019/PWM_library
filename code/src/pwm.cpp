#include <pwm.hpp>
#include <hwlib.hpp>
#include <cmath>

namespace R2D2::pwm_lib {
    struct pwm_channel_s {
        uint32_t absr;
        uint32_t pdr;
    };

    constexpr pwm_channel_s pwm_channels[] = {
        // ch0 PIO: C2 peripheral: B arduino_board_pin: 34
        pwm_channel_s{PIO_ABSR_P2, PIO_PDR_P2},

        // ch1 PIO C4 peripheral: B arduino_board_pin: 36
        pwm_channel_s{PIO_ABSR_P4, PIO_PDR_P4},
 
        // ch2 PIO C6 peripheral: B arduino_board_pin: 38
        pwm_channel_s{PIO_ABSR_P6, PIO_PDR_P6},

        // ch3 PIO C8 peripheral: B arduino_board_pin: 40
        pwm_channel_s{PIO_ABSR_P8, PIO_PDR_P8},

        // ch4 PIO C21 peripheral: B arduino_board_pin: 9
        pwm_channel_s{PIO_ABSR_P21, PIO_PDR_P21},

        // ch5 PIO C22 peripheral: B arduino_board_pin: 8
        pwm_channel_s{PIO_ABSR_P22, PIO_PDR_P22},

        // ch6 PIO C23 peripheral: B arduino_board_pin: 7
        pwm_channel_s{PIO_ABSR_P23, PIO_PDR_P23},

        // ch7 PIO C24 peripheral: B arduino_board_pin: 6
        pwm_channel_s{PIO_ABSR_P24, PIO_PDR_P24}
    };
    
    constexpr pwm_channel_s pwm_inverse_channels[] = {
        // ch0 PIO: C3 peripheral: B arduino_board_pin: 35
         pwm_channel_s{PIO_ABSR_P3, PIO_PDR_P3},
         
        // ch1 PIO C5 peripheral: B arduino_board_pin: 37
        pwm_channel_s{PIO_ABSR_P5, PIO_PDR_P5},
        
        // ch2 PIO C7 peripheral: B arduino_board_pin: 39
        pwm_channel_s{PIO_ABSR_P7, PIO_PDR_P7},
    
        // ch3 PIO C9 peripheral: B arduino_board_pin: 41
        pwm_channel_s{PIO_ABSR_P9, PIO_PDR_P9},
        
        // ch4 on arduino board pin routed 
        pwm_channel_s{0, 0},
        
        // ch5 PIO C19 peripheral: B arduino_board_pin: 44
        pwm_channel_s{PIO_ABSR_P19, PIO_PDR_P19},
        
        // ch6 PIO C18 peripheral: B arduino_board_pin: 45
        pwm_channel_s{PIO_ABSR_P18, PIO_PDR_P18},
        
         // ch7 on arduino board pin routed 
        pwm_channel_s{ 0, 0}
        
    };  

    void pwm_c::enable_pwm() {
        static bool initialized = false;

        if (initialized) {
            return;
        }

        // Enable PWM
        REG_PMC_PCER1 |= PMC_PCER1_PID36;

        // Enable auto update
        REG_PWM_SCM = (0x01 << 16);

        // Set CLOCKA to 50hz
        set_global_pwm_clock(50, clocks::CLOCKA);

        // Set CLOCKB to 200hz
        set_global_pwm_clock(200, clocks::CLOCKB);

        initialized = true;
    }

    void pwm_c::set_global_pwm_clock(const uint32_t frequency,const clocks clock) {
        uint16_t divider = 0;
        uint8_t clk_div = 0;

        while (clk_div < 11) {
            // Calculation stolen from llib
            divider = static_cast<uint16_t>(
                ((CHIP_FREQ_CPU_MAX / (frequency * 0xFF)) / std::pow(2, clk_div))
            );

            if (divider <= 0xFF) {
                break;
            }

            ++clk_div;
        }
		
        if (clock == clocks::CLOCKA) {
            REG_PWM_CLK = PWM_CLK_PREA(clk_div) | PWM_CLK_DIVA(divider) | (REG_PWM_CLK & 0xFFFF0000);  ;
        } else if (clock == clocks::CLOCKB) {
            REG_PWM_CLK = PWM_CLK_PREB(clk_div) | PWM_CLK_DIVB(divider) | (REG_PWM_CLK & 0xFFFF) ;
        }
    }

    pwm_c::pwm_c(uint_fast8_t ch_nr, bool inverse_enable_disable, polarity polarity )
        : ch_nr(ch_nr),
          ch_polarity(polarity),
          inverse(inverse_enable_disable){
        if (ch_nr > 7) {
            return;
        }
        set_inverse(inverse_enable_disable);
        set_polarity(ch_polarity);

        enable_pwm();
        // set the proper output pins
        REG_PIOC_ABSR |= pwm_channels[ch_nr].absr;
        REG_PIOC_PDR |= pwm_channels[ch_nr].pdr;
        //set period and duty cycle
        PWM->PWM_CH_NUM[ch_nr].PWM_CPRD = 256;
        PWM->PWM_CH_NUM[ch_nr].PWM_CDTY = 128;
        // select clock A
        PWM->PWM_CH_NUM[ch_nr].PWM_CMR = PWM_CMR_CPRE_CLKA;
        // enable channel
        uint8_t bin_ch_nr = (1 << ch_nr);
        REG_PWM_ENA = bin_ch_nr;
		
        while(!((REG_PWM_SR & bin_ch_nr) == bin_ch_nr)){
        }

    }

    void pwm_c::set_duty_cycle(uint8_t new_duty_cycle) {
        PWM->PWM_CH_NUM[ch_nr].PWM_CDTYUPD = new_duty_cycle;
    }

    void pwm_c::select_frequency(frequencies new_frequency) {
        PWM->PWM_CH_NUM[ch_nr].PWM_CMR = static_cast<uint32_t>(new_frequency);
    }
    
    void pwm_c::set_inverse(bool inverse_enable_disable){
        inverse = inverse_enable_disable;
        if (inverse) {
            REG_PIOC_ABSR |= pwm_inverse_channels[ch_nr].absr;
            REG_PIOC_PDR |= pwm_inverse_channels[ch_nr].pdr;
        } else {
            REG_PIOC_ABSR &= ~(pwm_inverse_channels[ch_nr].absr);
            REG_PIOC_PDR  &= ~(pwm_inverse_channels[ch_nr].absr);
        }
    }
    
    void pwm_c::set_polarity(polarity new_polartiy) {
	if (new_polarity != ch_polarity){
            ch_polarity = new_polartiy;
            if (ch_polarity == polarity::NEGATIVE){
                PWM->PWM_CH_NUM[ch_nr].PWM_CMR |= PWM_CMR_CPOL;
            } else {
                PWM->PWM_CH_NUM[ch_nr].PWM_CMR &= ~PWM_CMR_CPOL;
            }
	}
    }
}
    
