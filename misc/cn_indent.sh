#!/bin/sh
#
# Reformats all C source files.
#
find ../ -name "*.c" -exec indent --indent-level4 --no-tabs --brace-indent0 --dont-break-procedure-type --no-space-after-function-call-names {} \;

