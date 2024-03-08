#!/bin/sh 
###########################################################################
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
# 	sybopts.sh -- Helper script for UNIX makefiles that build Open Client
#    	Embedded SQL/Cobol applications. This script uses the value of
#    	the SYBPLATFORM environment variable to determine what platform
#    	specific libraries must be linked with the application.
#
# Usage
# -----
#  	sybopts.sh <args>
#
#  	where <args> can be one or more of the following:
#
#     	compile -- Based on SYBPLATFORM, echoes the compiler command
#	and platform-specific compile flags. For example, if
#       SYBPLATFORM indicates a platform with support for 
#	multithreading, the compile flag "-D_REENTRANT" might be 
#	required.
#
#     	precomp -- Based on SYBPLATFORM, echoes the Cobol precompiler.
#
#     	comlibs -- Based on SYBPLATFORM, echoes the list of required
#	lower-layer Sybase libraries that must be linked with the
#	application.
#
#     	syslibs -- Based on SYBPLATFORM, echoes the list of required
#	system (i.e., non-Sybase) libraries that must be linked with the
#	application.
#
#     	ct, cobct -- Based on SYBPLATFORM, echoes the name of the
#	archive for Client-Library or Cobol-Library, 
#
#     	verify -- Check SYBPLATFORM setting, if ok, echoes the library
#	names that will be used. If not ok, returns non-zero. This
#	argument must appear by itself with no other arguments.
#
#     	verify_threaded -- Returns non-zero if SYBPLATFORM indicates
# 	a version of Client-Library without multi-threaded support.
#
#     	verify_krb -- Returns non-zero if SYBPLATFORM value is a platform
#       for which we currently do *not* support CyberSAFE Kerberos.  Note
#       that Kerberos is currently only supported on the following UNIX
#       SYBPLATFORMs: nthread_sun_svr4(64), nthread_hpux and nthread_rs6000.
#
# Example
# -------
# 	COMPILE = `sh sybopts.sh compile`
#
###########################################################################

SET_SYBPLATFORM_MSG="Unknown machine type. Please set SYBPLATFORM variable"
BAD_ARGUMENT_MSG="Unknown argument"
THREAD_SUPPORT_MSG="This target requires multi-threaded support."
KRB_SUPPORT_MSG="This target requires that CyberSAFE Kerberos be installed and running."
SYBPLATFORM_MSG="Check the value of your SYBPLATFORM environment variable."

#
# COBOL: Compiler name
# CTLIB, COBLIB and COMLIBS: Sybase libraries. These come
#       in multi-threaded and single-threaded versions.
# SYSLIBS: The list of system libraries that are required in order
#       to build a Sybase Open Client/Server application, in the order
#       in which they must be linked.
#

