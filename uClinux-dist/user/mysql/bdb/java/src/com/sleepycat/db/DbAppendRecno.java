/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2000
 *	Sleepycat Software.  All rights reserved.
 *
 *	$Id: DbAppendRecno.java,v 1.2 2005/07/25 04:32:28 magicyang Exp $
 */

package com.sleepycat.db;

/*
 * This interface is used by Db.set_append_recno()
 * 
 */
public interface DbAppendRecno
{
    public abstract void db_append_recno(Db db, Dbt data, int recno)
        throws DbException;
}

// end of DbAppendRecno.java
