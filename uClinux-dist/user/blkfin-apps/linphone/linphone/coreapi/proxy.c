/*
linphone
Copyright (C) 2000  Simon MORLAT (simon.morlat@free.fr)
*/
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include "linphonecore.h" 
#include <eXosip.h>
#include <osipparser2/osip_message.h>
#include "lpconfig.h"


void linphone_proxy_config_init(LinphoneProxyConfig *obj){
	memset(obj,0,sizeof(LinphoneProxyConfig));
	obj->rid=-1;
	obj->expires=600;
}

LinphoneProxyConfig *linphone_proxy_config_new(const gchar *server_addr){
	LinphoneProxyConfig *obj=NULL;
	int err;
	obj=g_new(LinphoneProxyConfig,1);
	linphone_proxy_config_init(obj);
	err=linphone_proxy_config_set_server_addr(obj,server_addr);
	if (err<0){
		linphone_proxy_config_destroy(obj);
		obj=NULL;
	}
	return obj;
}

void linphone_proxy_config_destroy(LinphoneProxyConfig *obj){
	if (obj->reg_proxy!=NULL) g_free(obj->reg_proxy);
	if (obj->reg_identity!=NULL) g_free(obj->reg_identity);
	if (obj->reg_route!=NULL) g_free(obj->reg_route);
}

static void linphone_proxy_config_register(LinphoneProxyConfig *obj){
	eXosip_lock();
	eXosip_register(obj->rid,obj->expires);
	eXosip_unlock();
	obj->reg_time=time(NULL);
}


gint linphone_proxy_config_set_server_addr(LinphoneProxyConfig *obj, const gchar *server_addr){
	int err;
	osip_from_t *url;
	if (server_addr==NULL || strlen(server_addr)==0) return -1;
	osip_from_init(&url);
	err=osip_from_parse(url,server_addr);
	if (err<0){
		g_warning("Could not parse %s",server_addr);
		osip_from_free(url);
		return -1;
	}else{
		if (obj->reg_proxy!=NULL) g_free(obj->reg_proxy);
		obj->reg_proxy=g_strdup(server_addr);
	}
	osip_from_free(url);
	return 0;
}

void linphone_proxy_config_set_identity(LinphoneProxyConfig *obj, const gchar *identity){
	int err=0;
	osip_from_t *url;
	if (identity!=NULL && strlen(identity)>0){
		osip_from_init(&url);
		err=osip_from_parse(url,identity);
		if (err<0){
			g_warning("Could not parse %s",identity);
			osip_from_free(url);
			return;
		}
	} else err=-2;
	if (obj->reg_identity!=NULL) {
		g_free(obj->reg_identity);
		obj->reg_identity=NULL;
	}
	if (err==-2) obj->reg_identity=NULL;
	else obj->reg_identity=g_strdup(identity);
	
}

void linphone_proxy_config_set_route(LinphoneProxyConfig *obj, const gchar *route)
{
	int err;
	osip_uri_param_t *lr_param=NULL;
	osip_route_t *rt=NULL;
	char *tmproute=NULL;
	if (route!=NULL && strlen(route)>0){
		osip_route_init(&rt);
		err=osip_route_parse(rt,route);
		if (err<0){
			g_warning("Could not parse %s",route);
			osip_route_free(rt);
			return ;
		}
		if (obj->reg_route!=NULL) {
			g_free(obj->reg_route);
			obj->reg_route=NULL;
		}
			
		/* check if the lr parameter is set , if not add it */
		osip_uri_uparam_get_byname(rt->url, "lr", &lr_param);
	  	if (lr_param==NULL){
			osip_uri_uparam_add(rt->url,osip_strdup("lr"),NULL);
			osip_route_to_str(rt,&tmproute);
			obj->reg_route=g_strdup(tmproute);
			osip_free(tmproute);
		}else obj->reg_route=g_strdup(route);
	}else{
		if (obj->reg_route!=NULL) g_free(obj->reg_route);
		obj->reg_route=NULL;
	}
}

