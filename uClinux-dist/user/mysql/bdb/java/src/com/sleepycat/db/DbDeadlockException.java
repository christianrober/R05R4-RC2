/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1999, 2000
 *	Sleepycat Software.  All rights reserved.
 *
 *	$Id: DbDeadlockException.java,v 1.2 2005/07/25 04:32:28 magicyang Exp $
 */

package com.sleepycat.db;

public class DbDeadlockException extends DbException
{
    // methods
    //

    public DbDeadlockException(String s)
    {
        super(s);
    }

    public DbDeadlockException(String s, int errno)
    {
        super(s, errno);
    }
}

// end of DbDeadlockException.java
