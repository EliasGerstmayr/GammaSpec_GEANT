# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/EG415/GEANT/QED

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/EG415/GEANT/QED

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/local/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/local/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/EG415/GEANT/QED/CMakeFiles /Users/EG415/GEANT/QED/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/EG415/GEANT/QED/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named QED

# Build rule for target.
QED: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 QED
.PHONY : QED

# fast build rule for target.
QED/fast:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/build
.PHONY : QED/fast

QED.o: QED.cc.o

.PHONY : QED.o

# target to build an object file
QED.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/QED.cc.o
.PHONY : QED.cc.o

QED.i: QED.cc.i

.PHONY : QED.i

# target to preprocess a source file
QED.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/QED.cc.i
.PHONY : QED.cc.i

QED.s: QED.cc.s

.PHONY : QED.s

# target to generate assembly for a file
QED.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/QED.cc.s
.PHONY : QED.cc.s

src/B1ActionInitialization.o: src/B1ActionInitialization.cc.o

.PHONY : src/B1ActionInitialization.o

# target to build an object file
src/B1ActionInitialization.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1ActionInitialization.cc.o
.PHONY : src/B1ActionInitialization.cc.o

src/B1ActionInitialization.i: src/B1ActionInitialization.cc.i

.PHONY : src/B1ActionInitialization.i

# target to preprocess a source file
src/B1ActionInitialization.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1ActionInitialization.cc.i
.PHONY : src/B1ActionInitialization.cc.i

src/B1ActionInitialization.s: src/B1ActionInitialization.cc.s

.PHONY : src/B1ActionInitialization.s

# target to generate assembly for a file
src/B1ActionInitialization.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1ActionInitialization.cc.s
.PHONY : src/B1ActionInitialization.cc.s

src/B1DetectorConstruction.o: src/B1DetectorConstruction.cc.o

.PHONY : src/B1DetectorConstruction.o

# target to build an object file
src/B1DetectorConstruction.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1DetectorConstruction.cc.o
.PHONY : src/B1DetectorConstruction.cc.o

src/B1DetectorConstruction.i: src/B1DetectorConstruction.cc.i

.PHONY : src/B1DetectorConstruction.i

# target to preprocess a source file
src/B1DetectorConstruction.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1DetectorConstruction.cc.i
.PHONY : src/B1DetectorConstruction.cc.i

src/B1DetectorConstruction.s: src/B1DetectorConstruction.cc.s

.PHONY : src/B1DetectorConstruction.s

# target to generate assembly for a file
src/B1DetectorConstruction.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1DetectorConstruction.cc.s
.PHONY : src/B1DetectorConstruction.cc.s

src/B1EventAction.o: src/B1EventAction.cc.o

.PHONY : src/B1EventAction.o

# target to build an object file
src/B1EventAction.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1EventAction.cc.o
.PHONY : src/B1EventAction.cc.o

src/B1EventAction.i: src/B1EventAction.cc.i

.PHONY : src/B1EventAction.i

# target to preprocess a source file
src/B1EventAction.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1EventAction.cc.i
.PHONY : src/B1EventAction.cc.i

src/B1EventAction.s: src/B1EventAction.cc.s

.PHONY : src/B1EventAction.s

# target to generate assembly for a file
src/B1EventAction.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1EventAction.cc.s
.PHONY : src/B1EventAction.cc.s

src/B1PrimaryGeneratorAction.o: src/B1PrimaryGeneratorAction.cc.o

.PHONY : src/B1PrimaryGeneratorAction.o

# target to build an object file
src/B1PrimaryGeneratorAction.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1PrimaryGeneratorAction.cc.o
.PHONY : src/B1PrimaryGeneratorAction.cc.o

src/B1PrimaryGeneratorAction.i: src/B1PrimaryGeneratorAction.cc.i

.PHONY : src/B1PrimaryGeneratorAction.i

# target to preprocess a source file
src/B1PrimaryGeneratorAction.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1PrimaryGeneratorAction.cc.i
.PHONY : src/B1PrimaryGeneratorAction.cc.i

src/B1PrimaryGeneratorAction.s: src/B1PrimaryGeneratorAction.cc.s

.PHONY : src/B1PrimaryGeneratorAction.s

# target to generate assembly for a file
src/B1PrimaryGeneratorAction.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1PrimaryGeneratorAction.cc.s
.PHONY : src/B1PrimaryGeneratorAction.cc.s

src/B1RunAction.o: src/B1RunAction.cc.o

.PHONY : src/B1RunAction.o

# target to build an object file
src/B1RunAction.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1RunAction.cc.o
.PHONY : src/B1RunAction.cc.o

src/B1RunAction.i: src/B1RunAction.cc.i

.PHONY : src/B1RunAction.i

