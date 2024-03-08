############################################################################
#
# UNIX Makefile for the DB-Library example programs 
# -----------------------------------------------------
#
# Copyright Notice and Disclaimer
# -------------------------------
#       (c) Copyright 1993-2012. 
#       Sybase, Inc. All rights reserved.  
#       Unpublished rights reserved under U.S. copyright laws.
#       This material is the confidential and trade secret information of
#       Sybase, Inc.
#
#       Sybase grants Licensee a non-exclusive license to use, reproduce, 
#       modify, and distribute the sample source code below (the "Sample Code"),
#       subject to the following conditions: 
#
#       (i) redistributions must retain the above copyright notice; 
#
#       (ii) Sybase shall have no obligation to correct errors or deliver 
#       updates to the Sample Code or provide any other support for the 
#       Sample Code; 
#
#       (iii) Licensee may use the Sample Code to develop applications 
#       (the "Licensee Applications") and may distribute the Sample Code in 
#       whole or in part as part of such Licensee Applications, however in no 
#       event shall Licensee distribute the Sample Code on a standalone basis; 
#
#       (iv) and subject to the following disclaimer:
#       THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#       INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
#       AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
#       SYBASE, INC. OR ITS LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
#       INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
#       BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
#       OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
#       ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
#       TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
#       USE OF THE SAMPLE CODE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
#       DAMAGE.
#
# Description
# ----------- 
#	To use this makefile, you need to set the following 
#	environment variables:
#
#       SYBASE	  	Set to the Sybase installation directory.
#       SYBASE_OCS      Set to the Open Client products directory
#       SYBPLATFORM     Set to the Sybase abbreviation for your platform.
#
# 	The shell script sybopts.sh is used to create platform-specific command
# 	line options for compilation and linking with Sybase and system 
#	libraries. The SYBPLATFORM environment variable must be set in 
#	order for this script to work correctly. Please see README file 
#	for valid settings.
#
#############################################################################
#############################################################################
#
# Directories and search paths.
#
#############################################################################

SHELL     = /bin/sh
RELDIR    = $(SYBASE)/$(SYBASE_OCS)

# Sybase include directory
INCDIR    = $(RELDIR)/include

#
# Modify the following line to use debug/nondebug libraries
#
#LIBDIR   = $(RELDIR)/devlib
LIBDIR    = $(RELDIR)/lib

# Specify the include and library search paths for the compiler
INCLUDE   = -I. -I$(INCDIR)
LIBPATH   = -L$(LIBDIR)

##############################################################################
#
# Macros for compile/link lines
#
##############################################################################

#
# Specify the compile-time defines.
# Use -DDEBUG only if linking with devlib/*
#
#DEFS = -DDEBUG

#
# Define the compiler command and compile flags.
#
# `sh sybopts.sh compile` echoes a platform-specific command to invoke the C
# compiler. You can replace this invocation with the comand to invoke your
# preferred compiler if you like.
#
COMPILE = `$(SHELL) sybopts.sh compile` $(INCLUDE) $(DEFS)

#
# Define the list of libraries that must be linked with in order to call
# DB-Library
#
# 'sh sybopts.sh' with argument 'dblib' echoes the DB-Library.
#
# `sh sybopts.sh syslibs` echoes a platform-specific list of system libraries
# that are required.
#
DBLIB = `$(SHELL) sybopts.sh dblib`
UNILIB = `$(SHELL) sybopts.sh unilib`
SYSLIBS = `$(SHELL) sybopts.sh syslibs`

##############################################################################
#
# The targets and how to build them.
#
##############################################################################


all: example1 example2 example3 example4 example5 example6 example7 \
     example8 example9 example10 example11 example12 bulkcopy twophase

example1: example1.c
	@ printf "$(COMPILE) example1.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example1.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example2: example2.c
	@ printf "$(COMPILE) example2.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example2.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example3: example3.c
	@ printf "$(COMPILE) example3.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example3.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example4: example4.c
	@ printf "$(COMPILE) example4.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example4.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example5: example5.c
	@ printf "$(COMPILE) example5.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example5.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example6: example6.c
	@ printf "$(COMPILE) example6.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example6.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example7: example7.c
	@ printf "$(COMPILE) example7.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example7.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example8: example8.c
	@ printf "$(COMPILE) example8.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example8.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example9: example9.c
	@ printf "$(COMPILE) example9.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example9.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example10: example10.c
	@ printf "$(COMPILE) example10.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example10.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example11: example11.c
	@ printf "$(COMPILE) example11.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example11.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

example12: example12.c
	@ printf "$(COMPILE) example12.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) example12.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

bulkcopy: bulkcopy.c
	@ printf "$(COMPILE) bulkcopy.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) bulkcopy.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

twophase: twophase.c
	@ printf "$(COMPILE) twophase.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@\n\n";
	@ $(COMPILE) twophase.c $(LIBPATH) $(DBLIB) $(UNILIB) $(SYSLIBS)  -o $@

clean:
	rm -f example1 example2 example3 example4 example5 example6 \
	      example7 example8 example9 example10 example11 example12 \
	      bulkcopy twophase *.o