void linphone_proxy_config_enableregister(LinphoneProxyConfig *obj, gboolean val){
	obj->reg_sendregister=val;
}

void linphone_proxy_config_expires(LinphoneProxyConfig *obj, gint val){
	if (val<=0) val=600;
	obj->expires=val;
}

void linphone_proxy_config_enable_publish(LinphoneProxyConfig *obj, gboolean val){
	obj->publish=val;
}

void linphone_proxy_config_edit(LinphoneProxyConfig *obj){
	obj->frozen=TRUE;
	if (obj->reg_sendregister){
		/* unregister */
		if (obj->registered) {
			eXosip_lock();
			/* obj->expires = 0; fixed by amd 27/MARCH/2005 */
			eXosip_register(obj->rid,0);
			
#if 0
			obj->rid=-1;
#endif
			eXosip_unlock();
		}
	}
}

void linphone_proxy_config_apply(LinphoneProxyConfig *obj,LinphoneCore *lc)
{
	obj->lc=lc;
	linphone_proxy_config_done(obj);
}

void linphone_proxy_config_done(LinphoneProxyConfig *obj)
{
	const gchar *id_str;
	if (obj->reg_identity!=NULL) id_str=obj->reg_identity;
	else id_str=linphone_core_get_primary_contact(obj->lc);
	obj->frozen=FALSE;
	if (obj->reg_sendregister){
		gchar *ct=NULL;
		if (obj->lc->net_conf.use_nat && (obj->lc->net_conf.nat_address!=NULL)) {
			osip_from_t *identity;
			osip_from_init(&identity);
			osip_from_parse(identity,id_str);
			osip_free(identity->url->host);
			identity->url->host=osip_strdup(obj->lc->net_conf.nat_address);
			identity->url->port=osip_malloc(15);
			snprintf(identity->url->port, 15, "%i", obj->lc->sip_conf.sip_port);
			osip_from_to_str(identity,&ct);
			osip_from_free(identity);
		}
		obj->rid=eXosip_register_init((gchar*)id_str,obj->reg_proxy,ct);
		linphone_proxy_config_register(obj);
		if (ct!=NULL) g_free(ct);
	}
}

void linphone_proxy_config_set_realm(LinphoneProxyConfig *cfg, const char *realm)
{
	if (cfg->realm!=NULL) {
		g_free(cfg->realm);
		cfg->realm=NULL;
	}
	if (realm!=NULL) cfg->realm=g_strdup(realm);
}

void linphone_core_update_proxy_register(LinphoneCore *lc){
	GList *elem;
	for(elem=linphone_core_get_proxy_config_list(lc);elem!=NULL;elem=g_list_next(elem)){
		LinphoneProxyConfig *cfg=(LinphoneProxyConfig*) elem->data;
		if (cfg->reg_sendregister && cfg->rid>=0){
			if ( (time(NULL)-cfg->reg_time) > cfg->expires){
				/* update the registration */
				linphone_proxy_config_register(cfg);
			}
			else if (time(NULL)-cfg->reg_time > 200) {
				/* update the registration more often */
				linphone_proxy_config_register(cfg);
			}
		}
	}
}