# target to preprocess a source file
src/B1RunAction.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1RunAction.cc.i
.PHONY : src/B1RunAction.cc.i

src/B1RunAction.s: src/B1RunAction.cc.s

.PHONY : src/B1RunAction.s

# target to generate assembly for a file
src/B1RunAction.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1RunAction.cc.s
.PHONY : src/B1RunAction.cc.s

src/B1SteppingAction.o: src/B1SteppingAction.cc.o

.PHONY : src/B1SteppingAction.o

# target to build an object file
src/B1SteppingAction.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1SteppingAction.cc.o
.PHONY : src/B1SteppingAction.cc.o

src/B1SteppingAction.i: src/B1SteppingAction.cc.i

.PHONY : src/B1SteppingAction.i

# target to preprocess a source file
src/B1SteppingAction.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1SteppingAction.cc.i
.PHONY : src/B1SteppingAction.cc.i

src/B1SteppingAction.s: src/B1SteppingAction.cc.s

.PHONY : src/B1SteppingAction.s

# target to generate assembly for a file
src/B1SteppingAction.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/B1SteppingAction.cc.s
.PHONY : src/B1SteppingAction.cc.s

src/HistoManager.o: src/HistoManager.cc.o

.PHONY : src/HistoManager.o

# target to build an object file
src/HistoManager.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/HistoManager.cc.o
.PHONY : src/HistoManager.cc.o

src/HistoManager.i: src/HistoManager.cc.i

.PHONY : src/HistoManager.i

# target to preprocess a source file
src/HistoManager.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/HistoManager.cc.i
.PHONY : src/HistoManager.cc.i

src/HistoManager.s: src/HistoManager.cc.s

.PHONY : src/HistoManager.s

# target to generate assembly for a file
src/HistoManager.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/HistoManager.cc.s
.PHONY : src/HistoManager.cc.s

src/HistoMessenger.o: src/HistoMessenger.cc.o

.PHONY : src/HistoMessenger.o

# target to build an object file
src/HistoMessenger.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/HistoMessenger.cc.o
.PHONY : src/HistoMessenger.cc.o

src/HistoMessenger.i: src/HistoMessenger.cc.i

.PHONY : src/HistoMessenger.i

# target to preprocess a source file
src/HistoMessenger.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/HistoMessenger.cc.i
.PHONY : src/HistoMessenger.cc.i

src/HistoMessenger.s: src/HistoMessenger.cc.s

.PHONY : src/HistoMessenger.s

# target to generate assembly for a file
src/HistoMessenger.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/HistoMessenger.cc.s
.PHONY : src/HistoMessenger.cc.s

src/MagneticField.o: src/MagneticField.cc.o

.PHONY : src/MagneticField.o

# target to build an object file
src/MagneticField.cc.o:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/MagneticField.cc.o
.PHONY : src/MagneticField.cc.o

src/MagneticField.i: src/MagneticField.cc.i

.PHONY : src/MagneticField.i

# target to preprocess a source file
src/MagneticField.cc.i:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/MagneticField.cc.i
.PHONY : src/MagneticField.cc.i

src/MagneticField.s: src/MagneticField.cc.s

.PHONY : src/MagneticField.s

# target to generate assembly for a file
src/MagneticField.cc.s:
	$(MAKE) -f CMakeFiles/QED.dir/build.make CMakeFiles/QED.dir/src/MagneticField.cc.s
.PHONY : src/MagneticField.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... QED"
	@echo "... QED.o"
	@echo "... QED.i"
	@echo "... QED.s"
	@echo "... src/B1ActionInitialization.o"
	@echo "... src/B1ActionInitialization.i"
	@echo "... src/B1ActionInitialization.s"
	@echo "... src/B1DetectorConstruction.o"
	@echo "... src/B1DetectorConstruction.i"
	@echo "... src/B1DetectorConstruction.s"
	@echo "... src/B1EventAction.o"
	@echo "... src/B1EventAction.i"
	@echo "... src/B1EventAction.s"
	@echo "... src/B1PrimaryGeneratorAction.o"
	@echo "... src/B1PrimaryGeneratorAction.i"
	@echo "... src/B1PrimaryGeneratorAction.s"
	@echo "... src/B1RunAction.o"
	@echo "... src/B1RunAction.i"
	@echo "... src/B1RunAction.s"
	@echo "... src/B1SteppingAction.o"
	@echo "... src/B1SteppingAction.i"
	@echo "... src/B1SteppingAction.s"
	@echo "... src/HistoManager.o"
	@echo "... src/HistoManager.i"
	@echo "... src/HistoManager.s"
	@echo "... src/HistoMessenger.o"
	@echo "... src/HistoMessenger.i"
	@echo "... src/HistoMessenger.s"
	@echo "... src/MagneticField.o"
	@echo "... src/MagneticField.i"
	@echo "... src/MagneticField.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
