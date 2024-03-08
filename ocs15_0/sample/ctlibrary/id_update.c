/*
** Copyright Notice and Disclaimer
** -------------------------------
**      (c) Copyright 2003-2012.
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
**    This is the example program that demonstrates the use of
**    'identity_update' option.
**    It connects to a server, creates 'test_table' in the 'pubs2'
**    database, inserts a couple of rows with an identity field,
**    then turns on the 'identity_update' option and updates the
**    identity in one of the rows with a user-specified value.
**
** Inputs
** ------
**    None.
**
** Server Tables
** -------------
**    This program requires the pubs2 database.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctpublic.h>
#include "example.h"

#define MAXCOLUMNS   2
#define MAXSTRING    255
#define ERR_CH stderr
#define OUT_CH stdout

/*
** Username and password are defined in example.h
*/
CS_CHAR *Ex_username = EX_USERNAME;
CS_CHAR *Ex_password = EX_PASSWORD;

/*
** Define a macro that exits if a function return code indicates
** failure.
*/
#define EXIT_ON_FAIL(context, ret, str) \
  if (ret != CS_SUCCEED) \
  { \
    fprintf(ERR_CH, "Fatal error: %s\n", str); \
    if (context != NULL) \
    { \
      (CS_VOID) ct_exit(context, CS_FORCE_EXIT); \
      (CS_VOID) cs_ctx_drop(context); \
    } \
    exit(EX_EXIT_FAIL); \
  }

/*
** Callback routines for library errors and server messages.
*/
CS_RETCODE CS_PUBLIC csmsg_callback(CS_CONTEXT *context,
		CS_CLIENTMSG *clientmsg);
CS_RETCODE CS_PUBLIC clientmsg_callback(CS_CONTEXT *context,
	CS_CONNECTION *connection, CS_CLIENTMSG *clientmsg);
CS_RETCODE CS_PUBLIC servermsg_callback(CS_CONTEXT *context,
	CS_CONNECTION *connection, CS_SERVERMSG *servermsg);

/*
** Sample values for inserting into char field.
*/
CS_CHAR *names[4] = {"Alice", "Brenda", "Claire", NULL};

/*
** Declare our own helper routine to process the results.
*/
int process_results(CS_CONTEXT *context, CS_COMMAND *cmd);


