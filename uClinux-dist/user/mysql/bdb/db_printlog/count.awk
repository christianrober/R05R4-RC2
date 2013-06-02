# $Id: count.awk,v 1.2 2005/07/25 04:32:25 magicyang Exp $
#
# Print out the number of log records for transactions that we
# encountered.

/^\[/{
	if ($5 != 0)
		print $5
}
