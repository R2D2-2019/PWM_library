# PWM library

include this library by adding the following lines to the makefile.due in your module.
```Makefile
#add I2C_library
PWM		           ?= ../../../libraries/PWM_library
include               $(PWM)/Makefile.inc
```

then include it by adding `#include <pwm.hpp> ` to your files
for more information visit this wiki page:
https://github.com/R2D2-2019/R2D2-2019/wiki/PWM_library 
