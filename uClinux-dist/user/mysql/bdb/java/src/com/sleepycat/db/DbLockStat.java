/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997, 1998, 1999, 2000
 *	Sleepycat Software.  All rights reserved.
 *
 *	$Id: DbLockStat.java,v 1.2 2005/07/25 04:32:29 magicyang Exp $
 */

package com.sleepycat.db;

/*
 * This is filled in and returned by the
 * DbLockTab.stat() method.
 */
public class DbLockStat
{
    public int st_maxlocks;             // Maximum number of locks in table.
    public int st_nmodes;               // Number of lock modes.
    public int st_nlockers;             // Number of lockers.
    public int st_nconflicts;           // Number of lock conflicts.
    public int st_nrequests;            // Number of lock gets.
    public int st_nreleases;            // Number of lock puts.
    public int st_ndeadlocks;           // Number of lock deadlocks.
    public int st_region_wait;          // Region lock granted after wait.
    public int st_region_nowait;        // Region lock granted without wait.
    public int st_regsize;              // Region size.
}

// end of DbLockStat.java
