# See the file LICENSE for redistribution information.
#
# Copyright (c) 1998, 1999, 2000
#	Sleepycat Software.  All rights reserved.
#
#	$Id: test040.tcl,v 1.2 2005/07/25 04:32:33 magicyang Exp $
#
# DB Test 40 {access method}
# DB_GET_BOTH functionality with off-page duplicates.
proc test040 { method {nentries 10000} args} {
	# Test with off-page duplicates
	eval {test038 $method $nentries 20 40 -pagesize 512} $args

	# Test with multiple pages of off-page duplicates
	eval {test038 $method [expr $nentries / 10] 100 40 -pagesize 512} $args
}
