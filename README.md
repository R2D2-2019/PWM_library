# PWM library

include this library by adding the following lines to the makefile.due in your module.
```Makefile
#add PWM_library
PWM		           ?= ../../../libraries/PWM_library
include               $(PWM)/Makefile.inc
```

**Note:** This needs to be done before the following block in your makefile.due
``` makefile
# defer to the Makefile.link
include           $(RELATIVE)/Makefile.link
```

then include it by adding `#include <pwm.hpp> ` to your files
for more information visit this wiki page:
https://github.com/R2D2-2019/R2D2-2019/wiki/PWM_library 
