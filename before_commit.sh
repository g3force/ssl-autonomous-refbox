#!/bin/sh

# execute this script, before you commit your changed.

make clean
astyle --options=astyle.conf -r ssl-refbox/*.cc ssl-refbox/*.cpp
