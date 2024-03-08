<?php
/*
** Copyright Notice and Disclaimer
** -------------------------------
**      (c) Copyright 2014.
**      SAP AG or an SAP affiliate company. All rights reserved.
**      Unpublished rights reserved under U.S. copyright laws.
**
**      SAP grants Licensee a non-exclusive license to use, reproduce,
**      modify, and distribute the sample source code below (the "Sample Code"),
**      subject to the following conditions:
**
**      (i) redistributions must retain the above copyright notice;
**
**      (ii) SAP shall have no obligation to correct errors or deliver
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
**      SAP AG or an SAP affiliate company OR ITS LICENSORS BE LIABLE FOR ANY DIRECT, 
**	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
**	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
**	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
**	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
**	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
**      USE OF THE SAMPLE CODE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
**      DAMAGE.
**
** Description
** -----------
**	This example is meant to be run from a command line.
**	It does not take any command line parameters and demonstrates
**	how to call an RPC command on a server, how to send input parameters
**	and how to process print statements and output parameter results
**	returned from the RPC command.
**
** Requirements
** ------------
**	An ASE with pubs2 installed, and a valid user account on it.
*/
require_once("sample_utils.php");

/*
** Global variable(s)
*/
$Sample_proc = "sample_php_rpc";


/*
** We set a default time zone and locale so we can use the PHP date/time
** manipulation functions to display a better readable date/time output.
*/
date_default_timezone_set ("UTC");
setlocale(LC_TIME, "en_US");

/*
** Helper function that creates the stored procedure.
*/
function
create_stored_proc($conn)
{
	global $Sample_proc;

	$resultset = smpl_drop_obj($conn, $Sample_proc, "proc");
	if ($resultset !== TRUE)
	{
		return FALSE;
	}

	$thisquery =
		"create proc $Sample_proc ( " .
			"@intparam int, " .
			"@sintparam smallint output, " .
			"@floatparam float output, " .
			"@decimalparam decimal(5,2) output, " .
			"@moneyparam money output, " .
			"@dateparam datetime output, " .
			"@charparam char(60) output, " .
			"@binaryparam binary(20) output ) " .
		"as " .
		"begin " .
			"select @intparam, @sintparam, @floatparam, " .
			"@moneyparam, @dateparam, @charparam, @binaryparam " .
			"select @sintparam = @intparam * 2 " .
			"select @floatparam = @intparam * 3 " .
			"select @decimalparam = 123.45 " .
			"select @moneyparam = convert(money, @intparam) " .
			"select @dateparam = convert(datetime, 'Oct 12 1992 12:34') " .
			"select @charparam = \"String set in stored procedure char(60) output parameter.\" " .
			"select @binaryparam = convert(binary, @intparam * 4) " .
			"print \"This is the message printed out by the stored procedure.\"" . 
		"end ";

	$resultset = sybase_query($thisquery);
	if ($resultset !== TRUE)
	{
		return FALSE;
	}

	return TRUE;
}


/*
** The call back function that prints out messages returned from the server.
*/
function
rpc_msg_callback($message_number, $severity, $description)
{
	if ($severity == "DBC_SV_INFORM")
	{
		/*
		** We'll ignore the 'Changed database' messages.
		*/
		if ($message_number != 5701)
		{
			/*
			** Print the informative message.
			*/
			print("$description\n");
		
		}

		return TRUE;
	}

	/*
	** Print the full details of the error message.
	*/
	print("message nbr.\t= $message_number\n");
	print("severity\t= $severity\n");
	print("description\t= $description\n");

	return TRUE;
}


/*
** main
**	This is the entry point of this PHP script.
**
** Switch PHP error and warning messages information on.
*/
ini_set('display_errors', 1);
error_reporting(E_ALL|E_STRICT);

/*
** Set the sample global variable defaults for
** $Dsquery, $Dbname, $Username, $Passwd, $Appname 
*/
smpl_set_global_dflts();

/*
** Connect to the server.
*/
$conn = sybase_pconnect($Dsquery, $Username, $Passwd);
if ($conn == FALSE)
{
	die("$Appname: Failed to connect to $Dsquery.\n");
}

/*
** Set the ASE PHP driver message handler.
*/
$retval = sybase_set_message_handler("rpc_msg_callback", 1, $conn);
if ($retval == FALSE)
{
	die("$Appname: Failed to register message callback.\n");
}

