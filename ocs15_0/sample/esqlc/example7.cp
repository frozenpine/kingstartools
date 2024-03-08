/*
** Copyright Notice and Disclaimer
** -------------------------------
**      (c) Copyright 2007-2012.
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
**	This example is an interactive query program that 
**	guides the user through a series of prompts to select
**	a title from the titles table in the pubs2 database.
**	It uses cursors extensively to guide the query.
**
**	This example is the same as example1, but uses a
**	different form of indicator handling in the INTO
**	clause. It uses structs as host variables and struct(s)
**	of shorts as indicator(s). See function show_book()
**	as the prime example.
*/

#include <stdio.h>
#include "sybsqlex.h"

/* Declare the SQLCA. */
EXEC SQL INCLUDE SQLCA;

/* 
** These tokens must be declared in a declare section
** because they are used in declare sections below.
*/
EXEC SQL BEGIN DECLARE SECTION;
#define TYPESIZE 	13
#define	TIDSIZE 	6
EXEC SQL END DECLARE SECTION;

#define  EOLN	'\0'

/* 
** Forward declarations of the error and message handlers and
** other subroutines called from main().
*/
void	error_handler();
void	warning_handler();
int	get_type();
int	get_titleid();
void	show_book();

#define ISWORDSPACE(c) (c == ' ' || c == '\t')

int
main(int argc, char *argv[])
{
	EXEC SQL BEGIN DECLARE SECTION;
	/* storage for login name and password. */
	char	username[30];
	char	password[30];
	EXEC SQL END DECLARE SECTION;

	char	u_type[TYPESIZE+1];
	char	u_titleid[TIDSIZE+1];

	EXEC SQL WHENEVER SQLERROR CALL error_handler();
	EXEC SQL WHENEVER SQLWARNING CALL warning_handler();
	EXEC SQL WHENEVER NOT FOUND CONTINUE;

	/* 
	** Copy the user name and password defined in sybsqlex.h to
	** the variables declared for them in the declare section. 
	*/
	strcpy(username, USER);
	strcpy(password, PASSWORD);

	EXEC SQL CONNECT :username IDENTIFIED BY :password;

	EXEC SQL USE pubs2;

	/*
	** This loop controls the query.
	*/
	while (get_type(u_type))
	{
		while (get_titleid(u_titleid, u_type))
		{
			show_book(u_titleid);
		}
	}

	EXEC SQL DISCONNECT DEFAULT;

	return(STDEXIT);
}

/*
** int get_type(type) 
**
**	Displays a list of book types from the titles
**	table and lets the user enter one. 
*/
int 
get_type(char *type)
{
	/* 
	** Declare a character string to hold types fetched
	** from the titles table.
	*/ 
	EXEC SQL BEGIN DECLARE SECTION;
	char	a_type[TYPESIZE+1];
	EXEC SQL END DECLARE SECTION;

	/* A buffer to hold user's fgets() entry. */
	char	buf[128];

	/* Declare a cursor to select a list of book types. */
	EXEC SQL DECLARE typelist CURSOR FOR
	SELECT DISTINCT type FROM titles;

	/* Open the cursor. */
	EXEC SQL OPEN typelist;

	/*
	** List the book types on the screen. Break out of the
	** loop when a FETCH sets sqlca.sqlcode to 100.
	*/
	printf("\n\nSelect a Book Type:\n\n");
	for (;;)
	{
		EXEC SQL FETCH typelist INTO :a_type;
		if (sqlca.sqlcode == 100)
			break;
		printf("\t%s\n", a_type);
	}

	/*
	** Prompt the user for the book type. We're using fgets() 
	** for the input function. We'll have to read the complete 
	** input from stdin, therefore we'll fgets() into a large 
	** temporary buffer, remove possible "\n" at the end of the
	** string and copy the correct number of bytes into the 
	** variable passed from main().
	*/
	printf("\nbook type? ");
	fgets(buf, sizeof(buf), stdin);
	if (buf[strlen(buf) - 1] == '\n')
	{
		buf[strlen(buf) - 1] = '\0';
	}

	strncpy(type, buf, TYPESIZE);
	type[TYPESIZE] = EOLN;

	/*
	** Close the cursor and return the length of the type string.
	*/
	EXEC SQL CLOSE typelist;
	return (strlen(type));	
}

