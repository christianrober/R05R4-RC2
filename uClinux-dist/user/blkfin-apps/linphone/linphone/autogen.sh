#!/bin/sh
#AM_VERSION=1.6
if test -n "$AM_VERSION" ; then
	ACLOCAL=aclocal-${AM_VERSION}
	AUTOMAKE=automake-${AM_VERSION}
else
	ACLOCAL=aclocal
	AUTOMAKE=automake
fi

echo "Generating build scripts in linphone..."

$ACLOCAL -I m4
autoheader
$AUTOMAKE --force-missing --add-missing --copy
libtoolize --copy --force
autoconf
rm -rf config.cache

echo "Generating build scripts in oRTP..."
cd oRTP && ./autogen.sh
