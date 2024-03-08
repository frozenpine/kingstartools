############################################################################
#
# UNIX Makefile for the db-library to ct-library example programs.
#
# Copyright Notice and Disclaimer
# -------------------------------
#       (c) Copyright 2005-2012.
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
#       To use this makefile, you need to set the following environment
#       variables:
#
#       SYBASE          Set to the Sybase installation directory.
#       SYBASE_OCS      Set to the Open Client products directory
#       SYBPLATFORM     Set to the Sybase abbreviation for your platform.
#
#       The shell script sybopts.sh is used to create platform-specific command
#       line options for compilation and linking with Sybase and system
#       libraries. The SYBPLATFORM environment variable must be set in order
#       for this script to work correctly. Please see README file for valid
#       settings.
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

# Directory containing source code for Client-Library samples.
CTSAMPLEDIR	= $(SYBASE)/$(SYBASE_OCS)/sample/ctlibrary
EXUTILS_C	= $(CTSAMPLEDIR)/exutils.c
EXUTILS_H	= $(CTSAMPLEDIR)/exutils.h

# Specify the include and library search paths for the compiler
INCLUDE   = -I. -I$(INCDIR) -I$(CTSAMPLEDIR)
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
#DEFS 	= -DDEBUG -D$(SYBPLATFORM)=1 
DEFS 	= -D$(SYBPLATFORM)=1 

#
# Define the compiler command and compile flags.
#
# `sh sybopts.sh compile` echoes a platform-specific command to invoke the C 
# compiler. You can replace this invocation with the comand to invoke your 
# preferred compiler if you like.
#
COMPILE = `sh sybopts.sh compile` $(INCLUDE) $(DEFS)

#
# Define the list of libraries that must be linked with in order to call 
# Client-Library and Bulk-Library routines.
#
# `sh sybopts.sh comlibs` echoes a platform-specific list of lower-layer 
# Sybase libraries to link with. These are required in order to link with 
# CS-Libary, Client-Library, Bulk-Library, or Server-Library.
# 'sh sybopts.sh' with argument 'ct', 'srv' or 'blk' respectively echo the 
# Client-Library, Server-Library and Bulk-Library.
#
# `sh sybopts.sh syslibs` echoes a platform-specific list of system libraries 
# that are required.
#
COMLIBS	= `$(SHELL) sybopts.sh comlibs`
CTLIBS	= `$(SHELL) sybopts.sh ct` 
DBLIB	= `$(SHELL) sybopts.sh dblib`
UNILIB	= `$(SHELL) sybopts.sh unilib`
BLKLIBS	= `$(SHELL) sybopts.sh blk`
SYSLIBS	= `$(SHELL) sybopts.sh syslibs`

##############################################################################
#
# The targets and how to build them.
#
##############################################################################

# dbfirst is a dblib application and thus not supported in threaded mode
# and only when dblibrary is installed. Therefore dbfirst must be made
# separately if all the above conditions are fulfilled.
all: ctfirst ex01ct ex02ct ex03ct ex04ct ex05ct ex06ct ex08ct ex09ct ex10ct ex11ct testctrb

dbfirst: dbfirst.c
	@ printf "$(COMPILE) dbfirst.c $(LIBPATH) $(DBLIB) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) dbfirst.c $(LIBPATH) $(DBLIB) $(SYSLIBS) -o $@

ctfirst: ctfirst.c dbtoctex.h
	@ printf "$(COMPILE) ctfirst.c $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ctfirst.c $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex01ct: ex01ct.c exutils.o dbtoctex.h
	@ printf "$(COMPILE) ex01ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex01ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex02ct: ex02ct.c exutils.o dbctutil.o dbtoctex.h
	@ printf "$(COMPILE) ex02ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex02ct.c exutils.o dbctutil.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex03ct: ex03ct.c exutils.o dbtoctex.h
	@ printf "$(COMPILE) ex03ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex03ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex04ct: ex04ct.c exutils.o ctrowbuf.o dbctutil.o dbtoctex.h
	@ printf "$(COMPILE) ex04ct.c exutils.o ctrowbuf.o dbctutil.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex04ct.c ctrowbuf.o dbctutil.o exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex05ct: ex05ct.c dbctutil.o dbtoctex.h
	@ printf "$(COMPILE) ex05ct.c dbctutil.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex05ct.c dbctutil.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

#
# Special define for ex06ct:
# define SIDESHOW (-DSIDESHOW on the COMPILE line) to enable the 
# additional cursor functionality illustrated by ex06ct. Don't 
# define SIDESHOW if you want the same output as the DB-Library 
# example6.c
#
ex06ct: ex06ct.c exutils.o dbtoctex.h
	@ printf "$(COMPILE) -DSIDESHOW ex06ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) -DSIDESHOW ex06ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex08ct: ex08ct.c exutils.o dbctutil.o dbtoctex.h
	@ printf "$(COMPILE) ex08ct.c exutils.o dbctutil.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex08ct.c dbctutil.o exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex09ct: ex09ct.c exutils.o txtimg.o dbtoctex.h
	@ printf "$(COMPILE) ex09ct.c exutils.o txtimg.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex09ct.c exutils.o txtimg.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex10ct: ex10ct.c exutils.o txtimg.o dbtoctex.h
	@ printf "$(COMPILE) ex10ct.c exutils.o txtimg.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex10ct.c exutils.o txtimg.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

ex11ct: ex11ct.c exutils.o dbtoctex.h
	@ printf "$(COMPILE) ex11ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) ex11ct.c exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

# testctrb is a interactive test for the ctrowbuf.c routines.
testctrb: testctrb.c ctrowbuf.o exutils.o dbctutil.o dbtoctex.h
	@ printf "$(COMPILE) testctrb.c exutils.o ctrowbuf.o dbctutil.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@\n\n";
	@ $(COMPILE) testctrb.c ctrowbuf.o dbctutil.o exutils.o $(LIBPATH) $(CTLIBS) $(COMLIBS) $(SYSLIBS) -o $@

exutils.o: $(EXUTILS_C) $(EXUTILS_H)
	@ printf "$(COMPILE) -c $(EXUTILS_C) -o exutils.o\n\n";
	@ $(COMPILE) -c $(EXUTILS_C) -o exutils.o

txtimg.o: txtimg.c txtimg.h
	@ printf "$(COMPILE) -c txtimg.c -o txtimg.o\n\n";
	@ $(COMPILE) -c txtimg.c -o txtimg.o

dbctutil.o: dbctutil.c dbctutil.h
	@ printf "$(COMPILE) -c dbctutil.c -o dbctutil.o\n\n";
	@ $(COMPILE) -c dbctutil.c -o dbctutil.o

ctrowbuf.o: ctrowbuf.c ctrowbuf.h
	@ printf "$(COMPILE) -c ctrowbuf.c -o ctrowbuf.o\n\n";
	@ $(COMPILE) -c ctrowbuf.c -o ctrowbuf.o

#
# Clean all binaries
#
clean: 
	rm -f	dbfirst ctfirst ex01ct ex02ct ex03ct ex04ct ex05ct \
		ex06ct ex08ct ex09ct ex10ct ex11ct testctrb *\.o
