# 
# Copyright Notice and Disclaimer
# -------------------------------
#       (c) Copyright 2011-2012. 
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
#    	This example demonstrates connecting to a server.
#    	It can be used as an aid in checking a successful installation.
#    	If it runs to completion, then the Sybase Python extension module
#    	was found and successfully loaded. And it also verifies that a 
#    	connection to the database can be established. 
#      
# Inputs
# ------         
#    	None.
#      
# Server Tables
# -------------  
#    	None.
# 

import sybpydb

conn = sybpydb.connect(user='sa', password='')
print("Successfully established connection, exiting.")
conn.close()
