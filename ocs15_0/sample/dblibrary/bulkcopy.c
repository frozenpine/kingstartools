/*
** Copyright Notice and Disclaimer
** -------------------------------
**      (c) Copyright 1992-2012.
**      Sybase, Inc. All rights reserved.
**      Unpublished rights reserved under U.S. copyright laws.
**      This material is the confidential and trade secret information of
**      Sybase, Inc.
**
**      Sybase grants Licensee a non-exclusive license to use, reproduce,
**      modify, and distribute the sample source code below (the "Sample Code"),
**      subject to the following conditions:
**
**      (i) redistributions must retain the above copyright notice;
**
**      (ii) Sybase shall have no obligation to correct errors or deliver
**      updates to the Sample Code or provide any other support for the
**      Sample Code;
**
**      (iii) Licensee may use the Sample Code to develop applications
**      (the "Licensee Applications") and may distribute the Sample Code in
**      whole or in part as part of such Licensee Applications, however in no
**      event shall Licensee distribute the Sample Code on a standalone basis;
**
**      (iv) and subject to the following disclaimer:
**      THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
**      INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
**      AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
**      SYBASE, INC. OR ITS LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
**      INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
**      BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
**      OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
**      ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
**      TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
**      USE OF THE SAMPLE CODE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
**      DAMAGE.
**
** Description
** -----------
** 	This example uses the bulk copy (bcp) routines to copy the
** 	data from a host file into a newly-created table containing several
** 	ASE datatypes.
**
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sybfront.h>
#include <sybdb.h>
#include "sybdbex.h"

#define BUFLEN      2048
#define PLEN        25 

/*
** Forward declarations
*/
CS_VOID		  doexit PROTOTYPE((int));

