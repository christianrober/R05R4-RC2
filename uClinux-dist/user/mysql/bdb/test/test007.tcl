# See the file LICENSE for redistribution information.
#
# Copyright (c) 1996, 1997, 1998, 1999, 2000
#	Sleepycat Software.  All rights reserved.
#
#	$Id: test007.tcl,v 1.2 2005/07/25 04:32:33 magicyang Exp $
#
# DB Test 7 {access method}
# Check that delete operations work.  Create a database; close database and
# reopen it.  Then issues delete by key for each entry.
proc test007 { method {nentries 10000} {tnum 7} args} {
	eval {test006 $method $nentries 1 $tnum} $args
}
