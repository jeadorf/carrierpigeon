#!/usr/bin/env python
#
# Build script.
#
"""Builds projects using the avr-gcc compiler. Projects are defined in a
configuration file named 'make.cfg' that follows the Python ConfigParser
syntax. The syntax is explained by example:
    
    [general]
    # Options that apply to all projects are defined in this special section.
    # There are no such options yet.

    [project:foo]
    # Defines a project identified by 'foo'. The identifier 'foo' will
    # represent the project throughout the whole config file as well as the 
    # build process.
    path = examples/foo
    # The path is relative to the location of the config file and it is required.
    files = main, bar
    # Specifies the names of the C source files without extension (please do not
    # use fancy names here) that should be compiled during the build process.
    dependencies = mylib:sensor, mylib2:led
    # Tells the build script which files this project depends on. The script will
    # automatically add the project paths to the GCC include path such that the
    # C header files can be found during the compilation. Also, these directives
    # will tell the script on what binaries to link.

In order to build projects, call 'python make.py <project1> <project2> ...'.
The script will automatically build dependencies and also build each
project at maximum once.
"""


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

class ConfigProjectManager(ProjectManager):
    """Retrieves project information from a configuration file"""
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
    """Models a dependency. A dependency object only links projects using
    identifiers and not using instances of 'Project'. This makes it possible
    to declare dependencies on projects solely on ID without having to load
    them before."""
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
                   the build script is executed.
           files -- a list of C source files (names only, and no extension!) 
                    Do not use any fancy names here.
           dependencies -- a list of dependencies"""
        self.name = name
        self.path = path
        self.files = files
        self.dependencies = dependencies
        self.project_manager = project_manager
        # path from target subdir to script location
        self.path_to_root = self._construct_path_to_root(path)
    def _construct_path_to_root(self, path):
        # a bit hacky
        ptr = "../../"
        while os.path.split(path)[0] != "":
            ptr += "../"
            path = os.path.split(path)[0]
        return ptr
    def build(self):
        """Triggers all necessary actions to create a 'flashable' binary for
        this project. Do not call this function directly, use an instance of
        ProjectManager instead."""
        _info("Building '%s' ..." % self.name)
        self.compile()
        self.link()
        self.strip()
        # TODO: find out was avr dump does (in makefile)
        self.objcopy()
    def compile(self):
        """Compiles all sources. This step needs access to the header files
        but does not require other projects to be built"""
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
        """Builds all projects this project depends on and then links the binaries."""
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
