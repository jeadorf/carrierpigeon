#!/usr/bin/env python
#
# Build script.
#

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
import sys
import ConfigParser

verbose = False

def _info(message):
    print message

def _fine(message):
    if verbose:
        print message

class ProjectManager:
    """Retrieves project information when given a project name"""
    pass

class ConfigProjectManager:
    def __init__(self, config):
        self.config = config
    def get_project(self, project_name):
        sect = "project:" + project_name
        # path is required
        path = self.config.get(sect, "path")
        # files is optional
        if self.config.has_option(sect, "files"):
            files = self.config.get(sect, "files").split(",")
        else:
            files = tuple() 
        # dependencies is optional
        if self.config.has_option(sect, "dependencies"):
            dep_tokens = self.config.get(sect, "dependencies").split(",")
            dependencies = [Dependency(
                            dep[:dep.find(":")].strip(),
                            dep[dep.find(":")+1:].strip()) for dep in dep_tokens]
        else:
            dependencies = tuple() 
        print "Project: " + project_name
        print "Dependencies: " + str(dependencies)
        print
        return Project(path, files, dependencies, self) 

class Dependency:
    """Models a dependency. It does not hardlink to the project. Thus, it is
    not required to initialize a valid project instance before declaring a
    dependency on that project."""
    def __init__(self, project_name, file):
        self.project_name = project_name
        self.file = file
    def __repr__(self):
        return self.project_name + ":" + self.file

class Project:
    """Responsible for compilation, linking, flashing, cleaning and all other
    build targets. The builder class does not automatically build dependencies."""
    def __init__(self, path, files, dependencies, project_manager):
        """path -- a relative path from the working directory where
                           the build script is executed. This parameter is
                           likely to vanish in future versions.
           files -- a list of C source files (names only, and no extension!) 
           dependencies -- a list of dependencies"""
        self.path = path
        self.files = files
        self.dependencies = dependencies
        self.project_manager = project_manager
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
            dp = project_manager.get_project(d.project_name)
            cmd.append("-I%s%s" % (dp.path, "/".join(d.file)))
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
            cmd.append("%s%s.o" % (d.project.path, "/".join(d)))
    def _execute(self, cmd):
        _fine("Running '%s'" % " ".join(cmd))
        proc = subprocess.Popen(cmd)
        proc.communicate()
        return proc


if __name__ == "__main__":
    project = sys.argv[1]
    print "Building project '%s'" % project

    cfg = ConfigParser.ConfigParser()
    cfg.read("make.cfg")

    # -- PREREQUISITES --
    # TODO: move to configuration file
    # TODO: replace examples
    path = "experiments/assert"

    files = (
        "main",
        "assert"
    )

    dependencies = (
        Dependency("buttons", "led")
        #("experiments", "buttons", "led"),
        #("experiments", "eeprom", "eeprom")
    )

    # builder = Project(path, files, dependencies)
    project_manager = ConfigProjectManager(cfg)
    project = project_manager.get_project("assert")
    project.compile()
    project.link()
    print "Done."