case "${SYBPLATFORM}" in
	hpia)		COBOL="cob -x";
			CTLIB="-lsybct"; COBLIB="-lsybcobct";
			COMLIBS="-lsybtcl -lsybcs -lsybcomn -lsybintl -lsybunic";
			SYSLIBS="-lcl -lm -ldld" ;;
	nthread_hpia)	COBOL="cob -t -x";
			CTLIB="-lsybct_r"; COBLIB="-lsybcobct_r";
			COMLIBS="-lsybtcl_r -lsybcs_r -lsybcomn_r -lsybintl_r -lsybunic";
			SYSLIBS="-lcl -lm -lpthread -lrt -ldld" ;;
	hpia64)		COBOL="cob -x";
			CTLIB="-lsybct64"; COBLIB="-lsybcobct64";
			COMLIBS="-lsybtcl64 -lsybcs64 -lsybcomn64 -lsybintl64 -lsybunic64";
			SYSLIBS="-lcl -lm -ldld" ;;
	nthread_hpia64)	COBOL="cob -t -x";
			CTLIB="-lsybct_r64"; COBLIB="-lsybcobct_r64";
			COMLIBS="-lsybtcl_r64 -lsybcs_r64 -lsybcomn_r64 -lsybintl_r64 -lsybunic64";
			SYSLIBS="-lcl -lm -lpthread -lrt -ldld" ;;
	hpux)		COBOL="cob -x";
			CTLIB="-lsybct"; COBLIB="-lsybcobct";
			COMLIBS="-lsybtcl -lsybcs -lsybcomn -lsybintl -lsybunic";
			SYSLIBS="-lcl -lm -lBSD -ldld" ;;
	nthread_hpux)   COBOL="cob -t -x";
			CTLIB="-lsybct_r"; COBLIB="-lsybcobct_r";
			COMLIBS="-lsybtcl_r -lsybcs_r -lsybcomn_r -lsybintl_r -lsybunic";
			SYSLIBS="-lcl -lm -lpthread -lrt -ldld" ;;
	hpux64)		COBOL="cob -x";
			CTLIB="-lsybct64"; COBLIB="-lsybcobct64";
			COMLIBS="-lsybtcl64 -lsybcs64 -lsybcomn64 -lsybintl64 -lsybunic64";
			SYSLIBS="-lcl -lm -ldld" ;;
	nthread_hpux64) COBOL="cob -x";
			CTLIB="-lsybct_r64"; COBLIB="-lsybcobct_r64";
			COMLIBS="-lsybtcl_r64 -lsybcs_r64 -lsybcomn_r64 -lsybintl_r64 -lsybunic64";
			SYSLIBS="-lcl -lm -lpthread -lrt -ldld" ;;
        linux)		COBOL="cob -x";
                        CTLIB="-lsybct"; COBLIB="-lsybcobct";
                        COMLIBS="-lsybtcl -lsybcs -lsybcomn -lsybintl -lsybunic";
                        SYSLIBS="-ldl -lnsl -lm" ;;
        nthread_linux)	COBOL="cob -t -x";
                        CTLIB="-lsybct_r"; COBLIB="-lsybcobct_r";
                        COMLIBS="-lsybtcl_r -lsybcs_r -lsybcomn_r -lsybintl_r -lsybunic";
                        SYSLIBS="-ldl -lpthread -lnsl -lm" ;;
        linux64)	COBOL="cob -x";
                        CTLIB="-lsybct64"; COBLIB="-lsybcobct64";
                        COMLIBS="-lsybtcl64 -lsybcs64 -lsybcomn64 -lsybintl64 -lsybunic64";
                        SYSLIBS="-ldl -lnsl -lm" ;;
        nthread_linux64)	
			COBOL="cob -t -x";
                        CTLIB="-lsybct_r64"; COBLIB="-lsybcobct_r64";
                        COMLIBS="-lsybtcl_r64 -lsybcs_r64 -lsybcomn_r64 -lsybintl_r64 -lsybunic64";
                        SYSLIBS="-ldl -lpthread -lnsl -lm" ;;
        ibmplinux64)	COBOL="cob -x";
                        CTLIB="-lsybct64"; COBLIB="-lsybcobct64";
                        COMLIBS="-lsybtcl64 -lsybcs64 -lsybcomn64 -lsybintl64 -lsybunic64";
                        SYSLIBS="-ldl -lnsl -lm" ;;
        nthread_ibmplinux64)	
			COBOL="cob -t -x";
                        CTLIB="-lsybct_r64"; COBLIB="-lsybcobct_r64";
                        COMLIBS="-lsybtcl_r64 -lsybcs_r64 -lsybcomn_r64 -lsybintl_r64 -lsybunic64";
                        SYSLIBS="-ldl -lpthread -lnsl -lm" ;;
	rs6000)		COBOL="cob -x";
			CTLIB="-lsybct"; COBLIB="-lsybcobct";
			COMLIBS="-lsybtcl -lsybcs -lsybcomn -lsybintl -lsybunic";
			SYSLIBS="-lm" ;;
	nthread_rs6000) COBOL="cob -x";
			CTLIB="-lsybct_r"; COBLIB="-lsybcobct_r";
			COMLIBS="-lsybtcl_r -lsybcs_r -lsybcomn_r -lsybintl_r -lsybunic";
			SYSLIBS="-lm" ;;
	rs600064)	COBOL="cob -x";
			CTLIB="-lsybct64"; COBLIB="-lsybcobct64";
			COMLIBS="-lsybtcl64 -lsybcs64 -lsybcomn64 -lsybintl64 -lsybunic64";
			SYSLIBS="-lm" ;;
	nthread_rs600064) COBOL="cob -x";
			CTLIB="-lsybct_r64"; COBLIB="-lsybcobct_r64";
			COMLIBS="-lsybtcl_r64 -lsybcs_r64 -lsybcomn_r64 -lsybintl_r64 -lsybunic64";
			SYSLIBS="-lm" ;;
	sun_svr4|sunx86)       
			COBOL="cob -x";
			CTLIB="-lsybct"; COBLIB="-lsybcobct";
			COMLIBS="-lsybtcl -lsybcs -lsybcomn -lsybintl -lsybunic";
			SYSLIBS="-lsocket -lnsl -ldl -lm" ;;
	nthread_sun_svr4|nthread_sunx86)
			COBOL="cob -x";
			CTLIB="-lsybct_r"; COBLIB="-lsybcobct_r";
			COMLIBS="-lsybtcl_r -lsybcs_r -lsybcomn_r -lsybintl_r -lsybunic";
			SYSLIBS="-lsocket -lnsl -ldl -lpthread -lthread -lm" ;;
	sun_svr464|sunx8664)     
			COBOL="cob -x";
			CTLIB="-lsybct64"; COBLIB="-lsybcobct64";
			COMLIBS="-lsybtcl64 -lsybcs64 -lsybcomn64 -lsybintl64 -lsybunic64";
			SYSLIBS="-lsocket -lnsl -ldl -lm" ;;
	nthread_sun_svr464|nthread_sunx8664)
			COBOL="cob -x";
			CTLIB="-lsybct_r64"; COBLIB="-lsybcobct_r64";
			COMLIBS="-lsybtcl_r64 -lsybcs_r64 -lsybcomn_r64 -lsybintl_r64 -lsybunic64";
			SYSLIBS="-lsocket -lnsl -ldl -lpthread -lthread -lm" ;;

	*)		echo " $SET_SYBPLATFORM_MSG"; exit 1 ;;