int
main(int argc, char *argv[])
{

	LOGINREC        *login;
	DBPROCESS       *dbproc;

	/* Variables for host file data to be copied to database. */
	DBCHAR           age[PLEN+1];
	DBCHAR           userid[PLEN+1];
	DBCHAR           royalty[PLEN+1];
	DBCHAR           name[PLEN+1];
	DBCHAR           title_id[PLEN+1];
	DBCHAR           us_citizen[PLEN+1];
	DBCHAR           account[PLEN+1];
	DBCHAR           title[PLEN+1];
	DBCHAR           manager[PLEN+1];
	FILE             *infile;

	printf("Demo of SQL queries in a command batch\n\n");
	fflush(stdout);

	/* Initialize DB-Library. */
	if (dbinit() == FAIL)
		exit(ERREXIT);
	dbsetversion(DBVERSION_100);

	/*
	** Install the user-supplied error-handling and message-handling
	** routines. They are defined at the bottom of this source file.
	*/
	dberrhandle((EHANDLEFUNC)err_handler);
	dbmsghandle((MHANDLEFUNC)msg_handler);
	
	/*
	** Allocate and initialize the LOGINREC structure to be used
	** to open a connection to ASE.
	*/
	login = dblogin();
	DBSETLUSER(login, USER);
	DBSETLPWD(login, PASSWORD);
	DBSETLAPP(login, "example_bcp");

	/* Enable bulk copy for this connection. */
	BCP_SETL(login, TRUE);
	
	/*
	** Get a connection to the database.
	*/
	if ((dbproc = dbopen(login, NULL)) == (DBPROCESS *) NULL)
	{
		fprintf(stderr, "Can't connect to server.\n");
		doexit(ERREXIT);
	}

	/*
	** Create the database and turn on the option to allow bulkcopy.
	*/
	printf("Creating the 'example_bcp' database.\n");

	/* Create the database. */
	dbcmd(dbproc,"create database example_bcp");
	dbsqlexec(dbproc);
	while (dbresults(dbproc) != NO_MORE_RESULTS)
	{
		continue;
	}

	/*
	** Create a table that contains several ASE datatypes.
	*/
	printf("Creating the 'alltypes' table.\n");
	dbuse(dbproc, "example_bcp");
	dbcmd(dbproc,"create table alltypes ");
	dbcmd(dbproc,"(age tinyint,");
	dbcmd(dbproc,"userid smallint,");
	dbcmd(dbproc,"royalty int,");
	dbcmd(dbproc,"name char(25),");
	dbcmd(dbproc,"title_id varbinary(20),"); 
	dbcmd(dbproc,"us_citizen bit,");
	dbcmd(dbproc,"account float,");
	dbcmd(dbproc,"title varchar(20),");
	dbcmd(dbproc,"manager char(25))");

	dbsqlexec(dbproc);
	while (dbresults(dbproc) != NO_MORE_RESULTS)
	{
		continue;
	}

	/* Turn on option to allow bulk copy */
	dbuse(dbproc, "master");
	dbcmd(dbproc, "execute sp_dboption example_bcp, 'bulk', true");
	dbsqlexec(dbproc);
	while (dbresults(dbproc) != NO_MORE_RESULTS)
	{
		continue;
	}

	dbuse(dbproc, "example_bcp");
	dbcmd(dbproc, "checkpoint");
	dbsqlexec(dbproc);
	while (dbresults(dbproc) != NO_MORE_RESULTS)
	{
		continue;
	}

	/*
	** Insert rows of data into the newly created table "alltypes".
	** We will use the bulk copy routines to read in the data from
	** a host file.
	*/

	/*
	** We first need to initialize the bulk copy routines by
	** naming the table and host file to be copied. Although
	** we could use have the data copied directly from the
	** host file into the database table, that's not very
	** interesting for demonstration purposes.  Instead we'll
	** read the data from the host file ourselves and then
	** use the bulk copy routines to take the data from our
	** program variables.
	*/
	if (bcp_init(dbproc, "example_bcp..alltypes", NULL,
		"bcp.errors", DB_IN) != SUCCEED)
			doexit(ERREXIT);

	/*
	** We now tell bcp which program variables will contain the
	** data to be copied in.  For each column, we give the variable
	** address, its program datatype, its column position,
	** length information, and terminator information.
	*/

	/*
	** args: dbproc; program variable address; prefix length, if any;
	**	 program variable length; terminator, if any; terminator length;
	**	 program variable type; column number
	**
	** Because we're going to read all the data into program variables
	** that are character arrays, we can have the bcp bind routine
	** automatically do the conversion to the correct table column
	** datatype.
	*/
	bcp_bind(dbproc, (BYTE DBFAR *)age, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 1);
	bcp_bind(dbproc, (BYTE DBFAR  *)userid, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 2);
	bcp_bind(dbproc, (BYTE DBFAR *)royalty, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 3);
	bcp_bind(dbproc, (BYTE DBFAR *)name, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 4);
	bcp_bind(dbproc, (BYTE DBFAR *)title_id, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 5);
	bcp_bind(dbproc, (BYTE DBFAR *)us_citizen, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 6);
	bcp_bind(dbproc, (BYTE DBFAR *)account, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 7);
	bcp_bind(dbproc, (BYTE DBFAR *)title, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 8);
	bcp_bind(dbproc, (BYTE DBFAR *)manager, 0, -1, (BYTE DBFAR *)"", 1, 
		 SYBCHAR, 9);

	/*
	** Now we'll open the data file, read the data into our program
	** variables. As we read in each row, we'll have bcp move it
	** into the database table.
	*/
	if ((infile=fopen("bcpdata.in","r")) == NULL)
	{
		fprintf(stderr, "Unable to open file 'bcpdata.in'.\n");
		doexit(ERREXIT);
	}

	/* Read the data in from the host file. */
	fprintf(stdout, "Moving data from host file to database.\n");
	while (fscanf(infile, "%s %s %s %s %s %s %s %s %s",
		age, userid, royalty, name, title_id, us_citizen,
		account, title, manager) != EOF)

			/* Bulk copy it into the database */
			bcp_sendrow(dbproc);

	/*
	** Close the bulk copy process so all the changes are committed
	** to the database.
	*/
	bcp_done(dbproc);

	/* 
	** Drop the database. 
	*/
	printf("Dropping the 'example_bcp' database.\n");
	dbuse(dbproc, "master");
	dbcmd(dbproc,"drop database example_bcp");
	dbsqlexec(dbproc);
	while (dbresults(dbproc) != NO_MORE_RESULTS)
	{
		continue;
	}

	doexit(STDEXIT);
	return (0);
}


CS_VOID
doexit(int value)
{
	dbexit();            /* always call dbexit before returning to OS */
	exit(value);
}


int CS_PUBLIC
err_handler(DBPROCESS *dbproc, int severity, int dberr, int oserr, 
	    char *dberrstr, char *oserrstr)
{
	if ((dbproc == NULL) || (DBDEAD(dbproc)))
	{
		return (INT_EXIT);
	}
	else 
	{
		printf("DB-Library error:\n\t%s\n", dberrstr);

		if (oserr != DBNOERR)
		{
			printf("Operating-system error:\n\t%s\n", oserrstr);
		}

		return (INT_CANCEL);
	}
}

int CS_PUBLIC 
msg_handler(DBPROCESS *dbproc, DBINT msgno, int msgstate, int severity, char *msgtext, 
	    char *srvname, char *procname, int line)
{
	/*
	** If it's a database change message, we'll ignore it.
	** Also ignore language change message.
	*/
	if (msgno == 5701 || msgno == 5703)
	{
		return (0);
	}

	printf("Msg %d, Level %d, State %d\n", 
	        msgno, severity, msgstate);

	if (strlen(srvname) > 0)
	{
		printf("Server '%s', ", srvname);
	}

	if (strlen(procname) > 0)
	{
		printf("Procedure '%s', ", procname);
	}

	if (line > 0)
	{
		printf("Line %d", line);
	}

	printf("\n\t%s\n", msgtext);
	return (0);
}