int linphone_proxy_config_send_publish(LinphoneProxyConfig *proxy,
			       LinphoneOnlineStatus presence_mode)
{
  osip_message_t *pub;
  int i;
  const gchar *from=NULL;
  char buf[5000];

  if (proxy->publish==FALSE) return 0;
	
  if (proxy!=NULL) {
    from=linphone_proxy_config_get_identity(proxy);
  }
  if (from==NULL) from=linphone_core_get_primary_contact(proxy->lc);

  if (presence_mode==LINPHONE_STATUS_ONLINE)
    {
      snprintf(buf, 5000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<presence xmlns=\"urn:ietf:params:xml:ns:pidf\"\n\
          entity=\"%s\">\n\
<tuple id=\"sg89ae\">\n\
<status>\n\
<basic>open</basic>\n\
</status>\n\
<contact priority=\"0.8\">%s</contact>\n\
<note>online</note>\n\
</tuple>\n\
</presence>",
	       from, from);
    }
  else if (presence_mode==LINPHONE_STATUS_BUSY
	   ||presence_mode==LINPHONE_STATUS_NOT_DISTURB)
    {
      snprintf(buf, 5000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<presence xmlns=\"urn:ietf:params:xml:ns:pidf\"\n\
          xmlns:es=\"urn:ietf:params:xml:ns:pidf:status:rpid-status\"\n\
          entity=\"%s\">\n\
<tuple id=\"sg89ae\">\n\
<status>\n\
<basic>open</basic>\n\
<es:activities>\n\
  <es:activity>busy</es:activity>\n\
</es:activities>\n\
</status>\n\
<contact priority=\"0.8\">%s</contact>\n\
<note>busy</note>\n\
</tuple>\n\
</presence>",
	      from, from);
    }
  else if (presence_mode==LINPHONE_STATUS_BERIGHTBACK)
    {
      snprintf(buf, 5000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<presence xmlns=\"urn:ietf:params:xml:ns:pidf\"\n\
          xmlns:es=\"urn:ietf:params:xml:ns:pidf:status:rpid-status\"\n\
          entity=\"%s\">\n\
<tuple id=\"sg89ae\">\n\
<status>\n\
<basic>open</basic>\n\
<es:activities>\n\
  <es:activity>in-transit</es:activity>\n\
</es:activities>\n\
</status>\n\
<contact priority=\"0.8\">%s</contact>\n\
<note>be right back</note>\n\
</tuple>\n\
</presence>",
	      from,from);
    }
  else if (presence_mode==LINPHONE_STATUS_AWAY
	   ||presence_mode==LINPHONE_STATUS_MOVED
	   ||presence_mode==LINPHONE_STATUS_ALT_SERVICE)
    {
      snprintf(buf, 5000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<presence xmlns=\"urn:ietf:params:xml:ns:pidf\"\n\
          xmlns:es=\"urn:ietf:params:xml:ns:pidf:status:rpid-status\"\n\
          entity=\"%s\">\n\
<tuple id=\"sg89ae\">\n\
<status>\n\
<basic>open</basic>\n\
<es:activities>\n\
  <es:activity>away</es:activity>\n\
</es:activities>\n\
</status>\n\
<contact priority=\"0.8\">%s</contact>\n\
<note>away</note>\n\
</tuple>\n\
</presence>",
	      from, from);
    }
  else if (presence_mode==LINPHONE_STATUS_ONTHEPHONE)
    {
      snprintf(buf, 5000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<presence xmlns=\"urn:ietf:params:xml:ns:pidf\"\n\
          xmlns:es=\"urn:ietf:params:xml:ns:pidf:status:rpid-status\"\n\
          entity=\"%s\">\n\
<tuple id=\"sg89ae\">\n\
<status>\n\
<basic>open</basic>\n\
<es:activities>\n\
  <es:activity>on-the-phone</es:activity>\n\
</es:activities>\n\
</status>\n\
<contact priority=\"0.8\">%s</contact>\n\
<note>on the phone</note>\n\
</tuple>\n\
</presence>",
	      from, from);
    }
  else if (presence_mode==LINPHONE_STATUS_OUTTOLUNCH)
    {
      snprintf(buf, 5000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<presence xmlns=\"urn:ietf:params:xml:ns:pidf\"\n\
          xmlns:es=\"urn:ietf:params:xml:ns:pidf:status:rpid-status\"\n\
          entity=\"%s\">\n\
<tuple id=\"sg89ae\">\n\
<status>\n\
<basic>open</basic>\n\
<es:activities>\n\
  <es:activity>meal</es:activity>\n\
</es:activities>\n\
</status>\n\
<contact priority=\"0.8\">%s</contact>\n\
<note>out to lunch</note>\n\
</tuple>\n\
</presence>",
	      from, from);
    }
  else if (presence_mode==LINPHONE_STATUS_OFFLINE)
    {
      /* */
      snprintf(buf, 5000, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<presence xmlns=\"urn:ietf:params:xml:ns:pidf\"\n\
xmlns:es=\"urn:ietf:params:xml:ns:pidf:status:rpid-status\"\n\
entity=\"%s\">\n%s",
	      from,
"<tuple id=\"sg89ae\">\n\
<status>\n\
<basic>closed</basic>\n\
<es:activities>\n\
  <es:activity>permanent-absence</e:activity>\n\
</es:activities>\n\
</status>\n\
</tuple>\n\
\n</presence>\n");
    }

  i = eXosip_build_publish(&pub, (char *)from, (char *)from, NULL, "presence", "1800", "application/pidf+xml", buf);

  if (i<0)
    {
      g_message("Failed to build publish request.");
      return -1;
    }

  eXosip_lock();
  i = eXosip_publish(pub, from); /* should update the sip-if-match parameter
				    from sip-etag  from last 200ok of PUBLISH */
  eXosip_unlock();
  if (i<0)
    {
      g_message("Failed to send publish request.");
      return -1;
    }
  return 0;
}

