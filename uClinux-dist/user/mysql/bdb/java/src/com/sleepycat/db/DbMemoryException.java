/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1999, 2000
 *	Sleepycat Software.  All rights reserved.
 *
 *	$Id: DbMemoryException.java,v 1.2 2005/07/25 04:32:29 magicyang Exp $
 */

package com.sleepycat.db;

public class DbMemoryException extends DbException
{
    // methods
    //

    public DbMemoryException(String s)
    {
        super(s);
    }

    public DbMemoryException(String s, int errno)
    {
        super(s, errno);
    }
}

// end of DbMemoryException.java
