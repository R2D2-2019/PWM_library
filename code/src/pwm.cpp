#include <pwm.hpp>
#include <hwlib.hpp>
#include <cmath>

namespace R2D2::pwm_lib {
    struct pwm_channel_s {
        uint32_t absr;
        uint32_t pdr;
    };

    constexpr pwm_channel_s pwm_channels[] = {
        // ch0 PIO: C2 perpheral: B arduino_board_pin: 34
        pwm_channel_s{PIO_ABSR_P2, PIO_PDR_P2},

        // ch1 PIO C4 perpheral: B arduino_board_pin: 36
        pwm_channel_s{PIO_ABSR_P4, PIO_PDR_P4},

        // ch2 PIO C6 perpheral: B arduino_board_pin: 38
        pwm_channel_s{PIO_ABSR_P6, PIO_PDR_P6},

        // ch3 PIO C8 perpheral: B arduino_board_pin: 40
        pwm_channel_s{PIO_ABSR_P8, PIO_PDR_P8},

        // ch4 PIO C21 perpheral: B arduino_board_pin: 9
        pwm_channel_s{PIO_ABSR_P21, PIO_PDR_P21},

        // ch5 PIO C22 perpheral: B arduino_board_pin: 8
        pwm_channel_s{PIO_ABSR_P22, PIO_PDR_P22},

        // ch6 PIO C23 perpheral: B arduino_board_pin: 7
        pwm_channel_s{PIO_ABSR_P23, PIO_PDR_P23},

        // ch7 PIO C24 perpheral: B arduino_board_pin: 6
        pwm_channel_s{PIO_ABSR_P24, PIO_PDR_P24}
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

    void pwm_c::set_global_pwm_clock(const uint32_t frequency, const clocks clock) {
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
            REG_PWM_CLK = PWM_CLK_PREA(clk_div) | PWM_CLK_DIVA(divider);
        } else if (clock == clocks::CLOCKB) {
            REG_PWM_CLK = PWM_CLK_PREB(clk_div) | PWM_CLK_DIVB(divider);
        }
    }

    pwm_c::pwm_c(uint_fast8_t ch_nr)
        : ch_nr(ch_nr) {
        if (ch_nr > 7) {
            return;
        }

        enable_pwm();

        PWM->PWM_CH_NUM[ch_nr].PWM_CPRD = 256;
        PWM->PWM_CH_NUM[ch_nr].PWM_CDTY = 128;
        PWM->PWM_CH_NUM[ch_nr].PWM_CMR = PWM_CMR_CPRE_CLKA;

        REG_PWM_ENA = (1U << ch_nr);

        REG_PIOC_ABSR |= pwm_channels[ch_nr].absr;
        REG_PIOC_PDR |= pwm_channels[ch_nr].pdr;
    }

    void pwm_c::change_duty_cycle(uint8_t new_duty_cycle) {
        PWM->PWM_CH_NUM[ch_nr].PWM_CDTYUPD = new_duty_cycle;
    }

    void pwm_c::select_frequency(frequencies new_frequency) {
        PWM->PWM_CH_NUM[ch_nr].PWM_CMR = static_cast<uint32_t>(new_frequency);
    }
}