void linphone_core_add_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *cfg){
	lc->sip_conf.proxies=g_list_append(lc->sip_conf.proxies,(gpointer)cfg);
	linphone_proxy_config_apply(cfg,lc);
}

extern void linphone_friend_check_for_removed_proxy(LinphoneFriend *lf, LinphoneProxyConfig *cfg);

void linphone_core_remove_proxy_config(LinphoneCore *lc, LinphoneProxyConfig *cfg){
	GList *elem;
	lc->sip_conf.proxies=g_list_remove(lc->sip_conf.proxies,(gpointer)cfg);
	/* add to the list of destroyed proxies, so that the possible unREGISTER request can succeed authentication */
	lc->sip_conf.deleted_proxies=g_list_append(lc->sip_conf.deleted_proxies,(gpointer)cfg);
	/* this will unREGISTER */
	linphone_proxy_config_edit(cfg);
	if (lc->default_proxy==cfg){
		lc->default_proxy=NULL;
	}
	/* invalidate all references to this proxy in our friend list */
	for (elem=lc->friends;elem!=NULL;elem=g_list_next(elem)){
		linphone_friend_check_for_removed_proxy((LinphoneFriend*)elem->data,cfg);
	}
	
}

void linphone_core_set_default_proxy(LinphoneCore *lc, LinphoneProxyConfig *config){
	/* check if this proxy is in our list */
	if (config!=NULL){
		if (g_list_find(lc->sip_conf.proxies,(gconstpointer)config)==NULL){
			g_warning("Bad proxy address: it is not in the list !");
			lc->default_proxy=NULL;
			return ;
		}
	}
	lc->default_proxy=config;
	
}
	

void linphone_core_set_default_proxy_index(LinphoneCore *lc, int index){
	if (index<0) linphone_core_set_default_proxy(lc,NULL);
	else linphone_core_set_default_proxy(lc,g_list_nth_data(lc->sip_conf.proxies,index));
}

int linphone_core_get_default_proxy(LinphoneCore *lc, LinphoneProxyConfig **config){
	int pos=-1;
	if (config!=NULL) *config=lc->default_proxy;
	if (lc->default_proxy!=NULL){
		pos=g_list_position(lc->sip_conf.proxies,g_list_find(lc->sip_conf.proxies,(gpointer)lc->default_proxy));
	}
	return pos;
}

