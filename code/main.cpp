#include <hwlib.hpp>
#include <pwm.hpp>

using namespace R2D2::PWM_lib;
int main() {
	WDT->WDT_MR = WDT_MR_WDDIS;
	hwlib::wait_ms(1000);
    enable_pwm();
	auto pwm1 = PWM_c(0);
	auto pwm2 = PWM_c(1);
	set_clock(50,clocks::CLOCKA);
	pwm1.select_frequency(frequencys::f_320hz);
	for(;;){
	}
}