#
# Copyright Notice and Disclaimer
# -------------------------------
#       (c) Copyright 2012. 
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
# 	This example program demonstrates the use of the Python iteration
#	protocol to bulk copy out rows of a table.
#      
# Inputs
# ------         
#    	None.
#      
# 

import sybpydb

data = [["first", 1], ["second", 2], ["third", 3], ["fourth", 4], ["fifth", 5]]

conn = sybpydb.connect(dsn="user=sa;bulklogin=true;chainxacts=off")
cur = conn.cursor()
cur.execute("if exists (select name from sysdatabases where name = \"mydb\") \
		drop database mydb")
cur.execute("create database mydb")
cur.execute("exec sp_dboption mydb, \"select into/bulkcopy\", true")
cur.execute("use mydb")
cur.execute("create table mytable (name char(10), id int)")
cur.executemany("insert into mytable values (?, ?)", data)
cur.close()


print("Name       Id")
print("----       --")
blkcpy = conn.blkcursor()
# Start the bulk copy operation
blkcpy.copy("mytable", direction="out")


# Bulk copy out the rows by iterating over the rows copied out.
# This usage is only for a bulk copy out operation.
for row in blkcpy:
    print("%s %d" % (row[0], row[1]))
blkcpy.done()


blkcpy.close()
conn.close()
