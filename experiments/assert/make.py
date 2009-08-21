#!/usr/bin/env python
#
# Build script.
#

# -- PREREQUISITES --
# TODO: move to configuration file
# TODO: replace examples
path_to_root = "../../"

files = (
    "main",
    "assert"
)

dependencies = (
    ("experiments", "buttons", "led"),
    ("experiments", "eeprom", "eeprom")
)

# -- ACTUAL BUILD PROCESS --
import subprocess

compile = [
    "avr-gcc",
    "-O2",
    "-DF_CPU=16000000",
    "-mmcu=atmega8515",
    "-c"]

for f in files:
    compile.append(f + ".c")

for d in dependencies:
    compile.append("-I%s%s" % (path_to_root, "/".join(d[:-1])))

print " ".join(compile)

compile_proc = subprocess.Popen(compile)
compile_proc.communicate()

link = [
    "avr-gcc",
    "-O2",
    "-mmcu=atmega8515",
    "-o main.o"]

for f in files:
    link.append(f + ".o")

for d in dependencies:
    link.append("%s%s.o" % (path_to_root, "/".join(d)))

print " ".join(link)

link_proc = subprocess.Popen(link)
link_proc.communicate()