esac ;

#
# COBPRE: Cobol precompiler name
#
case "${SYBPLATFORM}" in
        nthread_*64)           
			PRECOMP="${SYBASE}/${SYBASE_OCS}/bin/cobpre_r64" ;;
        nthread_*)
			PRECOMP="${SYBASE}/${SYBASE_OCS}/bin/cobpre_r" ;;
	*64)	
			PRECOMP="${SYBASE}/${SYBASE_OCS}/bin/cobpre64" ;;
        *)
			PRECOMP="${SYBASE}/${SYBASE_OCS}/bin/cobpre" ;;
esac ;

#
# A single argument of "verify" means that we should print
# the value of SYBPLATFORM, print the choices that will be made
# for comlibs, syslibs, etc., then exit with a status of 0. If
# SYBPLATFORM is incorrect, we will not get here.
#
if [ $1 = "verify" ]
  then
	echo "SYBPLATFORM is $SYBPLATFORM"
	echo "Compiling with $COBOL"
	echo "Sybase base libraries: $COMLIBS"
	echo "Sybase Client library: $CTLIB"
	echo "System libraries: $SYSLIBS"
	exit 0
fi

if [ $1 = verify_threaded ]
  then
	case $SYBPLATFORM in
		nthread_*)
			exit 0;;
		*)
			echo " $THREAD_SUPPORT_MSG";
			exit 1 ;;
	esac
	exit 0
fi

#
# Ensure that Kerberos is installed and running.  (Note that Kerberos is
# currently only supported on nthread_sun_svr4(64), nthread_hpux and nthread_rs6000)
#
if [ $1 = verify_krb ]
  then
	case $SYBPLATFORM in
		nthread_sun_svr4*|nthread_hpux|nthread_rs6000)
			exit 0;;
		*)
			echo " $KRB_SUPPORT_MSG";
			exit 1 ;;
	esac
	exit 0
fi

#
# Build a string to echo what was asked for:
#
ECHOSTRING=""
for a
do
	case "$a" in
		compile) 	ECHOSTRING="$ECHOSTRING $COBOL" ;;
		precomp)	ECHOSTRING="$ECHOSTRING $PRECOMP" ;;
		comlibs) 	ECHOSTRING="$ECHOSTRING $COMLIBS" ;;
		syslibs) 	ECHOSTRING="$ECHOSTRING $SYSLIBS" ;;
		cobct)		ECHOSTRING="$ECHOSTRING $COBLIB" ;;
		ct) 		ECHOSTRING="$ECHOSTRING $CTLIB" ;;
		*) 		echo "$0: $a: $BAD_ARGUMENT_MSG "; exit 1;;
	esac
done

echo "$ECHOSTRING"
exit 0
