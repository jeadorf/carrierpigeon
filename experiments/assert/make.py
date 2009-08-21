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
# TODO: do error checking
# TODO: add documentation
# TODO: make compiler options configurable
# TODO: make linker options configurable
# TODO: support verbosity configuration
# TODO: add target clean (clean)
# TODO: add target objcopy (copy)
# TODO: add target objdump (dump)
# TODO: add target avrstrip (strip)
# TODO: add target avrdude (flash)
import subprocess
import os

verbose = False

def _info(message):
    print message

def _fine(message):
    if verbose:
        print message

class Builder:
    def __init__(self, path_to_root, files, dependencies):
        self.path_to_root = path_to_root
        self.files = files
        self.dependencies = dependencies
    def compile(self):
        _info("Compiling ...")
        compile_cmd = [
            "avr-gcc",
            "-c",
            "-O2",
            "-DF_CPU=16000000",
            "-mmcu=atmega8515"] 
        self._append_compile_sources(compile_cmd)
        self._append_include_paths(compile_cmd)
        self._execute(compile_cmd)
    def _append_compile_sources(self, cmd):
        for f in self.files:
            cmd.append(f + ".c")
    def _append_include_paths(self, cmd):
        for d in self.dependencies:
            cmd.append("-I%s%s" % (path_to_root, "/".join(d[:-1])))
    def link(self):
        _info("Linking ...")
        link_cmd = [
            "avr-gcc",
            "-O2",
            "-mmcu=atmega8515",
            "-o main"]
        self._append_compile_output_files(link_cmd)
        self._append_dependencies_output_files(link_cmd)
        self._execute(link_cmd)
    def _append_compile_output_files(self, cmd):
        for f in files:
            cmd.append(f + ".o")
    def _append_dependencies_output_files(self, cmd):
        for d in dependencies:
            cmd.append("%s%s.o" % (path_to_root, "/".join(d)))
    def _execute(self, cmd):
        _fine("Running '%s'" % " ".join(cmd))
        proc = subprocess.Popen(cmd)
        proc.communicate()
        return proc

if __name__ == "__main__":
    builder = Builder(path_to_root, files, dependencies)
    builder.compile()
    builder.link()
    print "Done."
