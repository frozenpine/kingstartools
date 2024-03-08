#
# Copyright Notice and Disclaimer
# -------------------------------
#       (c) Copyright 2013. 
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
# 	This example program demonstrates a bulk copy operation on a
#	table which has an identity column in the following scenarios:
#	- copying data without any properties set
#	- copying data with the 'Identity'  property set
#	- copying data with the 'IdStartNum' property set
#      
# Inputs
# ------         
#    	None.

import sybpydb

# Connect with the bulklogin property set. 
conn = sybpydb.connect(dsn="user=sa;bulklogin=true;chainxacts=off")
cur = conn.cursor()
cur.execute("if exists (select name from sysdatabases where name = \"mydb\") \
		drop database mydb")
cur.execute("create database mydb")
cur.execute("exec sp_dboption mydb, \"select into/bulkcopy\", true")
cur.execute("use mydb")
cur.execute("create table mytable (empid int identity, empname varchar(20))")
cur.close()

blk = conn.blkcursor()

# Start bulk copy in operation. Do not specify values for identity columns.
# Values will be generated by the server. 
blk.copy(name="mytable", direction="in")
blk.rowxfer(["Joanne"])
blk.rowxfer(["John"])
blk.done()

# Start bulk copy in operation, with the 'identity' property set. 
# Values for identity columns will have to be specified.
blk.copy(name="mytable", direction="in", properties="identity=on")
blk.rowxfer([11, "Maya"])
blk.rowxfer([12, "Uma"])
blk.done()

# Specify starting identity column value of 21 for the copy operation.
blk.copy(name="mytable", direction="in", properties="IdStartNum=21")
blk.rowxfer(["Max"])
blk.rowxfer(["Danny"])
blk.done()

# Copy the data out
print("Id    Name")
print("--    ----")
blk.copy(name="mytable", direction="out")
for row in blk:
    print("%d    %s" % (row[0], row[1]))
blk.done()

blk.close()
conn.close()