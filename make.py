#!/usr/bin/env python
#
# Build script.
#
"""Compiles and links projects."""

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
# TODO: write some tests, especially for configuration parsing
# TODO: wrap command line arguments with quotes if necessary
# TODO: support inclusion of header files only
# TODO: improve error handling if project with name xyz does not exist
# TODO: add command-line option to control verbosity
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
    """Retrieves project information when given a project name and keeps track
    of the build status."""
    # def get_project(self, name) -- retrieves a project
    # def build(self, name)       -- ensures that a certain project is built
    pass

class ConfigProjectManager:
    # TODO: implement caching
    def __init__(self, config):
        self.config = config
        self.projects = {}
        self.builds = set()
    def get_project(self, name):
        if not name in self.projects:
            self._load_project(name)
        return self.projects[name]
    def _load_project(self, name):
        sect = "project:" + name 
        # path is required
        path = self.config.get(sect, "path")
        # files is optional
        if self.config.has_option(sect, "files"):
            files = [f.strip() for f in self.config.get(sect, "files").split(",")]
        else:
            files = [] 
        # dependencies is optional
        if self.config.has_option(sect, "dependencies"):
            dep_tokens = self.config.get(sect, "dependencies").split(",")
            dependencies = [Dependency(
                            dep[:dep.find(":")].strip(),
                            dep[dep.find(":")+1:].strip()) for dep in dep_tokens]
        else:
            dependencies = [] 
        _fine("Loaded project '%s'" % name) 
        # TODO: check conflicts
        self.projects[name] = Project(name, path, files, dependencies, self) 
    def build(self, name):
        if self._needs_build(name):
            project = self.get_project(name)
            project.build()
            self._set_build_completed(name)
        else:
            _fine("Nothing to do. Project '%s' has already been built." % name)
    def _set_build_completed(self, name):
        self.builds.add(name)
    def _needs_build(self, name):
        return name not in self.builds

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
    def __init__(self, name, path, files, dependencies, project_manager):
        """path -- a relative path from the working directory where
                           the build script is executed. This parameter is
                           likely to vanish in future versions.
           files -- a list of C source files (names only, and no extension!) 
           dependencies -- a list of dependencies"""
        self.name = name
        self.path = path
        self.files = files
        self.dependencies = dependencies
        self.project_manager = project_manager
        # path from target subdir to script location
        self.path_to_root = "../../"
        path_copy = path
        while os.path.split(path_copy)[0] != "":
            self.path_to_root += "../"
            path_copy = os.path.split(path_copy)[0]
    def build(self):
        """Compiles and links this project."""
        _info("Building '%s' ..." % self.name)
        self.compile()
        self.link()
        self.strip()
        # TODO: find out was avr dump does (in makefile)
        self.objcopy()
    def compile(self):
        _info("Compiling '%s' ..." % self.name)
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
            cmd.append(os.path.join(self.path_to_root, self.path, f + ".c"))
    def _append_include_paths(self, cmd):
        for d in self.dependencies:
            dp = project_manager.get_project(d.project_name)
            cmd.append("-I%s" % os.path.join(self.path_to_root, dp.path, os.path.dirname(d.file)))
    def link(self):
        _info("Linking '%s' ..." % self.name)
        link_cmd = [
            "avr-gcc",
            "-O2",
            "-mmcu=atmega8515",
            "-o", "main"]
        self._build_dependencies()
        self._append_compile_output_files(link_cmd)
        self._append_dependencies_output_files(link_cmd)
        self._execute(link_cmd)
    def _append_compile_output_files(self, cmd):
        for f in self.files:
            cmd.append(f + ".o")
    def _append_dependencies_output_files(self, cmd):
        for d in self.dependencies:
            dp = project_manager.get_project(d.project_name)
            cmd.append("%s.o" % os.path.join(self.path_to_root, "target", dp.path, d.file))
    def _build_dependencies(self):
        for d in self.dependencies:
            project_manager.build(d.project_name)
    def strip(self):
        _info("Stripping '%s' ..." % self.name)
        strip_cmd = [
            "avr-strip",
            "main",
            "-o", "main-stripped"]
        self._execute(strip_cmd)
    def objcopy(self):
        # TODO: find better name for this function
        _info("Objcopy '%s' ..." % self.name)
        srec_cmd = [
            "avr-objcopy",
            "-O", "srec",
            "main-stripped",
            "main.srec"]
        ihex_cmd = [
            "avr-objcopy",
            "-O", "ihex",
            "main-stripped",
            "main.hex"]
        binary_cmd = [
            "avr-objcopy",
            "-O", "binary",
            "main-stripped",
            "main.flash"]
        self._execute(srec_cmd)
        self._execute(ihex_cmd)
        self._execute(binary_cmd)
    def _execute(self, cmd):
        _fine("Running '%s'" % " ".join(cmd))
        wd = os.path.join("target", self.path)
        if not os.path.exists(wd):
            _fine("Creating directory '%s'" % wd)
            os.makedirs(wd)
        _fine("Working directory is '%s'" % wd)
        proc = subprocess.Popen(cmd, cwd = wd)
        proc.communicate()
        return proc


if __name__ == "__main__":
    _info("Building")
    _info("-------------------------------------------------------------------")

    cfg = ConfigParser.ConfigParser()
    cfg.read("make.cfg")
    project_manager = ConfigProjectManager(cfg)
    for project_name in sys.argv[1:]:
        project_manager.build(project_name)
        
    _info("-------------------------------------------------------------------")
    _info("Done.")
