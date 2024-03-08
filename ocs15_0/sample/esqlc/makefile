############################################################################
#
# UNIX Makefile for the Embedded SQL/C example programs
# -----------------------------------------------------
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
#	libraries. The SYBPLATFORM environment variable must be set in 
#	order for this script to work correctly. Please see 'readme' file 
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
# Define the compiler command and compile flags.
#
# `sh sybopts.sh compile` echoes a platform-specific command to invoke the C
# compiler. You can replace this invocation with the comand to invoke your
# preferred compiler if you like.
#
COMPILE   = `sh sybopts.sh compile` $(INCLUDE) 

#
# Get the name and location of the C precompiler
#
PRECOMP   = `sh sybopts.sh precomp`

#
# Define the list of libraries that must be linked with in order to call
# Client-Library routines.
#
# `sh sybopts.sh comlibs` echoes a platform-specific list of lower-layer
# Sybase libraries to link with. 'sh sybopts.sh' with argument 'ct' echoes
# the  Client-Library
#
SYBLIBS   = `sh sybopts.sh ct comlibs`
SYSLIBS   = `sh sybopts.sh syslibs` 

##############################################################################
#
# The targets and how to build them.
#
##############################################################################


all: example1 example2 example4 example5 example6 example7 example8 exampleHA exampleHA2
 
uni: uni_example1 uni_example2
 
example1: example1.c sybsqlex.h
	@ printf "$(COMPILE) example1.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example1.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@
 
example1.c: example1.cp
	@ printf "$(PRECOMP) -m example1.cp\n\n";
	@ $(PRECOMP) -m example1.cp

example2: example2.c sybsqlex.h
	@ printf "$(COMPILE) example2.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example2.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example2.c: example2.cp
	@ printf "$(PRECOMP) -m example2.cp\n\n";
	@ $(PRECOMP) -m example2.cp

example4: example4.c sybsqlex.h
	@ printf "$(COMPILE) example4.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example4.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example4.c: example4.cp
	@ printf "$(PRECOMP) -m example4.cp\n\n";
	@ $(PRECOMP) -m example4.cp

example5: example5.c sybsqlex.h
	@ printf "$(COMPILE) example5.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example5.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example5.c: example5.cp
	@ printf "$(PRECOMP) -m example5.cp\n\n";
	@ $(PRECOMP) -m example5.cp

example6: example6.c sybsqlex.h
	@ printf "$(COMPILE) example6.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example6.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example6.c: example6.cp
	@ printf "$(PRECOMP) -m example6.cp\n\n";
	@ $(PRECOMP) -m example6.cp

example7: example7.c sybsqlex.h
	@ printf "$(COMPILE) example7.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example7.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example7.c: example7.cp
	@ printf "$(PRECOMP) -m example7.cp\n\n";
	@ $(PRECOMP) -m example7.cp

example8: example8.c sybsqlex.h
	@ printf "$(COMPILE) example8.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) example8.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@

example8.c: example8.cp
	@ printf "$(PRECOMP) -m example8.cp\n\n";
	@ $(PRECOMP) -m example8.cp

exampleHA: exampleHA.c sybsqlex.h
	@ printf "$(COMPILE) exampleHA.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) exampleHA.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@
 
exampleHA.c: exampleHA.cp
	@ printf "$(PRECOMP) -H -m exampleHA.cp\n\n";
	@ $(PRECOMP) -H -m exampleHA.cp

exampleHA2: exampleHA2.c sybsqlex.h
	@ printf "$(COMPILE) exampleHA2.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) exampleHA2.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@
 
exampleHA2.c: exampleHA2.cp
	@ printf "$(PRECOMP) -H -m exampleHA2.cp\n\n";
	@ $(PRECOMP) -H -m exampleHA2.cp

uni_example1: uni_example1.c sybsqlex.h
	@ printf "$(COMPILE) uni_example1.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) uni_example1.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@
 
uni_example1.c: uni_example1.cp
	@ printf "$(PRECOMP) -m uni_example1.cp\n\n";
	@ $(PRECOMP) -m uni_example1.cp

uni_example2: uni_example2.c sybsqlex.h
	@ printf "$(COMPILE) uni_example2.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) uni_example2.c $(RELDIR)/include/sybesql.c $(LIBPATH) $(SYBLIBS) $(SYSLIBS) -o $@
 
uni_example2.c: uni_example2.cp
	@ printf "$(PRECOMP) -m uni_example2.cp\n\n";
	@ $(PRECOMP) -m uni_example2.cp

clean: 
	rm -f *.o example1 example2 example4 example5 example6 example7 example8 exampleHA exampleHA2 \
	uni_example1 uni_example2
	rm -f example1.c example2.c example4.c example5.c example6.c example7.c example8.c exampleHA.c \
	exampleHA2.c uni_example1.c uni_example2.c
	rm -f generic
