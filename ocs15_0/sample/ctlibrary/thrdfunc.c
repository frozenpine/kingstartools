/*
** Sybase Open Client 
** ------------------
**
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
*/

/*
**  thrdfunc.c 
**
**  Thread and synchronization routines for Pthread based sample. 
*/

#define DEBUG 1
#define	_INCLUDE_POSIX_SOURCE

#include	<pthread.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<memory.h>
#include	<sys/errno.h>
#include	<ctpublic.h>
#include 	<thrdfunc.h>

pthread_mutex_t		global = PTHREAD_MUTEX_INITIALIZER ;

typedef struct _possem_
{
	CS_INT		ev;	/* The value of the semaphore */
	pthread_mutex_t pm;	/* Mutex for access to the sem */
	pthread_cond_t  pc;	/* The condition variable */
} POSSEM;

/*
** ex_create_mutex()
**
**	Create a Pthread mutex
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_create_mutex(CS_VOID **mutex)
{
	pthread_mutex_t		*pm;
	pthread_mutexattr_t	*pmap;

	if((pm = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)))
		== (pthread_mutex_t *)NULL)
	{
		return(CS_FAIL);
	}

	if((pmap = (pthread_mutexattr_t *)malloc(sizeof(pthread_mutexattr_t)))
		== (pthread_mutexattr_t *)NULL)
	{
                free(pm);
		return(CS_FAIL);
	}

	if(pthread_mutexattr_init(pmap) < 0)
	{
		free(pm);
		free(pmap);
		return(CS_FAIL);
	}

	if(pthread_mutex_init(pm, pmap) == -1)
	{
		free(pm);
		free(pmap);
		return(CS_FAIL);
	}

	free(pmap);

	*mutex = (CS_VOID *)pm;
	return(CS_SUCCEED);
}

/*
** ex_delete_mutex()
**
**	Delete a Pthread mutex
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_delete_mutex(CS_VOID *mutex)
{
	if(mutex == (CS_VOID *)NULL)
	{
		return(CS_FAIL);
	}

	if(pthread_mutex_destroy((pthread_mutex_t *)mutex) < 0)
	{
		return(CS_FAIL);
	}

	free(mutex);
	return(CS_SUCCEED);
}

/*
** ex_lock_mutux()
**
**	Lock a Pthread mutex
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_take_mutex(CS_VOID *mutex)
{
	
	if(pthread_mutex_lock((pthread_mutex_t *)mutex) < 0)
	{
		return(CS_FAIL);
	}
	
	return(CS_SUCCEED);
}

/*
** ex_unlock_mutux()
**
**	Unlock a Pthread mutex
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_release_mutex(CS_VOID *mutex)
{
	
	if(pthread_mutex_unlock((pthread_mutex_t *)mutex) == -1)
	{
		return(CS_FAIL);
	}
	return(CS_SUCCEED);
}

/*
** ex_create_sem()
**
**	Create a POS semaphore
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_create_sem(CS_VOID **sem, CS_UINT count)
{
	POSSEM *possema ;
	pthread_mutexattr_t	attr ;
	pthread_condattr_t	cattr ;

	if( (possema = (POSSEM *)malloc(sizeof(POSSEM)) ) == (POSSEM *)NULL)
	{
		return(CS_FAIL);
	}

	if(pthread_mutexattr_init(&attr) < 0)
	{
		free(possema);
		return(CS_FAIL);
	}

	if(pthread_mutex_init(&(possema->pm), &attr) < 0)
	{
		free(possema);
		return(CS_FAIL);
	}

	if(pthread_condattr_init(&cattr) < 0)
	{
		free(possema);
		return(CS_FAIL);
	}

	if(pthread_cond_init(&(possema->pc), &cattr) == -1)
	{
		free(possema);
		return(CS_FAIL);
	}

	possema->ev = (CS_INT)0;
	*sem = (CS_VOID *)possema;
	return(CS_SUCCEED);
}

/*
** ex_delete_sem()
**
**	Delete a POS semaphore
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_delete_sem(CS_VOID *sem)
{
	POSSEM	*possem;

	/* Initialization						*/
	possem = (POSSEM *)sem;

	if(possem == NULL)
	{
		return(CS_FAIL);
	}

	if(pthread_cond_destroy(&(possem->pc)) < 0)
	{
		return(CS_FAIL);
	}

	if(pthread_mutex_destroy(&(possem->pm)) < 0)
	{
		return(CS_FAIL);
	}

	free(possem);
	return(CS_SUCCEED);
}

/*
** ex_release_sem()
**
**	Release a POS semaphore
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_release_sem(CS_VOID *sem)
{
	POSSEM	*possem;

	/* Initialization						*/
	possem = (POSSEM *)sem;

	if(pthread_mutex_lock(&(possem->pm)) < 0)
	{
		return(CS_FAIL);
	}

	/* Set the event						*/
	possem->ev++ ;

	/* We need to only broadcast if we are moving from 0		*/
	if(possem->ev == 1)
	{
		if(pthread_cond_broadcast(&(possem->pc)) == -1)
		{

			if(pthread_mutex_unlock(&(possem->pm)) < 0)
			{
			}
			return(CS_FAIL);
		}
	}

	if(pthread_mutex_unlock(&(possem->pm)) < 0)
	{
		return(CS_FAIL);
	}
	return(CS_SUCCEED);
}

