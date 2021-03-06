/*
 * mangler module
 *
 * $Id: sdp_mangler.h,v 1.1 2005/07/25 05:52:03 magicyang Exp $
 *
 * Copyright (C) 2001-2003 Fhg Fokus
 *
 * This file is part of ser, a free SIP server.
 *
 * ser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * For a license to use the ser software under conditions
 * other than those described here, or to purchase support for this
 * software, please contact iptel.org by e-mail at the following addresses:
 *    info@iptel.org
 *
 * ser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* History:
 * --------
 *  2003-04-07 first version.  
 */

/* TO DO: precompiled expresions */

#ifndef SDP_MANGLER_H
#define SDP_MANGLER_H


#include "../../parser/msg_parser.h"	/* struct sip_msg */
#include "common.h"
#include <regex.h>


/* With STRICT_CHECK off:
If you define a port like 41231311 and BEST_EFFORT is defined it will 
consider a port the first 5 digits
Similary an ip like 12.31.12.313131132131 will be mangled with only 3 digits
from the last group 
*/

#ifdef STRICT_CHECK
	#define PORT_REGEX "(m=[a-z]+ [0-9]{1,5} )"
	#define IP_REGEX "(c=IN IP4 [0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}( |\n|\r))"
#endif


#ifndef STRICT_CHECK
	#define PORT_REGEX "m=[a-z]+ [0-9]{1,5}"
	#define IP_REGEX "(c=IN IP4 [0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3})"
#endif


#define MIN_ORIGINAL_PORT 1
#define MAX_ORIGINAL_PORT 65535
#define MIN_MANGLED_PORT  1
#define MAX_MANGLED_PORT  65535
#define MIN_OFFSET_VALUE -65535
#define MAX_OFFSET_VALUE  65535



regex_t *portExpression;
regex_t *ipExpression;



/* replaces all apearences of a port in lines like m=audio port with
	a new value for port which is oldvalue+offset 
	@param msg a pointer to a sip message
	@param offset value of an offset.Must be a numeric format like "-12345"
	@param unused unused parameter
	@return negative in case of error or number of replacements - 1
*/
int sdp_mangle_port (struct sip_msg *msg, char *offset, char *unused);

/* may replace all apearences of an ip in lines like c=IN IP4 ip with
	a new value for ip if the found ip matches the filter
	@param msg a pointer to a sip message
	@param oldip an filter for ip's in form ip/mask.Example
	of oldip "10.0.0.0/255.0.0.0" or "10.0.0.0/8"
	@param newip the ip replacing old ip
	@return negative in case of error or number of replacements - 1
*/

int sdp_mangle_ip (struct sip_msg *msg, char *oldip, char *newip);

int compile_expresions(char *port,char *ip);
int free_compiled_expresions();

#endif
