/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2000
 *	Sleepycat Software.  All rights reserved.
 *
 *	$Id: DbDupCompare.java,v 1.2 2005/07/25 04:32:28 magicyang Exp $
 */

package com.sleepycat.db;

/*
 * This interface is used by DbEnv.set_dup_compare()
 * 
 */
public interface DbDupCompare
{
    public abstract int dup_compare(Db db, Dbt dbt1, Dbt dbt2);
}

// end of DbDupCompare.java