static gint rid_compare(LinphoneProxyConfig *cfg,int rid){
	g_message("cfg= %s, cfg->rid=%i, rid=%i",cfg->reg_proxy, cfg->rid, rid);
	return cfg->rid-rid;
}

LinphoneProxyConfig *linphone_core_get_proxy_config_from_rid(LinphoneCore *lc, int rid){
	GList *elem=g_list_find_custom(lc->sip_conf.proxies,GINT_TO_POINTER(rid),(GCompareFunc)rid_compare);
	if (elem==NULL){
		g_message("linphone_core_get_proxy_config_from_rid: searching in deleted proxies...");
		elem=g_list_find_custom(lc->sip_conf.deleted_proxies,GINT_TO_POINTER(rid),(GCompareFunc)rid_compare);
	}
	if (elem==NULL) return NULL;
	else return (LinphoneProxyConfig*)elem->data;
}

void linphone_core_retry_proxy_register(LinphoneCore *lc, const gchar *realm)
{
	GList *elem;
	for (elem=lc->sip_conf.proxies;elem!=NULL;elem=g_list_next(elem)){
		LinphoneProxyConfig *cfg=(LinphoneProxyConfig*)elem->data;
		/*g_message("linphone_core_retry_proxy_register: cfg->auth_pending=%i, cfg->realm=%s, realm=%s",
					cfg->auth_pending,cfg->realm,realm);*/
		if (cfg->auth_pending && cfg->realm!=NULL && strcmp(cfg->realm,realm)==0){
			g_message("Restarting REGISTER request for %s.",cfg->reg_proxy);
			linphone_proxy_config_register(cfg);
		}
	}
}

GList *linphone_core_get_proxy_config_list(LinphoneCore *lc){
	return lc->sip_conf.proxies;
}


void linphone_proxy_config_write_to_config_file(LpConfig *config, LinphoneProxyConfig *obj, int index)
{
	gchar key[50];

	sprintf(key,"proxy_%i",index);
	if (obj==NULL){
		lp_config_clean_section(config,key);
		return;
	}
	if (obj->reg_proxy!=NULL){
		lp_config_set_string(config,key,"reg_proxy",obj->reg_proxy);
	}
	if (obj->reg_route!=NULL){
		lp_config_set_string(config,key,"reg_route",obj->reg_route);
	}
	if (obj->reg_identity!=NULL){
		lp_config_set_string(config,key,"reg_identity",obj->reg_identity);
	}
	lp_config_set_int(config,key,"reg_expires",obj->expires);
	lp_config_set_int(config,key,"reg_sendregister",obj->reg_sendregister);
	lp_config_set_int(config,key,"publish",obj->publish);
}

LinphoneProxyConfig *linphone_proxy_config_new_from_config_file(LpConfig *config, int index)
{
	const gchar *tmp;
	LinphoneProxyConfig *cfg;
	gchar key[50];
	
	sprintf(key,"proxy_%i",index);

	if (!lp_config_has_section(config,key)){
		return NULL;
	}
	tmp=lp_config_get_string(config,key,"reg_proxy",NULL);
	if (tmp==NULL) return NULL;
	cfg=linphone_proxy_config_new(tmp);
	if (cfg==NULL) return NULL;
		
	tmp=lp_config_get_string(config,key,"reg_route",NULL);
	if (tmp!=NULL) linphone_proxy_config_set_route(cfg,tmp);
		
	tmp=lp_config_get_string(config,key,"reg_identity",NULL);
	if (tmp!=NULL) linphone_proxy_config_set_identity(cfg,tmp);
		
	linphone_proxy_config_expires(cfg,lp_config_get_int(config,key,"reg_expires",600));
	linphone_proxy_config_enableregister(cfg,lp_config_get_int(config,key,"reg_sendregister",0));
	
	linphone_proxy_config_enable_publish(cfg,lp_config_get_int(config,key,"publish",0));
	
	return cfg;
}