/*
**	Displays a list of title id's and titles that are
**	from the selected type. Lets the user enter a title id. 
**	Copies the entry into tid, and returns the number of 
**	characters entered.
*/
int 
get_titleid(char *tid, char *type)
{
	/*
	** Declare destination variables for the fetch. 
	*/
	EXEC SQL BEGIN DECLARE SECTION;
	char	b_titleid[TIDSIZE+1];
	char	b_title[65];
	/* 
	** We need to make a copy of the type argument because
	** we can't put the formal argument in a declare section.
	*/
	char	b_type[TYPESIZE+1];
	EXEC SQL END DECLARE SECTION;

	/* A buffer to hold keyboard input. */
	char	buf[128];

	/* Copy the type into b_type so we can use it to declare
	** the cursor.
	*/
	strcpy(b_type, type);

	/* This cursor selects title_id and title to display
	** a list for the user. The substring() function truncates
	** titles, if necessary, so they'll fit on an 80 column
	** display.
	*/
	EXEC SQL DECLARE titlelist CURSOR FOR
	SELECT	title_id, substring(title,1,64) 
	FROM	titles
	WHERE	type LIKE :b_type;

	/* Set b_titleid to null so we'll know if no titles
	** were selected for the specified type.
	*/
	b_titleid[0] = EOLN;

	/* Open the cursor. */
	EXEC SQL OPEN titlelist;

	/* Display the list of titles */ 
	printf("\n\nSelect a title:\n\n");
	for (;;)
	{
		EXEC SQL FETCH titlelist INTO :b_titleid, :b_title;
		if (sqlca.sqlcode == 100)
			break;
		printf("   %-8s %s\n", b_titleid, b_title);
	}

	/* If b_titleid is still null, no titles were found, so 
	** we won't ask the user to select one. 
	*/
	if (!strlen(b_titleid))
	{
		printf("\nThere are no '%s' titles.\n", b_type);
		tid[0] = EOLN;
	}
	else
	{
		/*
		** We use fgets() as the input function. A title_id is
		** only 6 bytes long but we'll have to read in the 
		** complete input from stdin. Therefore, we'll fgets() into
		** a longer buffer, remove any possible "\n" at the end of
		** the string and copy 6 bytes into the tid variable.
		*/
		printf("\ntitle ID? ");
		fgets(buf, sizeof(buf), stdin);
		if (buf[strlen(buf) - 1] == '\n')
		{
			buf[strlen(buf) - 1] = '\0';
		}

		strncpy(tid, buf, TIDSIZE);
		tid[TIDSIZE] = EOLN;
	}

	/*
	** Close the cursor and return the number of characters entered.
	*/
	EXEC SQL CLOSE titlelist;
	return (strlen(tid));	
}

