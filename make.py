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
    # Note that you have also have to include transitive dependencies.
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
# TODO: make compiler options configurable
# TODO: make linker options configurable
# TODO: implement target clean (clean)
# TODO: write some tests, especially for configuration parsing
# TODO: support inclusion of header files only
# TODO: improve error handling if project with name xyz does not exist
import subprocess
import os
import sys
import ConfigParser
import optparse

verbose = False 

def _info(message):
    print message

def _fine(message):
    if verbose:
        print message

def _recursively_remove_dir(top):
    for root, dirs, files in os.walk(top, topdown=False):
        for name in files:
            _fine("Removing '%s'" % os.path.join(root, name))
            os.remove(os.path.join(root, name))
        for name in dirs:
            _fine("Removing '%s'" % os.path.join(root, name))
            os.rmdir(os.path.join(root, name))

class BuildException(Exception):
    pass

class ProjectManager:
    """Retrieves project information when given a project name and keeps track
    of the build status."""
    # def get_project(self, name)  -- retrieves a project
    # def list_project_names(self) -- list the names of all defined projects
    # def build(self, name)        -- ensures that a certain project is built
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
    def list_project_names(self):
        names = []
        for sect in self.config.sections():
            if sect.startswith("project:"):
                names.append(sect.split(":")[1])
        return names
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
        if self._is_executable():
            self.link()
            self.strip()
            # TODO: find out was avr dump does (in makefile)
            self.objcopy()
        else:
            _fine("Skipping link, strip and objcopy. Project '%s' does not have a main file." % self.name)
    def _is_executable(self):
        # only link if main
        return "main" in self.files
    def compile(self):
        """Compiles all sources. This step needs access to the header files
        but does not require other projects to be built"""
        _info("Compiling '%s' ..." % self.name)
        # TODO: f_cpu and other compiler options are hard-coded
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
        # change script here if you want to automatically include transitive dependencies
        for d in self.dependencies:
            dp = project_manager.get_project(d.project_name)
            ipath = "-I%s" % os.path.join(self.path_to_root, dp.path, os.path.dirname(d.file))
            if ipath not in cmd:
                cmd.append(ipath)
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
        # change script here if you want to automatically include transitive dependencies
        for d in self.dependencies:
            dp = project_manager.get_project(d.project_name)
            of = "%s.o" % os.path.join(self.path_to_root, "target", dp.path, d.file)
            if of not in cmd:
                cmd.append(of)
    def _build_dependencies(self):
        for d in self.dependencies:
            project_manager.build(d.project_name)
    def strip(self):
        if not self._is_executable():
            raise BuildException("Project is not executable")
        _info("Stripping '%s' ..." % self.name)
        strip_cmd = [
            "avr-strip",
            "main",
            "-o", "main-stripped"]
        self._execute(strip_cmd)
    def objcopy(self):
        if not self._is_executable():
            raise BuildException("Project is not executable")
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
    def program(self):
        if not self._is_executable():
            raise BuildException("Project is not executable")
        program_cmd = [
            "avrdude",
            "-c", "avrisp2",
            "-P", "usb",
            "-p", "atmega8515",
            "-U", "hfuse:w:0xdd:m",
            "-U", "lfuse:w:0x3f:m",
            "-U", "lock:w:0x3f:m",
            "-U", "flash:w:main.srec"]
        self._execute(program_cmd)
    def clean(self):
        # Check for marker
        if os.path.exists("target"):
            if os.path.exists("target/build_marker"):
                top = os.path.join("target", self.path)
                _recursively_remove_dir(top)
            else:
                raise BuildException("Cleaning aborted as a safety restriction against "
                                     "accidental removal. Target directory does not seem "
                                     "to contain a build marker file.")
    def _execute(self, cmd):
        _fine("Running '%s'" % " ".join(cmd))
        # mark the target directory. This is a safety mechanism used by the
        # clean function which checks for the marker before deleting any
        # files or directories on the file system.
        open("target/build_marker", "a").close()
        # construct working directory in order to make all commands put their output
        # into the designated target subfolder.
        wd = os.path.join("target", self.path)
        if not os.path.exists(wd):
            _fine("Creating directory '%s'" % wd)
            os.makedirs(wd)
        _fine("Working directory is '%s'" % wd)
        proc = subprocess.Popen(cmd, cwd = wd)
        proc.communicate()
        # fail-fast
        if proc.returncode != 0:
            raise BuildException("Build failed. Received bad status code "
                                 "'%d' from command '%s'" % (proc.returncode, cmd))
        return proc



def _add_build_option(option_parser):
    option_parser.add_option(
        "-b",
        "--build",
        action="store_true",
        dest="build",
        default=False,
        help="Build projects.")

def _add_clean_option(option_parser):
    option_parser.add_option(
        "-c",
        "--clean",
        action="store_true",
        dest="clean",
        default=False,
        help="Clean project directory. Will only empty directories not remove them completely."
    )

def _add_program_option(option_parser):
    option_parser.add_option(
        "-p",
        "--program",
        action="store_true",
        dest="program",
        default=False,
        help="Flash binary on target platform."
    )

def _add_verbosity_option(option_parser):
    option_parser.add_option(
        "-v",
        "--verbose",
        action="store_true",
        dest="verbose",
        default=False,
        help="Be talkative."
    )

if __name__ == "__main__":
    option_parser = optparse.OptionParser("./make.py [OPTIONS] [PROJECT...]")
    _add_build_option(option_parser)
    _add_clean_option(option_parser)
    _add_program_option(option_parser)
    _add_verbosity_option(option_parser)
    options, args = option_parser.parse_args()

    verbose = options.verbose

    cfg = ConfigParser.ConfigParser()
    cfg.read("make.cfg")
    project_manager = ConfigProjectManager(cfg)
    
    if len(args) == 0:
        if options.program:
            _info("Cannot use --program and without specifying a project")
            exit(1)
        else:
            # get ALL projects
            _fine("No projects explicitly specified - operating on all projects!")
            args = project_manager.list_project_names()

    if not options.clean and not options.program:
        options.build = True

    try:
        if options.clean:
            _info("-------------------------------------------------------------------")
            _info("Cleaning")
            _info("-------------------------------------------------------------------")
            for project_name in args: 
                proj = project_manager.get_project(project_name)
                proj.clean()
       
        if options.program or options.build:
            _info("-------------------------------------------------------------------")
            _info("Building")
            _info("-------------------------------------------------------------------")
            
            for project_name in args: 
                project_manager.build(project_name)

        if options.program:
            _info("-------------------------------------------------------------------")
            _info("Programming")
            _info("-------------------------------------------------------------------")
            
            proj = project_manager.get_project(args[-1])
            proj.program()
                
        _info("-------------------------------------------------------------------")
        _info("Done.")
        _info("-------------------------------------------------------------------")
    except BuildException, be:
        _info(be)
        _info("BUILD FAILED!") # speak up on failure
        exit(1)