/*
** ex_waitfor_sem()
**
**	Wait on a POS semaphore. 
**
** Returns:
**	CS_SUCCEED
**	CS_FAIL
**
** Side Effects:
**      None
*/
CS_RETCODE 
ex_waitfor_sem(CS_VOID *sem)
{
	POSSEM		*possem;

	/* Initialization						*/
	possem = (POSSEM *)sem;

	if(pthread_mutex_lock(&(possem->pm)) < 0)
	{
		return(CS_FAIL);
	}

	if(possem->ev < 0)
	{
		return(CS_FAIL);
	}

	if(possem->ev > 0)
	{
		possem->ev--;

		if(pthread_mutex_unlock(&(possem->pm)) < 0)
		{
			return(CS_FAIL);
		}
		return(CS_SUCCEED);
	}

	/* Now wait for the condition to be set */

	while(possem->ev == 0)
	{
		if(pthread_cond_wait(&(possem->pc), &(possem->pm)) < 0)
		{

			if(pthread_mutex_unlock(&(possem->pm)) < 0)
			{
			}
			return(CS_FAIL);
		}
	}

	possem->ev--;

	if(pthread_mutex_unlock(&(possem->pm)) == -1)
	{
		return(CS_FAIL);
	}
	return(CS_SUCCEED);
}

/*
**  ex_yield
**  
**  Type of function: internal
**  
**  Purpose:
**      Give up timeslice.
**  
**  Return value:
**  		none
**  
**  Side effects:
**  		none
**  
**  Synchronization:
**  		none
**  
**  Assumptions:
**  		none
**  
**  Notes:
**  		none
**  
**  History:
**  
*/
CS_VOID   
ex_yield(void)
{
	/* this is depricated in POSIX 
        pthread_yield();
	*/
}

/*
**  ex_create_thread
**  
**  Type of function: internal
**  
**  Purpose:
**      allocate the necessary data structures, start a new thread
**  
**  Return value:
**  		CS_FAIL or CS_SUCCEED.
**  
**  Side effects:
**  		none
**  
**  Synchronization:
**  		none
**  
**  Assumptions:
**  		none
**  
**  Notes:
**  		none
**  
**  History:
**  
*/
CS_RETCODE   
ex_create_thread(CS_VOID **thread, CS_VOID *startRoutine, CS_VOID *arg)
{
        pthread_t       *pt;
        
        if((pt = (pthread_t *)malloc(sizeof(pthread_t))) == (pthread_t *)NULL)
        {
                return(CS_FAIL);
        }
        if(pthread_create(pt, NULL, (void *(*)(void *))startRoutine, arg) < 0)
        {                 
		free(pt);
                return(CS_FAIL);
        }
        *thread = pt;
        return(CS_SUCCEED);
}


/*
**
**  ex_detach_thread
**
**  Type of function: external
**
**  Purpose:
**	Detach internal thread resources. Thread object is not anymore
**	accessible.
**
**  Return value:
**              CS_FAIL or CS_SUCCEED.
**
**  Side effects:
**
**  Synchronization:
**              none
**
**  Assumptions:
**
**  Notes:
**
**  History:
**
*/
CS_RETCODE 
ex_detach_thread(CS_VOID *threadp)
{
        pthread_t       *handle;        /* thread to kill               */

        /* Initialize */
        handle = (pthread_t *)threadp;

        /*
        ** Detach all resources associated with this thread
        */
        (void)pthread_detach(*handle);

        /* Get the real Free function  and free the handle */
        free(threadp);

        return CS_SUCCEED;
}

/*
** ex_take_global_mutex()
**
**	Take global mutex
**
** Returns:
**      CS_SUCCEED - always
**
** Side Effects:
**      None
*/
CS_RETCODE   
ex_take_global_mutex(void)
{
        pthread_mutex_lock(&global);
	return(CS_SUCCEED);
}

/*
** ex_release_global_mutex()
**
**      Release global mutex
**
** Returns:
**      CS_SUCCEED - always
**
** Side Effects:
**      None
*/
CS_RETCODE   
ex_release_global_mutex(void)
{
        pthread_mutex_unlock(&global);
	return(CS_SUCCEED);
}

/*
** ex_create_global_mutex()
**
**	Setup global mutex mechanism. 
**	For this thread scheme, this is a empty stub routine. 
**
** Returns:
**      CS_SUCCEED - always
**
** Side Effects:
**      None
*/
CS_RETCODE   
ex_create_global_mutex(void)
{
	return(CS_SUCCEED);
}

/*
** ex_delete_global_mutex()
**
**      Delete global mutex mechanism. 
**      For this thread scheme, this is a empty stub routine.
**
** Returns:
**      CS_SUCCEED - always
**
** Side Effects:
**      None
*/
CS_RETCODE   
ex_delete_global_mutex(void)
{
	return(CS_SUCCEED);
}