/*
** void show_book(typeid) 
**
**	Displays information about the book with the specified
**	title id. A book can have coauthors, so we need a cursor
**	to list the authors.
*/
void 
show_book(char *titleid)
{
	EXEC SQL BEGIN DECLARE SECTION;

	/* 
	** Destination hostvar struct for INTO section fetches. 
	*/
	struct _hvar
	{
		char	m_titleid[7];
		char	m_title[65];
		char	m_pubname[41];
		char	m_pubcity[21];
		char	m_pubstate[3];	
		char	m_notes[201];
	} hvar_struct;

	char	m_author[62];

	/* 
	** A struct of indicator variables. Note that the number
	** of elements in the struct must be te same as the number
	** of elements in the main hostvar structure.
	*/ 
	struct _indvar
	{
		short	i_titleid;
		short	i_title;
		short	i_pubname;
		short	i_pubcity;
		short	i_pubstate;	
		short	i_notes;   /* Is Nullable */
	} indvar_struct;

	EXEC SQL END DECLARE SECTION;

	/* These variables are used in the word-wrapping routine
	** that display the notes.
	*/
	char	*p, noteline[65];
	int	i, notelen;

	/* We need a local copy of the titleid to use in the
	** fetch.
	*/
	strcpy(hvar_struct.m_titleid, titleid);	

	/* 
	** Fetch the information about the title and publisher. A
	** cursor is not needed because the title id is unique. 
	** No more than one row can be returned.
	**
	** Note: INDICATOR keyword is optional, one can also do:
	**	   SELECT ... INTO :hvar_struct:indvar_struct
	*/
	EXEC SQL 
	SELECT	title_id, substring(title, 1, 64), pub_name, city, state, notes
	INTO    :hvar_struct INDICATOR :indvar_struct
	FROM	titles, publishers
	WHERE	titles.pub_id = publishers.pub_id 
	AND	title_id = :hvar_struct.m_titleid;

	/* sqlca.sqlcode is set to 100 if the title_id isn't found. */
	if (sqlca.sqlcode == 100) 
	{
		printf("\n\t** Can't find title '%s'.\n", 
					hvar_struct.m_titleid);
		return;
	}

	/* Display the title and publisher's name and address. */
	printf("\n\n%s", hvar_struct.m_title);
	printf("\n\nPub:\t%s", hvar_struct.m_pubname);
	printf("\n\t%s %s", hvar_struct.m_pubcity, 
				hvar_struct.m_pubstate);

	/* There can be more than one author, so we need a cursor
	** to list them. We can let ASE combine the first
	** and last names so that only one column is returned.
	*/
	EXEC SQL DECLARE authors CURSOR FOR
	SELECT  au_fname + ' ' + au_lname 
	FROM	authors, titleauthor
	WHERE	authors.au_id = titleauthor.au_id
	AND	title_id = :hvar_struct.m_titleid;

	/* Open the cursor. */
	EXEC SQL OPEN authors;

	/* Each row fetched is an additional coauthor. */ 
	printf("\n\nBy:");
	for(;;)
	{
		EXEC SQL FETCH authors INTO :m_author;
		if (sqlca.sqlcode == 100)
			break;
		printf("\t%s\n", m_author);
	}

	/* Close the cursor. */
	EXEC SQL CLOSE authors;
		
	/* 
	** This routine prints the notes column on multiple lines
	** with word wrapping. Nothing is printed if the indicator
	** variable was set to -1.
	**
	** With the struct of indicators one can scan the struct
	** members individually for an indicator element that is set 
	** to '-1', '0' or '>0'.
	** Do a shortcut here, only i_notes is NULLABLE...
	*/
	if (indvar_struct.i_notes != -1)
	{
		register  char  *q;

		printf("\nNotes:");
		notelen = strlen(hvar_struct.m_notes);
		p = hvar_struct.m_notes;
		while (notelen > 64)
		{
			noteline[64] = EOLN;
			strncpy(noteline, p, 64);
			for (i = strlen(noteline), q = noteline + i; 
				i  &&  ! (*q == ' ' || *q == '\t'); 
				i--, q--);
			noteline[i] = EOLN;
			notelen -= strlen(noteline);
			printf("\t%s\n", noteline);
			p += (strlen(noteline)+1);
		}
		if (notelen)
			printf("\t%s\n", p);
	}

	/* Pause before returning so the user can read the display. */
	printf("\npress return...");
	getchar();
	return;
}

/*
** void error_handler()
** 
**	Displays error codes and numbers from the SQLCA and exits with
**	an ERREXIT status. 
*/
void 
error_handler(void)
{
	fprintf(stderr, "\n** SQLCODE=(%ld)", sqlca.sqlcode);

	if (sqlca.sqlerrm.sqlerrml)
	{
		fprintf(stderr, "\n** ASE Error ");
		fprintf(stderr, "\n** %s", sqlca.sqlerrm.sqlerrmc);
	}

	fprintf(stderr, "\n\n");

	exit(ERREXIT);
}

/*
** void warning_handler()
** 
**	Displays warning messages.
*/
void 
warning_handler(void)
{

	if (sqlca.sqlwarn[1] == 'W')
	{
		fprintf(stderr, 
			"\n** Data truncated.\n");
	}

	if (sqlca.sqlwarn[3] == 'W')
	{
		fprintf(stderr, 
			"\n** Insufficient host variables to store results.\n");
	}	
	return;
}

