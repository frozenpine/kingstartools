/*
** Copyright Notice and Disclaimer
** -------------------------------
**      (c) Copyright 2004-2012.
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
**	This example demonstrates using a scrollable, read-only cursor.  
**	It opens a cursor with a canned query.  It processes the results 
**	using the standard ct_results() while loop.  It binds the column 
**	values to program variables. It then fetches and displays the rows 
**	in the standard ct_scroll_fetch() loop. This example uses a 
**	scrollable cursor with arrays as program variables and uses array 
**	binding. The results are offered in an array filled by a single 
**	ct_scroll_fetch() call. 
**
**	Restrictions/caveats:
**	CS_CURSOR_ROWS setting must be greater then 1 to be meaningful and
**	have the benefits of row pre-fetching. Also, for efficiency purpose
**	it is advised to set the size of the array (in #rows) the same to the 
**	setting of CS_CURSOR_ROWS. Setting it larger than CS_CURSOR_ROWS will
**	waste memory.
**
**	This is the canned query:
**		select au_fname, au_lname, postalcode from authors
**
** Routines Used
** -------------
**	cs_ctx_alloc()
**	ct_init()
**	ct_config()
**	ct_callback()
**	ct_con_alloc()
**	ct_con_props()
**	ct_connect()
**	ct_cmd_alloc()
**	ct_cursor(CS_CURSOR_DECLARE)
**	ct_cursor(CS_CURSOR_ROWS)
**	ct_cursor(CS_CURSOR_OPEN)
**	ct_cursor(CS_CURSOR_CLOSE)
**	ct_send()
**	ct_results()
**	ct_res_info()
**	ct_describe()
**	ct_bind()
**	ct_scroll_fetch()
**	ct_cmd_drop()
**	ct_close()
**	ct_con_drop()
**	ct_exit()
**	ct_ctx_drop()
**
** Input
** -----
**	This example uses a hard-coded query of the authors table in the pubs2
**	database.  The query is defined by SELECT below
**
** Output
** ------
**	This example simply displays the rows fetched in the user defined
**	program arrays.
**	Note: exutils2.c shows arraybinding and fetch code (ex_scroll_fetch_2).
**
** Server Dependencies
** -------------------
**	This example requires ASE version 15.0 or higher with scrollable
**	cursor support.
**
** Server Tables
** -------------
**	This example relies on the pubs2 database and the authors table.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctpublic.h>
#include "example.h"
#include "exutils.h"

CS_CHAR *Ex_appname  = "csr_disp_scrollcurs2";
CS_CHAR *Ex_dbname   = "pubs2";
CS_CHAR *Ex_server   = EX_SERVER;
CS_CHAR *Ex_username = EX_USERNAME;
CS_CHAR *Ex_password = EX_PASSWORD;

#define SELECT	"select au_fname, au_lname, postalcode from authors"

CS_STATIC CS_RETCODE DoCursor(CS_CONNECTION *connection);
CS_RETCODE CS_PUBLIC ex_scroll_fetch_2(CS_COMMAND *cmd);

int
main(int argc, char *argv[])
{
	CS_CONTEXT	*context;
	CS_CONNECTION	*connection;
	CS_RETCODE	retcode;

	EX_SCREEN_INIT();
	
	fprintf(stdout, "Scrollable Cursor Example,\n");
	fprintf(stdout, "With multi-row fetch and array binding\n");
	fflush(stdout);

	retcode = ex_init(&context);
	if (retcode != CS_SUCCEED)
	{
		ex_panic("ex_init failed");
	}

	retcode = ex_connect(context, &connection, Ex_appname,
				Ex_username, Ex_password, Ex_server);

	if (retcode == CS_SUCCEED)
	{
		retcode = DoCursor(connection);
	}

	if (connection != NULL)
	{
		retcode = ex_con_cleanup(connection, retcode);
	}
	
	if (context != NULL)
	{
		retcode = ex_ctx_cleanup(context, retcode);
	}

	return (retcode == CS_SUCCEED) ? EX_EXIT_SUCCEED : EX_EXIT_FAIL;
}

CS_STATIC CS_RETCODE
DoCursor(CS_CONNECTION *connection)
{
	CS_RETCODE	retcode;
	CS_COMMAND	*cmd;	
	CS_INT		res_type;

        if ((retcode = ex_use_db(connection, Ex_dbname)) != CS_SUCCEED)
        {
                ex_error("DoCursor: ex_use_db(pubs2) failed");
                return retcode;
        }

        if ((retcode = ct_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
        {
                ex_error("DoCursor: ct_cmd_alloc() failed");
                return retcode;
        }

	/*
	** Scrollable cursor declare, using CS_SCROLL_INSENSITIVE.
	*/
	retcode = ct_cursor(cmd, CS_CURSOR_DECLARE, "cursor_a", CS_NULLTERM,
			SELECT, CS_NULLTERM, CS_SCROLL_INSENSITIVE);

	if (retcode != CS_SUCCEED)
	{
                ex_error("DoCursor: ct_cursor(declare) failed");
		return retcode;
	}

	/*
	** CS_CURSOR_ROW setting (prefetch), this should match the bind count
	** set prior to calling ct_bind(). We set CS_CURSOR_ROW to 5 rows here.
	** Note: total number of rows in pubs2...authors is 23.
	*/	
	retcode = ct_cursor(cmd, CS_CURSOR_ROWS, NULL, CS_UNUSED, NULL, 
			CS_UNUSED, (CS_INT)5);
	if (retcode != CS_SUCCEED)
	{
                ex_error("DoCursor: ct_cursor(currows) failed");
		return retcode;
	}

	retcode = ct_cursor(cmd, CS_CURSOR_OPEN, NULL, CS_UNUSED, NULL,
			CS_UNUSED, CS_UNUSED);
	if (retcode != CS_SUCCEED)
	{
                ex_error("DoCursor: ct_cursor() failed");
		return retcode;
	}

	if ((retcode = ct_send(cmd)) != CS_SUCCEED)
	{
                ex_error("DoCursor: ct_send() failed");
		return retcode;
	}

	while((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
		case CS_CMD_SUCCEED:
			break;

		case CS_CMD_DONE:
   			break;

		case CS_CMD_FAIL:
			ex_error("DoCursor: ct_results() returned CMD_FAIL");
			break;

		case CS_CURSOR_RESULT:
			retcode = ex_scroll_fetch_2(cmd);

			if (retcode != CS_SUCCEED)
			{
				if (retcode == CS_SCROLL_CURSOR_ENDS || 
					retcode == CS_CURSOR_BEFORE_FIRST ||
				    	retcode == CS_CURSOR_AFTER_LAST)
				{
					retcode = CS_SUCCEED;
				}
			}
			else
			{
				ex_error("DoCursor: ex_scroll_fetch_2() failed on CS_CURSOR_RESULT ");
			  	return retcode;
			}
			break;
 
		default:
			ex_error("DoCursor: ct_results() returned unexpected result type");
			return CS_FAIL;
		}
	}

	switch ((int)retcode)
	{
		case CS_SUCCEED:
		case CS_END_RESULTS:
			break;

		case CS_FAIL:
			ex_error("DoCursor: ct_results() failed");
			return retcode;

		default:
			ex_error("DoCursor: ct_results() returned unexpected result code");
			return retcode;
	}

	/*
	** cursor close and deallocate
	*/
	retcode = ct_cursor(cmd, CS_CURSOR_CLOSE, NULL, CS_UNUSED, NULL, CS_UNUSED, CS_DEALLOC);

	if (retcode != CS_SUCCEED)
	{
               ex_error("DoCursor: ct_cursor(close) failed");
		return retcode;
	}

	if ((retcode = ct_send(cmd)) != CS_SUCCEED)
	{
               ex_error("DoCursor: ct_send() for cursor_close failed");
		return retcode;
	}

	while((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
	{
		switch ((int)res_type)
		{
		    case CS_CMD_SUCCEED:
		    case CS_CMD_DONE:
			break;
 
		case CS_CMD_FAIL:
			ex_error("DoCursor: ct_results() cursor_close returned CMD_FAIL");
			break;

		default:
			ex_error("DoCursor: ct_results() cursor_close returned unexpected result type");
			return CS_FAIL;
		}
	}

	if (retcode != CS_END_RESULTS)
	{
		ex_error("DoCursor: cursor_close, ENDRESULTS ct_results() failed");
		return retcode;
	}

	if ((retcode = ct_cmd_drop(cmd)) != CS_SUCCEED)
	{
                ex_error("DoCursor: ct_cmd_drop() failed");
                return retcode;
	}

	return retcode;
}