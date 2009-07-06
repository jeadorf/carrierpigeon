import fbuild
import fbuild.builders.c

PROG_METHOD = 'avrisp2'
PORT = 'usb'
MCU = 'atmega8515'

def build():
    static = fbuild.builders.c.guess_static(platform={'avr', 'gcc'}, mmcu=MCU)
    exe = static.build_exe('main', ['main.c', 'buttons.c', 'led.c'])

    fbuild.execute(['avr-strip', exe])
    fbuild.execute(['avr-objcopy', '-O', 'srec', exe, 'main.srec'])
    fbuild.execute(['avr-objcopy', '-O', 'ihex', exe, 'main.hex'])
    fbuild.execute(['avr-objcopy', '-O', 'binary', exe, 'main.flash'])
    #fbuild.execute(['avrdude', '-U', 'foo.srec'])
    fbuild.execute(['avrdude', '-c', PROG_METHOD,
        '-P', PORT, '-p', MCU,
        '-U', 'hfuse:w:0xdd:m',
        '-U', 'lfuse:w:0x3f:m',
        '-U', 'lock:w:0x3f:m',
        '-U', 'flash:w:main.srec'
    ])