/*
** Use the default database.
*/
if (sybase_select_db($Dbname) !== TRUE)
{
	die("$Appname: Failed to use database $Dbname.\n");
}

/*
** Create the stored procedure.
*/
if (create_stored_proc($conn) != TRUE)
{
	die("$Appname: Failed to create the stored procedure.\n");
}

/*
** Initialize the RPC
*/
$smpl_sp_stmt = sybase_rpc_init($conn, "$Dbname..$Sample_proc");
if ($smpl_sp_stmt === FALSE)
{
	/*
	** Remove the stored procedure from the server.
	*/
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: Failed to initialize the RPC\n");
}


/*
** Set a value for the input parameter.
*/
$a_int = 3;

/*
** Set precision and scale for the numeric output parameter.
*/
$a_numeric = "5.2";

/*
** Bind the stored procedure parameters.
*/
$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 0, &$a_int, "DT_INT",
		FALSE, SASE_D_INPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for int input " .
			"parameter #1 failed\n");
}

$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 1, &$a_sint, "DT_SMALLINT",
		FALSE, SASE_D_OUTPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for smallint output " .
			"parameter #2 failed\n");
}

$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 2, &$a_float, "DT_FLOAT",
		FALSE, SASE_D_OUTPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for float output " .
			"parameter #3 failed\n");
}

$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 3, &$a_numeric, "DT_DECIMAL",
		FALSE, SASE_D_OUTPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for decimal output " .
			"parameter #4 failed\n");
}

$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 4, &$a_money, "DT_MONEY",
		FALSE, SASE_D_OUTPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for money output " .
			"parameter #5 failed\n");
}

$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 5, &$a_date, "DT_BIN_TIMESTAMP",
		FALSE, SASE_D_OUTPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for date output " .
			"parameter #6 failed\n");
}

$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 6, &$a_char, "DT_STRING",
		FALSE, SASE_D_OUTPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for char output " .
			"parameter #7 failed\n");
}

$smpl_bool = sybase_rpc_bind_param_ex($smpl_sp_stmt, 7, &$a_binary, "DT_BINARY",
		FALSE, SASE_D_OUTPUT);
if ($smpl_bool !== TRUE)
{
	smpl_drop_obj($conn, $Sample_proc, "proc");
	die("$Appname: sybase_rpc_bind_param_ex for binary output " .
			"parameter #8 failed\n");
}

/*
** Execute the stored procedure.
*/
$resultset = sybase_rpc_execute($smpl_sp_stmt);

/*
** Loop through the results and 'fetch' them in order to let the ASE PHP driver
** pick up the output parameter values.
** We don't expect any row results, so here is no further action required.
*/
while ($resultset != FALSE)
{
	
	while ($row = sybase_fetch_assoc($resultset))
		;

	/*
	** Clean up the resultset, we're done with it.
	*/
	sybase_free_result($resultset);

	$resultset = sybase_next_result($conn);
}


/*
** Remove the stored procedure from the server.
*/
smpl_drop_obj($conn, $Sample_proc, "proc");

/*
** Close the connection to the database.
*/
if ($conn != NULL)
{
	$closed = sybase_close($conn);
	if ($closed != TRUE)
	{
		die("$Appname: Failed to close the connection to $Dsquery.\n");
	}
}

/*
** Print the contents of the output parameters.
*/
print("\nValues of the stored procedure output parameters of type:\n");
print("smallint:\t$a_sint\n");
print("float:\t\t$a_float\n");
print("numeric(5,2):\t$a_numeric\n");
print("money:\t\t$a_money\n");
print("char(60):\t$a_char\n");

/*
** The rtrim() call will make sure the bin2hex() function stops parsing
** at the actual end of the string instead of continuing for the full
** string length.
** We'll also prepend '0x' to the output to indicate that
** we're displaying hexadecimal.
*/
print("binary(20):\t0x" . bin2hex(rtrim($a_binary)). "\n");

/*
** Create and print a custom formatted date/time string from the ISO-8601
** string we received in the $a_date output parameter.
** On Windows use the date formating directive '%#d' as '%e'
** is not supported on that platform.
*/
$date_fmt_str = "%A " .
	((strtolower(substr(PHP_OS, 0, 3)) == "win") ? "%#d" : "%e") .
	" %B %Y, %H:%M:%S %p";
$locdatetime = new DateTime($a_date);
print("date, time:\t" . strftime($date_fmt_str, $locdatetime->getTimestamp()) .
	"\n");

?>
