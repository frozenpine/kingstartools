############################################################################
#
# UNIX Makefile for the Embedded SQL/Cobol example programs
# ---------------------------------------------------------
#
# Copyright Notice and Disclaimer
# -------------------------------
#       (c) Copyright 1996-2012. 
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
# 	To use this makefile, you need to set the following 
#	environment variables:
#
#       SYBASE          Set to the Sybase installation directory.
#       SYBASE_OCS      Set to the Open Client products directory
#       SYBPLATFORM     Set to the Sybase abbreviation for your platform.
#
# 	The shell script sybopts.sh is used to create platform-specific command
# 	line options for compilation and linking with Sybase and system 
#	libraries. The SYBPLATFORM environment variable must be set in order 
#	for this script to work correctly. Please see 'readme' file for 
#	valid settings.
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
INCLUDE   = -I$(INCDIR)
LIBPATH   = -L$(LIBDIR)

##############################################################################
#
# Macros for compile/link lines
#
##############################################################################


#
# Define the compiler command and compile flags.
#
# `sh sybopts.sh compile` echoes a platform-specific command to invoke the 
# Cobol compiler. You can replace this invocation with the comand to invoke
# your preferred compiler if you like.
#
COMPILE   = `sh sybopts.sh compile`

#
# Get the name and location of the Cobol precompiler
#
PRECOMP   = `sh sybopts.sh precomp` $(INCLUDE)

#
# Define the list of libraries that must be linked with in order to call
# Client-Library routines.
#
# `sh sybopts.sh comlibs` echoes a platform-specific list of lower-layer
# Sybase libraries to link with. 'sh sybopts.sh' with argument 'ct' echoes
# the  Client-Library
#
SYBLIBS   = `sh sybopts.sh cobct ct comlibs`
SYSLIBS   = `sh sybopts.sh syslibs`

##############################################################################
#
# The targets and how to build them.
#
##############################################################################


all: example1 example2 example3 example4 example5 example6 example7
 
example1: example1.cbl
	@ printf "$(COMPILE) example1.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example1.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example1.cbl: example1.pco generic
	@ printf "$(PRECOMP) -m example1.pco\n\n";
	@ $(PRECOMP) -m example1.pco

example2: example2.cbl
	@ printf "$(COMPILE) example2.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example2.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example2.cbl: example2.pco generic
	@ printf "$(PRECOMP) -m example2.pco\n\n";
	@ $(PRECOMP) -m example2.pco

example3: example3.cbl
	@ printf "$(COMPILE) example3.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example3.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example3.cbl: example3.pco generic
	@ printf "$(PRECOMP) -m example3.pco\n\n";
	@ $(PRECOMP) -m example3.pco

example4: example4.cbl
	@ printf "$(COMPILE) example4.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example4.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example4.cbl: example4.pco generic
	@ printf "$(PRECOMP) -m example4.pco\n\n";
	@ $(PRECOMP) -m example4.pco

example5: example5.cbl
	@ printf "$(COMPILE) example5.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example5.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example5.cbl: example5.pco generic
	@ printf "$(PRECOMP) -m example5.pco\n\n";
	@ $(PRECOMP) -m example5.pco

example6: example6.cbl
	@ printf "$(COMPILE) example6.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example6.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example6.cbl: example6.pco generic
	@ printf "$(PRECOMP) -m example6.pco\n\n";
	@ $(PRECOMP) -m example6.pco

example7: example7.cbl
	@ printf "$(COMPILE) example7.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example7.cbl $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example7.cbl: example7.pco generic
	@ printf "$(PRECOMP) -m example7.pco\n\n";
	@ $(PRECOMP) -m example7.pco

# Create the softlink to the sybase include directory.
generic:
	ln -s $(RELDIR) generic

clean:
	rm -f example1 example1.cbl example1.int example1.idy example1.o
	rm -f example2 example2.cbl example2.int example2.idy example2.o
	rm -f example3 example3.cbl example3.int example3.idy example3.o
	rm -f example4 example4.cbl example4.int example4.idy example4.o
	rm -f example5 example5.cbl example5.int example5.idy example5.o
	rm -f example6 example6.cbl example6.int example6.idy example6.o
	rm -f example7 example7.cbl example7.int example7.idy example7.o
	rm -f generic
