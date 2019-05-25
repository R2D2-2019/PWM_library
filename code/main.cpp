#include <hwlib.hpp>
#include <pwm.hpp>

using namespace r2d2::pwm_lib;

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    auto pwm1 = pwm_c(0);
    auto pwm2 = pwm_c(1);

    pwm1.select_frequency(frequencies::f_320hz);

    for (;;) {

    }
}