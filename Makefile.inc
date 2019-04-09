#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

SEARCH  += $(PWM)/code/headers
SEARCH  += $(PWM)/code/src


# source files in this library
SOURCES += $(wildcard $(PWM)/code/src/*.cpp)

# header files in this library
HEADERS += $(wildcard $(PWM)/code/headers/*.hpp)
