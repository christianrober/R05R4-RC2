/*
 * $Id: authorize.c,v 1.1 2005/07/25 05:52:00 magicyang Exp $
 *
 * Digest Authentication - Database support
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
 *
 * history:
 * ---------
 * 2003-02-28 scratchpad compatibility abandoned
 * 2003-01-27 next baby-step to removing ZT - PRESERVE_ZT (jiri)
 */


#include <string.h>
#include "../../ut.h"
#include "../../str.h"
#include "../../db/db.h"
#include "../../dprint.h"
#include "../../parser/digest/digest.h"
#include "../../parser/hf.h"
#include "authdb_mod.h"
#include "rfc2617.h"


#define MESSAGE_500 "Server Internal Error"

static char rpid_buffer[MAX_RPID_LEN];
static str rpid = {rpid_buffer, 0};

static inline int get_ha1(struct username* _username, str* _domain, char* _table, char* _ha1, str* _rpid)
{
	db_key_t keys[2];
	db_val_t vals[2];
	db_key_t col[2];
	db_res_t* res;
	str result;
	int n, nc;

	keys[0] = user_column;
	keys[1] = domain_column;
	col[0] = (_username->domain.len && !calc_ha1) ? (pass_column_2) : (pass_column);	
	col[1] = rpid_column;

	VAL_TYPE(vals) = VAL_TYPE(vals + 1) = DB_STR;
	VAL_NULL(vals) = VAL_NULL(vals + 1) = 0;
	
	VAL_STR(vals).s = _username->user.s;
	VAL_STR(vals).len = _username->user.len;

	VAL_STR(vals + 1).s = _domain->s;
	VAL_STR(vals + 1).len = _domain->len;

	n = (use_domain ? 2 : 1);
	nc = (use_rpid ? 2 : 1);
	db_use_table(db_handle, _table);
	if (db_query(db_handle, keys, 0, vals, col, n, nc, 0, &res) < 0) {
		LOG(L_ERR, "get_ha1(): Error while querying database\n");
		return -1;
	}

	if (RES_ROW_N(res) == 0) {
		DBG("get_ha1(): no result for user \'%.*s@%.*s\'\n", 
		    _username->user.len, ZSW(_username->user.s), (use_domain ? (_domain->len) : 0), ZSW(_domain->s));
		db_free_query(db_handle, res);
		return 1;
	}

        result.s = (char*)ROW_VALUES(RES_ROWS(res))[0].val.string_val;
	result.len = strlen(result.s);

	if (calc_ha1) {
		     /* Only plaintext passwords are stored in database,
		      * we have to calculate HA1 */
		calc_HA1(HA_MD5, &_username->whole, _domain, &result, 0, 0, _ha1);
		DBG("HA1 string calculated: %s\n", _ha1);
	} else {
		memcpy(_ha1, result.s, result.len);
		_ha1[result.len] = '\0';
	}

	if (use_rpid && (VAL_NULL(&(res->rows[0].values[1])) == 0)) {
		result.s = (char*)VAL_STRING(&(res->rows[0].values[1]));
		_rpid->len = strlen(result.s);
		memcpy(_rpid->s, result.s, _rpid->len);
	}

	db_free_query(db_handle, res);
	return 0;
}

/*
 * Calculate the response and compare with the given response string
 * Authorization is successfull if this two strings are same
 */
static inline int check_response(dig_cred_t* _cred, str* _method, char* _ha1)
{
	HASHHEX resp, hent;

	     /*
	      * First, we have to verify that the response received has
	      * the same length as responses created by us
	      */
	if (_cred->response.len != 32) {
		DBG("check_response(): Receive response len != 32\n");
		return 1;
	}

	     /*
	      * Now, calculate our response from parameters received
	      * from the user agent
	      */
	calc_response(_ha1, &(_cred->nonce), 
		      &(_cred->nc), &(_cred->cnonce), 
		      &(_cred->qop.qop_str), _cred->qop.qop_parsed == QOP_AUTHINT,
		      _method, &(_cred->uri), hent, resp);
	
	DBG("check_response(): Our result = \'%s\'\n", resp);
	
	     /*
	      * And simply compare the strings, the user is
	      * authorized if they match
	      */
	if (!memcmp(resp, _cred->response.s, 32)) {
		DBG("check_response(): Authorization is OK\n");
		return 0;
	} else {
		DBG("check_response(): Authorization failed\n");
		return 2;
	}
}


/*
 * Authorize digest credentials
 */
static inline int authorize(struct sip_msg* _m, str* _realm, char* _table, int _hftype)
{
	char ha1[256];
	int res;
	struct hdr_field* h;
	auth_body_t* cred;
	auth_result_t ret;
	str domain;

	domain = *_realm;

	ret = pre_auth_func(_m, &domain, _hftype, &h);
	
	switch(ret) {
	case ERROR:            return 0;
	case NOT_AUTHORIZED:   return -1;
	case DO_AUTHORIZATION: break;
	case AUTHORIZED:       return 1;
	}

	cred = (auth_body_t*)h->parsed;

		/* Clear the rpid buffer */
	rpid.len = 0;
	
	res = get_ha1(&cred->digest.username, &domain, _table, ha1, &rpid);
        if (res < 0) {
		     /* Error while accessing the database */
		if (sl_reply(_m, (char*)500, MESSAGE_500) == -1) {
			LOG(L_ERR, "authorize(): Error while sending 500 reply\n");
		}
		return 0;
	} else if (res > 0) {
		     /* Username not found in the database */
		return -1;
	}

	     /* Recalculate response, it must be same to authorize sucessfully */
        if (!check_response(&(cred->digest), &_m->first_line.u.request.method, ha1)) {
		ret = post_auth_func(_m, h, &rpid);
		switch(ret) {
		case ERROR:          return 0;
		case NOT_AUTHORIZED: return -1;
		case AUTHORIZED:     return 1;
		default:             return -1; 
		}
	}

	return -1;
}


/*
 * Authorize using Proxy-Authorize header field
 */
int proxy_authorize(struct sip_msg* _m, char* _realm, char* _table)
{
	     /* realm parameter is converted to str* in str_fixup */
	return authorize(_m, (str*)_realm, _table, HDR_PROXYAUTH);
}


/*
 * Authorize using WWW-Authorize header field
 */
int www_authorize(struct sip_msg* _m, char* _realm, char* _table)
{
	return authorize(_m, (str*)_realm, _table, HDR_AUTHORIZATION);
}
