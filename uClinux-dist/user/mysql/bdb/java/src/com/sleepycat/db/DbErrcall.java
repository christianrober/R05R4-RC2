/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997, 1998, 1999, 2000
 *	Sleepycat Software.  All rights reserved.
 *
 *	$Id: DbErrcall.java,v 1.2 2005/07/25 04:32:28 magicyang Exp $
 */

package com.sleepycat.db;

/**
 *
 * @author Donald D. Anderson
 */
public interface DbErrcall
{
    // methods
    //
    public abstract void errcall(String prefix, String buffer);
}

// end of DbErrcall.java
