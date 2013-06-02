# $Id: commit.awk,v 1.2 2005/07/25 04:32:25 magicyang Exp $
#
# Output tid of committed transactions.

/txn_regop/ {
	print $5
}
