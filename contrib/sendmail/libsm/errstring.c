/*
 * Copyright (c) 2001 Sendmail, Inc. and its suppliers.
 *	All rights reserved.
 * Copyright (c) 1983, 1995-1997 Eric P. Allman.  All rights reserved.
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the sendmail distribution.
 */

#include <sm/gen.h>
SM_RCSID("@(#)$Id: errstring.c,v 1.12 2001/10/03 16:09:32 ca Exp $")

#include <errno.h>
#include <stdio.h>	/* sys_errlist, on some platforms */

#include <sm/io.h>	/* sm_snprintf */
#include <sm/string.h>
#include <sm/errstring.h>

#if NAMED_BIND
# include <netdb.h>
#endif

#if LDAPMAP
# include <lber.h>
# include <ldap.h>			/* for LDAP error codes */
#endif /* LDAPMAP */

/*
**  Notice: this file is used by libmilter. Please try to avoid
**	using libsm specific functions.
*/

/*
**  SM_ERRSTRING -- return string description of error code
**
**	Parameters:
**		errnum -- the error number to translate
**
**	Returns:
**		A string description of errnum.
*/

const char *
sm_errstring(errnum)
	int errnum;
{
	char *ret;

	switch (errnum)
	{
	  case EPERM:
		/* SunOS gives "Not owner" -- this is the POSIX message */
		return "Operation not permitted";

	/*
	**  Error messages used internally in sendmail.
	*/

	  case E_SM_OPENTIMEOUT:
		return "Timeout on file open";

	  case E_SM_NOSLINK:
		return "Symbolic links not allowed";

	  case E_SM_NOHLINK:
		return "Hard links not allowed";

	  case E_SM_REGONLY:
		return "Regular files only";

	  case E_SM_ISEXEC:
		return "Executable files not allowed";

	  case E_SM_WWDIR:
		return "World writable directory";

	  case E_SM_GWDIR:
		return "Group writable directory";

	  case E_SM_FILECHANGE:
		return "File changed after open";

	  case E_SM_WWFILE:
		return "World writable file";

	  case E_SM_GWFILE:
		return "Group writable file";

	  case E_SM_GRFILE:
		return "Group readable file";

	  case E_SM_WRFILE:
		return "World readable file";

	/*
	**  DNS error messages.
	*/

#if NAMED_BIND
	  case HOST_NOT_FOUND + E_DNSBASE:
		return "Name server: host not found";

	  case TRY_AGAIN + E_DNSBASE:
		return "Name server: host name lookup failure";

	  case NO_RECOVERY + E_DNSBASE:
		return "Name server: non-recoverable error";

	  case NO_DATA + E_DNSBASE:
		return "Name server: no data known";
#endif /* NAMED_BIND */

	/*
	**  libsmdb error messages.
	*/

	  case SMDBE_MALLOC:
		return "Memory allocation failed";

	  case SMDBE_GDBM_IS_BAD:
		return "GDBM is not supported";

	  case SMDBE_UNSUPPORTED:
		return "Unsupported action";

	  case SMDBE_DUPLICATE:
		return "Key already exists";

	  case SMDBE_BAD_OPEN:
		return "Database open failed";

	  case SMDBE_NOT_FOUND:
		return "Key not found";

	  case SMDBE_UNKNOWN_DB_TYPE:
		return "Unknown database type";

	  case SMDBE_UNSUPPORTED_DB_TYPE:
		return "Support for database type not compiled into this program";

	  case SMDBE_INCOMPLETE:
		return "DB sync did not finish";

	  case SMDBE_KEY_EMPTY:
		return "Key is empty";

	  case SMDBE_KEY_EXIST:
		return "Key already exists";

	  case SMDBE_LOCK_DEADLOCK:
		return "Locker killed to resolve deadlock";

	  case SMDBE_LOCK_NOT_GRANTED:
		return "Lock unavailable";

	  case SMDBE_LOCK_NOT_HELD:
		return "Lock not held by locker";

	  case SMDBE_RUN_RECOVERY:
		return "Database panic, run recovery";

	  case SMDBE_IO_ERROR:
		return "I/O error";

	  case SMDBE_READ_ONLY:
		return "Database opened read-only";

	  case SMDBE_DB_NAME_TOO_LONG:
		return "Name too long";

	  case SMDBE_INVALID_PARAMETER:
		return "Invalid parameter";

	  case SMDBE_ONLY_SUPPORTS_ONE_CURSOR:
		return "Only one cursor allowed";

	  case SMDBE_NOT_A_VALID_CURSOR:
		return "Invalid cursor";

	  case SMDBE_OLD_VERSION:
		return "Berkeley DB file is an old version, recreate it";
	}

	/*
	**  LDAP error messages.
	*/

#if LDAPMAP
	if (errnum >= E_LDAPBASE)
		return ldap_err2string(errnum - E_LDAPBASE);
#endif /* LDAPMAP */

	ret = strerror(errnum);
	if (ret == NULL)
	{
		static char buf[30];

		(void) sm_snprintf(buf, sizeof buf, "Error %d", errnum);
		return buf;
	}
	return ret;
}