/*
** Main entry point for the program.
*/
int 
main(int argc, char *argv[])
{
	CS_CONTEXT	*context;	/* Context structure     */
	CS_CONNECTION	*connection;	/* Connection structure. */
	CS_COMMAND	*cmd;	/* Command structure.    */

	CS_RETCODE	ret;
	CS_CHAR		cmdstring[MAXSTRING];
	CS_INT		i;

	/*
	** Allocate a context structure.
	*/
	context = NULL;
	ret = cs_ctx_alloc(EX_CTLIB_VERSION, &context);
	EXIT_ON_FAIL(context, ret, "cs_ctx_alloc failed");

	/*
	** Initialize Client-Library.
	*/
	ret = ct_init(context, EX_CTLIB_VERSION);
	EXIT_ON_FAIL(context, ret, "ct_init failed");

	/*
	** Install a callback function to handle CS-Library errors.
	*/
	ret = cs_config(context, CS_SET, CS_MESSAGE_CB,
			(CS_VOID *)csmsg_callback,
			CS_UNUSED, NULL);
	EXIT_ON_FAIL(context, ret,
		     "cs_config(CS_MESSAGE_CB) failed");

	/*
	** Install a callback function to handle Client-Library errors.
	**
	** The client message callback receives error or informational
	** messages discovered by Client-Library.
	*/
	ret = ct_callback(context, NULL, CS_SET, CS_CLIENTMSG_CB,
			  (CS_VOID *)clientmsg_callback);
	EXIT_ON_FAIL(context, ret,
		     "ct_callback for client messages failed");

	/*
	** The server message callback receives server messages sent by
	** the server. These are error or inforamational messages.
	*/
	ret = ct_callback(context, NULL, CS_SET, CS_SERVERMSG_CB,
			  (CS_VOID *)servermsg_callback);
	EXIT_ON_FAIL(context, ret,
		     "ct_callback for server messages failed");


	/*
	** Allocate a connection structure.
	*/
	ret = ct_con_alloc(context, &connection);
	EXIT_ON_FAIL(context, ret, "ct_con_alloc() failed");

	/*
	** These two calls set the user credentials (username and
	** password) for opening the connection.
	*/
	ret = ct_con_props(connection, CS_SET, CS_USERNAME,
			   Ex_username, CS_NULLTERM, NULL);
	EXIT_ON_FAIL(context, ret, "Could not set user name");
	ret = ct_con_props(connection, CS_SET, CS_PASSWORD,
			   Ex_password, CS_NULLTERM, NULL);
	EXIT_ON_FAIL(context, ret, "Could not set password");

	/*
	** Create the connection.
	*/
	if (EX_SERVER == NULL)
	{
		ret = ct_connect(connection, NULL, 0);
	}
	else
	{
		ret = ct_connect(connection, EX_SERVER, CS_NULLTERM);
	}
	EXIT_ON_FAIL(context, ret, "Could not connect!");


	/*
	** Allocate a command structure.
	*/
	ret = ct_cmd_alloc(connection, &cmd);
	EXIT_ON_FAIL(context, ret, "ct_cmd_alloc() failed");

	/*
	** Use 'pubs2' database. 
	*/
	sprintf(cmdstring, "%s", "use pubs2");
	fprintf(OUT_CH, "%s\n", cmdstring);
	ret = ct_command(cmd, CS_LANG_CMD, cmdstring, CS_NULLTERM, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_command() failed");

	ret = ct_send(cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	ret = process_results(context, cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	/*
	** Create 'test_table' in 'pubs2' database. 
	*/
	sprintf(cmdstring, "%s", "create table test_table(col1 numeric(10,0) identity, col2 char(10))");
	fprintf(OUT_CH, "%s\n\n", cmdstring);
	ret = ct_command(cmd, CS_LANG_CMD, cmdstring, CS_NULLTERM, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_command() failed");

	ret = ct_send(cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");
	ret = process_results(context, cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	/*
	** Insert 3 rows in the table. 
	*/
	i = 0;
	while(names[i] != NULL)
	{
		sprintf(cmdstring, "%s (\"%s\")", "insert into test_table values", names[i++]);
		ret = ct_command(cmd, CS_LANG_CMD, cmdstring, CS_NULLTERM,
				 CS_UNUSED);
		EXIT_ON_FAIL(context, ret, "ct_command() failed");

		ret = ct_send(cmd);
		EXIT_ON_FAIL(context, ret, "ct_send() failed");

		ret = process_results(context, cmd);
		EXIT_ON_FAIL(context, ret, "ct_send() failed");
	}

	/*
	** Do a 'select' on the table to display the data. 
	*/
	sprintf(cmdstring, "%s", "select * from test_table");
	fprintf(OUT_CH, "%s\n", cmdstring);
	ret = ct_command(cmd, CS_LANG_CMD, cmdstring, CS_NULLTERM, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_command() failed");

	ret = ct_send(cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	ret = process_results(context, cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	/*
	** Set the CS_OPT_IDENTITYUPD_ON and 
	** update the identity field in one of the rows. 
	*/
	sprintf(cmdstring, "%s", "Setting CS_OPT_IDENTITYUPD_ON ...");
	fprintf(OUT_CH, "\n%s\n\n", cmdstring);
	ret = ct_options(connection, CS_SET, CS_OPT_IDENTITYUPD_ON, "test_table", CS_NULLTERM, NULL);
	EXIT_ON_FAIL(context, ret, "ct_command() failed");

	sprintf(cmdstring, "%s", "update test_table set col1=99 where col2='Brenda'");
	fprintf(OUT_CH, "%s\n\n", cmdstring);
	ret = ct_command(cmd, CS_LANG_CMD, cmdstring, CS_NULLTERM, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_command() failed");

	ret = ct_send(cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	ret = process_results(context, cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	/*
	** Display the updated table. 
	*/
	sprintf(cmdstring, "%s", "select * from test_table");
	fprintf(OUT_CH, "%s\n", cmdstring);
	ret = ct_command(cmd, CS_LANG_CMD, cmdstring, CS_NULLTERM, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_command() failed");

	ret = ct_send(cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	ret = process_results(context, cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	fprintf(OUT_CH, "\n");

	/*
	** Drop the table.  
	*/
	sprintf(cmdstring, "%s", "drop table pubs2..test_table");
	ret = ct_command(cmd, CS_LANG_CMD, cmdstring, CS_NULLTERM, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_command() failed");

	ret = ct_send(cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");

	ret = process_results(context, cmd);
	EXIT_ON_FAIL(context, ret, "ct_send() failed");


	/*
	** Step 5:  Clean up and exit.
	*/

	/*
	** Drop the command structure.
	*/
	ret = ct_cmd_drop(cmd);
	EXIT_ON_FAIL(context, ret, "ct_cmd_drop failed");

	/*
	** Close the connection and drop its control structure.
	*/
	ret = ct_close(connection, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_close failed");
	ret = ct_con_drop(connection);
	EXIT_ON_FAIL(context, ret, "ct_con_drop failed");

	/*
	** ct_exit tells Client-Library that we are done.
	*/
	ret = ct_exit(context, CS_UNUSED);
	EXIT_ON_FAIL(context, ret, "ct_exit failed");

	/*
	** Drop the context structure.
	*/
	ret = cs_ctx_drop(context);
	EXIT_ON_FAIL(context, ret, "cs_ctx_drop failed");

	/*
	** Normal exit to the operating system.
	*/
	return EX_EXIT_SUCCEED;
}


/*
**
**  Just a helper routine which processes the results
**  of the commands which we send to the server.
**
*/
int process_results(CS_CONTEXT *context, CS_COMMAND *cmd)
{

	/* Data format structures for column descriptions: */
	CS_DATAFMT          columns[MAXCOLUMNS];

	CS_INT              datalength[MAXCOLUMNS];
	CS_SMALLINT         indicator[MAXCOLUMNS];
	CS_INT              count;
	CS_RETCODE          results_ret;
	CS_INT              result_type;
	CS_CHAR             name[MAXSTRING];
	CS_CHAR             city[MAXSTRING];
	CS_RETCODE          ret;


	while ((results_ret = ct_results(cmd, &result_type))
	       == CS_SUCCEED)
	{

		/*
		** ct_results sets result_type to indicate when data is
		** available and to indicate command status codes.
		*/
		switch ((int)result_type)
		{
		case CS_ROW_RESULT:

			/*
			** This result_type value indicates that the rows
			** returned by the query have arrived. We bind and
			** fetch the rows.
			** 
			** We're expecting exactly two character columns:
			** Column 1 is au_lname, 2 is au_city.
			** 
			** For each column, fill in the relevant fields in
			** the column's data format structure, and bind
			** the column.
			*/
			columns[0].datatype = CS_CHAR_TYPE;
			columns[0].format = CS_FMT_NULLTERM;
			columns[0].maxlength = MAXSTRING;
			columns[0].count = 1;
			columns[0].locale = NULL;
			ret = ct_bind(cmd, 1, &columns[0],
				      name, &datalength[0],
				      &indicator[0]);
			EXIT_ON_FAIL(context, ret,
				     "ct_bind() for au_lname failed");

			/*
			** Same thing for the 'city' column.
			*/
			columns[1].datatype = CS_CHAR_TYPE;
			columns[1].format = CS_FMT_NULLTERM;
			columns[1].maxlength = MAXSTRING;
			columns[1].count = 1;
			columns[1].locale = NULL;

			ret = ct_bind(cmd, 2, &columns[1], city,
				      &datalength[1],
				      &indicator[1]);
			EXIT_ON_FAIL(context, ret,
				     "ct_bind() for city failed");

			/*
			** Now fetch and print the rows.
			*/
			while (((ret = ct_fetch(cmd, CS_UNUSED, CS_UNUSED,
						CS_UNUSED, &count))
				== CS_SUCCEED)
			       || (ret == CS_ROW_FAIL))
			{

				/*
				** Check if we hit a recoverable error.
				*/
				if (ret == CS_ROW_FAIL)
				{
					fprintf(ERR_CH,
						"Error on row %ld.\n",
						(long)(count + 1));
				}

				/*
				** We have a row, let's print it.
				*/
				fprintf(OUT_CH, "%s: %s\n", name, city);
			}

			/*
			** We're finished processing rows, so check
			** ct_fetch's final return value to see if an
			** error occurred. The final return code should be
			** CS_END_DATA.
			*/
			if (ret == CS_END_DATA)
			{
				/* All done processing rows. */
			}
			else
				/* Failure occurred. */
			{
				EXIT_ON_FAIL(context, CS_FAIL,
					     "ct_fetch failed");
			}

			/*
			** All done with this result set.
			*/
			break;

		case CS_CMD_SUCCEED:

			/*
			** We executed a command that never returns rows.
			*/
			break;

		case CS_CMD_FAIL:

			/*
			** The server encountered an error while
			** processing our command. These errors will be
			** displayed by the server-message callback that
			** we installed earlier.
			*/
			break;

		case CS_CMD_DONE:

			/*
			** The logical command has been completely
			** processed.
			*/
			break;

		default:

			/*
			** We got something unexpected.
			*/
			EXIT_ON_FAIL(context, CS_FAIL,
			   "ct_results returned unexpected result type");
			break;
		}
	}

	/*
	** We've finished processing results. Check the return value of
	** ct_results() to see if everything went okay.
	*/
	switch ((int)results_ret)
	{
	case CS_END_RESULTS:

		/*
		** Everything went fine.
		*/
		break;

	case CS_FAIL:

		/*
		** Something terrible happened.
		*/
		EXIT_ON_FAIL(context, CS_FAIL,
			     "ct_results() returned CS_FAIL.");
		break;

	default:

		/*
		** We got an unexpected return value.
		*/
		EXIT_ON_FAIL(context, CS_FAIL,
			   "ct_results returned unexpected return code");
		break;
	}

	return CS_SUCCEED;
}


/*
** Handler for server messages. Client-Library will call this
** routine when it receives a message from the server.
*/
CS_RETCODE CS_PUBLIC
servermsg_callback(cp, chp, msgp)
CS_CONTEXT         *cp;
CS_CONNECTION      *chp;
CS_SERVERMSG       *msgp;
{

	/*
	** Print the message info.
	*/
	fprintf(ERR_CH,
		"Server message:\n\t");
	fprintf(ERR_CH,
		"number(%ld) severity(%ld) state(%ld) line(%ld)\n",
		(long)msgp->msgnumber, (long)msgp->severity,
		(long)msgp->state, (long)msgp->line);

	/*
	** Print the server name if one was supplied.
	*/
	if (msgp->svrnlen > 0)
		fprintf(ERR_CH, "\tServer name: %s\n", msgp->svrname);

	/*
	** Print the procedure name if one was supplied.
	*/
	if (msgp->proclen > 0)
		fprintf(ERR_CH, "\tProcedure name: %s\n", msgp->proc);

	/*
	** Print the null terminated message.
	*/
	fprintf(ERR_CH, "\t%s\n", msgp->text);

	/*
	** Server message callbacks must return CS_SUCCEED.
	*/
	return CS_SUCCEED;
}

/*
**  Client-Library error handler. This function will be invoked
**  when a Client-Library has detected an error. Before Client-
**  Library routines return CS_FAIL, this handler will be called
**  with additional error information.
*/
CS_RETCODE CS_PUBLIC
clientmsg_callback(context, conn, emsgp)
CS_CONTEXT         *context;
CS_CONNECTION      *conn;
CS_CLIENTMSG       *emsgp;
{

	/*
	** Error number: Print the error's severity, number, origin, and
	** layer. These four numbers uniquely identify the error.
	*/
	fprintf(ERR_CH,
		"Client Library error:\n\t");
	fprintf(ERR_CH,
		"severity(%ld) number(%ld) origin(%ld) layer(%ld)\n",
		(long)CS_SEVERITY(emsgp->severity),
		(long)CS_NUMBER(emsgp->msgnumber),
		(long)CS_ORIGIN(emsgp->msgnumber),
		(long)CS_LAYER(emsgp->msgnumber));

	/*
	** Error text: Print the error text.
	*/
	fprintf(ERR_CH, "\t%s\n", emsgp->msgstring);

	/*
	** Operating system error information: Some errors, such as
	** network errors, may have an operating system error associated
	** with them. If there was an operating system error, this code
	** prints the error message text.
	*/
	if (emsgp->osstringlen > 0)
	{
		fprintf(ERR_CH,
			"Operating system error number(%ld):\n",
			(long)emsgp->osnumber);
		fprintf(ERR_CH, "\t%s\n", emsgp->osstring);
	}

	/*
	** If we return CS_FAIL, Client-Library marks the connection as
	** dead. This means that it cannot be used anymore. If we return
	** CS_SUCCEED, the connection remains alive if it was not already
	** dead.
	*/
	return CS_SUCCEED;
}

/*
**  CS-Library error handler. This function will be invoked
**  when CS-Library has detected an error.
*/
CS_RETCODE CS_PUBLIC
csmsg_callback(context, emsgp)
CS_CONTEXT         *context;
CS_CLIENTMSG       *emsgp;
{

	/*
	** Print the error number and message.
	*/
	fprintf(ERR_CH,
		"CS-Library error:\n");
	fprintf(ERR_CH,
		"\tseverity(%ld) layer(%ld) origin(%ld) number(%ld)",
		(long)CS_SEVERITY(emsgp->msgnumber),
		(long)CS_LAYER(emsgp->msgnumber),
		(long)CS_ORIGIN(emsgp->msgnumber),
		(long)CS_NUMBER(emsgp->msgnumber));

	fprintf(ERR_CH, "\t%s\n", emsgp->msgstring);

	/*
	** Print any operating system error information.
	*/
	if (emsgp->osstringlen > 0)
	{
		fprintf(ERR_CH, "Operating System Error: %s\n",
			emsgp->osstring);
	}

	return CS_SUCCEED;
}
