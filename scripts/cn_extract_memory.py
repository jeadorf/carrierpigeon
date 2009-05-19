#!/usr/bin/env python

"""Extracts data from all memory parts of the atmega8515 microcontroller, such
as flash data, hfuse and lfuse bytes, etc. and saves it to files in all formats
provided by avrdude. See manpage of avrdude for further information.

Be careful not to call this script too often or just for fun, as it produces a
lot of read operations on the memory parts of the microcontroller.

If you need to read the data frequently and need different formats, please look
for conversion utilities, making this script largely superfluous."""

import optparse
import subprocess

def main():
    memory_types = ["calibration",
                    "eeprom",
                    "efuse",
                    "flash",
                    "hfuse",
                    "lfuse",
                    "lock",
                    "signature"]

    # Note that format flags auto-detect ('a') and immediate ('m') do not make
    # any sense here and are therefore omitted. 
    formats = ["i", "s", "r", "d", "h", "o", "b"]

    for m in memory_types:
        for f in formats:
            # some options are hard-coded here and may be extracted, such as
            # port, programmer-id and device identifier
            avrdude = ("avrdude",
                "-c", "avrisp2",
                "-P", "usb",
                "-p", "atmega8515",
                "-U", "%s:r:%s:%s" % (m, m + "_" + f, f))
            p = subprocess.Popen(avrdude)
            p.communicate()        
            if p.returncode != 0:
                print "Couldn't extract memory from '%s' in format '%s'" % (m, f)

if __name__ == "__main__":
    main()
