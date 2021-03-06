/*
  * Linphone is sip (RFC3261) compatible internet phone.
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  */

#include "sdphandler.h"
#include <osipparser2/osip_port.h>
#include <osipparser2/sdp_message.h>
#include <eXosip.h>

#define keywordcmp(key,str)  strncmp(key,str,strlen(key))
#ifdef HAVE_GLIB
#include <glib.h>
#else
#include <uglib.h>
#endif

#define sstrdup_sprintf g_strdup_printf

#define eXosip_trace(loglevel,args)  do        \
{                       \
	char *__strmsg;  \
	__strmsg=g_strdup_printf args ;    \
	OSIP_TRACE(osip_trace(__FILE__,__LINE__,(loglevel),NULL,"%s\n",__strmsg)); \
	osip_free (__strmsg);        \
}while (0);


char * int_2char(int a){
	char *p=osip_malloc(16);
	snprintf(p,16,"%i",a);
	return p;
}

/* return the value of attr "field" for payload pt at line pos (field=rtpmap,fmtp...)*/
char *sdp_message_a_attr_value_get_with_pt(sdp_message_t *sdp,int pos,int pt,char *field)
{
	int i,tmppt=0,scanned=0;
	char *tmp;
	sdp_attribute_t *attr;
	for (i=0;(attr=sdp_message_attribute_get(sdp,pos,i))!=NULL;i++){
		if (keywordcmp(field,attr->a_att_field)==0){
			sscanf(attr->a_att_value,"%i %n",&tmppt,&scanned);
			if (pt==tmppt){
				tmp=attr->a_att_value+scanned;
				if (strlen(tmp)>0)
					return tmp;
				else {
					eXosip_trace(OSIP_WARNING,("sdp has a strange a= line."));
					continue;
				}
			}
		}
	}
	return NULL;
}



int
sdp_payload_init (sdp_payload_t * payload)
{
	memset (payload, 0, sizeof (sdp_payload_t));
	return 0;
}

sdp_context_t *sdp_handler_create_context(sdp_handler_t *handler, const char *localip, const char *username){
	sdp_context_t *ctx=osip_malloc(sizeof(sdp_context_t));
	memset(ctx,0,sizeof(sdp_context_t));
	if (localip!=NULL) ctx->localip=osip_strdup(localip);
	ctx->username=osip_strdup(username);
	ctx->handler=handler;
	return ctx;
}

void sdp_context_set_user_pointer(sdp_context_t * ctx, void* up){
	ctx->reference=up;
}

void *sdp_context_get_user_pointer(sdp_context_t * ctx){
	return ctx->reference;
}

int sdp_context_get_status(sdp_context_t* ctx){
	return ctx->negoc_status;
}

/* generate a template sdp */
sdp_message_t *
sdp_context_generate_template (sdp_context_t * ctx)
{
	sdp_message_t *local;
	int inet6;

	sdp_message_init (&local);
	if (strchr(ctx->localip,':')!=NULL){
		inet6=1;
	}else inet6=0;
	if (!inet6){
		sdp_message_v_version_set (local, osip_strdup ("0"));
		sdp_message_o_origin_set (local, osip_strdup (ctx->username),
				  osip_strdup ("123456"), osip_strdup ("654321"),
				  osip_strdup ("IN"), osip_strdup ("IP4"),
				  osip_strdup (ctx->localip));
		sdp_message_s_name_set (local, osip_strdup ("A conversation"));
		sdp_message_c_connection_add (local, -1,
				      osip_strdup ("IN"), osip_strdup ("IP4"),
				      osip_strdup (ctx->localip), NULL, NULL);
		sdp_message_t_time_descr_add (local, osip_strdup ("0"), osip_strdup ("0"));
	}else{
		sdp_message_v_version_set (local, osip_strdup ("0"));
		sdp_message_o_origin_set (local, osip_strdup (ctx->username),
				  osip_strdup ("123456"), osip_strdup ("654321"),
				  osip_strdup ("IN"), osip_strdup ("IP6"),
				  osip_strdup (ctx->localip));
		sdp_message_s_name_set (local, osip_strdup ("A conversation"));
		sdp_message_c_connection_add (local, -1,
				      osip_strdup ("IN"), osip_strdup ("IP6"),
				      osip_strdup (ctx->localip), NULL, NULL);
		sdp_message_t_time_descr_add (local, osip_strdup ("0"), osip_strdup ("0"));
	}
	return local;
}

/* to add payloads to the offer, must be called inside the write_offer callback */
void
sdp_context_add_payload (sdp_context_t * ctx, sdp_payload_t * payload, char *media)
{
	sdp_message_t *offer = ctx->offer;
	char *attr_field;
	if (!ctx->incb)
	{
		eXosip_trace (OSIP_ERROR,
			    ("You must not call sdp_context_add_*_payload outside the write_offer callback\n"));
		abort ();
	}
	if (payload->proto == NULL)
		payload->proto = "RTP/AVP";
	/*printf("payload->line=%i payload->pt=%i\n",payload->line, payload->pt);*/
	if (sdp_message_m_media_get (offer, payload->line) == NULL)
	{
		/*printf("Adding new mline %s \n",media);*/
		/* need a new line */
		sdp_message_m_media_add (offer, osip_strdup (media),
				 int_2char (payload->localport), NULL,
				 osip_strdup (payload->proto));
	}
	sdp_message_m_payload_add (offer, payload->line, int_2char (payload->pt));
	if (payload->a_rtpmap != NULL)
	{
		attr_field =
			sstrdup_sprintf ("%i %s", payload->pt,
					 payload->a_rtpmap);
		sdp_message_a_attribute_add (offer, payload->line,
				     osip_strdup ("rtpmap"), attr_field);
	}
	if (payload->a_fmtp != NULL)
	{
		attr_field =
			sstrdup_sprintf ("%i %s", payload->pt,
					 payload->a_fmtp);
		sdp_message_a_attribute_add (offer, payload->line, osip_strdup ("fmtp"),
				     attr_field);
	}
	if (payload->b_as_bandwidth != 0)
	{	
		if (sdp_message_bandwidth_get(offer,payload->line,0)==NULL){
			attr_field =
			sstrdup_sprintf ("%i", payload->b_as_bandwidth);
			sdp_message_b_bandwidth_add (offer, payload->line, osip_strdup ("AS"),
				     attr_field);
		}
	}
}

void
sdp_context_add_audio_payload (sdp_context_t * ctx, sdp_payload_t * payload)
{
	sdp_context_add_payload (ctx, payload, "audio");
}

void
sdp_context_add_video_payload (sdp_context_t * ctx, sdp_payload_t * payload)
{
	sdp_context_add_payload (ctx, payload, "video");
}

char *
sdp_context_get_offer ( sdp_context_t * ctx)
{
	sdp_message_t *offer;
	sdp_handler_t *sdph=ctx->handler;
	char *tmp;

	offer = sdp_context_generate_template (ctx);
	/* add audio codecs */
	ctx->offer = offer;
	ctx->incb = 1;
	if (sdph->set_audio_codecs != NULL)
		sdph->set_audio_codecs (ctx);
	if (sdph->set_video_codecs != NULL)
		sdph->set_video_codecs (ctx);
	ctx->incb = 0;
	sdp_message_to_str(offer,&tmp);
	ctx->offerstr=tmp;
	return tmp;
}


/* refuse the line */
static void refuse_mline(sdp_message_t *answer,char *mtype,char *proto, int mline)
{
	sdp_message_m_media_add (answer,
							 osip_strdup (mtype),
							 int_2char (0), NULL,
							 osip_strdup (proto));
	/* add a payload just to comply with sdp RFC.*/
	sdp_message_m_payload_add(answer,mline,int_2char(0));
}

char *
sdp_context_get_answer ( sdp_context_t *ctx,const char *remote_offer)
{
	sdp_message_t *answer=NULL,*remote=NULL;
	char *mtype=NULL, *tmp=NULL, *bw=NULL;
	char *proto=NULL, *port=NULL, *pt=NULL;
	int i, j, ncodec, m_lines_accepted = 0;
	int err;
	sdp_payload_t payload;
	sdp_handler_t *sdph=ctx->handler;

	
	sdp_message_init(&remote);
	err=sdp_message_parse(remote,remote_offer);
	if (err<0) {
		eXosip_trace(OSIP_WARNING,("Could not parse remote offer."));
		return NULL;
	}
	ctx->remote=remote;

	tmp = sdp_message_c_addr_get (remote, 0, 0);
	if (tmp == NULL)
	  tmp = sdp_message_c_addr_get (remote, -1, 0);
	if (ctx->localip==NULL) {
		/* NULL means guess, otherwise we use the address given as localip */
		eXosip_get_localip_for(tmp,&ctx->localip);
	}
	else eXosip_trace(OSIP_INFO1,("Using firewall address in sdp."));

	answer = sdp_context_generate_template (ctx);
	
	/* for each m= line */
	for (i = 0; !sdp_message_endof_media (remote, i); i++)
	{
		sdp_payload_init(&payload);
		mtype = sdp_message_m_media_get (remote, i);
		proto = sdp_message_m_proto_get (remote, i);
		port = sdp_message_m_port_get (remote, i);
		payload.remoteport = osip_atoi (port);
		payload.proto = proto;
		payload.line = i;
		payload.c_addr = sdp_message_c_addr_get (remote, i, 0);
		if (payload.c_addr == NULL)
			payload.c_addr = sdp_message_c_addr_get (remote, -1, 0);
		/* get application specific bandwidth, if any */
		bw=sdp_message_b_bandwidth_get(remote, i, 0);
		if (bw!=NULL) payload.b_as_bandwidth=atoi(bw);
		if (keywordcmp ("audio", mtype) == 0)
		{
			if (sdph->accept_audio_codecs != NULL)
			{
				ncodec = 0;
				/* for each payload type */
				for (j = 0;
				     ((pt =
				       sdp_message_m_payload_get (remote, i,
							  j)) != NULL); j++)
				{
					payload.pt = osip_atoi (pt);
					/* get the rtpmap associated to this codec, if any */
					payload.a_rtpmap =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "rtpmap");
					/* get the fmtp, if any */
					payload.a_fmtp =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "fmtp");
					
					/* ask the application if this codec is supported */
					err = sdph->accept_audio_codecs (ctx,
									 &payload);
					if (err == 0 && payload.localport > 0)
					{
						ncodec++;
						/* codec accepted */
						if (ncodec == 1)
						{
							/* first codec accepted, setup the line  */
							sdp_message_m_media_add
								(answer,
								 osip_strdup
								 (mtype),
								 int_2char
								 (payload.
								  localport),
								 NULL,
								 osip_strdup
								 (proto));
						}
						/* add the payload, rtpmap, fmtp */
						sdp_message_m_payload_add (answer, i,
								   int_2char
								   (payload.
								    pt));
						if (payload.a_rtpmap != NULL)
						{
							sdp_message_a_attribute_add
								(answer, i,
								 osip_strdup
								 ("rtpmap"),
								 sstrdup_sprintf
								 ("%i %s",
								  payload.pt,
								  payload.
								  a_rtpmap));
						}
						if (payload.a_fmtp != NULL)
						{
							sdp_message_a_attribute_add
								(answer, i,
								 osip_strdup
								 ("fmtp"),
								 sstrdup_sprintf
								 ("%i %s",
								  payload.pt,
								  payload.
								  a_fmtp));
						}
						if (payload.b_as_bandwidth !=
						    0)
						{
							if (sdp_message_bandwidth_get(answer,i,0)==NULL)
								sdp_message_b_bandwidth_add
								(answer, i,
								 osip_strdup
								 ("AS"),
								 sstrdup_sprintf
								 ("%i",
								  payload.
								  b_as_bandwidth));
						}
					}
				}
				if (ncodec == 0)
				{
					/* refuse the line */
					refuse_mline(answer,mtype,proto,i);
					
				}
				else
					m_lines_accepted++;
			}
			else
			{
				/* refuse this line (leave port to 0) */
				refuse_mline(answer,mtype,proto,i);
			}

		}
		else if (keywordcmp ("video", mtype) == 0)
		{
			if (sdph->accept_video_codecs != NULL)
			{
				ncodec = 0;
				/* for each payload type */
				for (j = 0;
				     ((pt =
				       sdp_message_m_payload_get (remote, i,
							  j)) != NULL); j++)
				{
					payload.pt = osip_atoi (pt);
					/* get the rtpmap associated to this codec, if any */
					payload.a_rtpmap =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "rtpmap");
					/* get the fmtp, if any */
					payload.a_fmtp =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "fmtp");
					/* ask the application if this codec is supported */
					err = sdph->accept_video_codecs (ctx,
									 &payload);
					if (err == 0 && payload.localport > 0)
					{
						ncodec++;
						/* codec accepted */
						if (ncodec == 1)
						{
							/* first codec accepted, setup the line  */
							sdp_message_m_media_add
								(answer,
								 osip_strdup
								 (mtype),
								 int_2char
								 (payload.localport), NULL,
								 osip_strdup
								 (proto));
						}
						/* add the payload, rtpmap, fmtp */
						sdp_message_m_payload_add (answer, i,
								   int_2char
								   (payload.
								    pt));
						if (payload.a_rtpmap != NULL)
						{
							sdp_message_a_attribute_add
								(answer, i,
								 osip_strdup
								 ("rtpmap"),
								 sstrdup_sprintf
								 ("%i %s",
								  payload.pt,
								  payload.
								  a_rtpmap));
						}
						if (payload.a_fmtp != NULL)
						{
							sdp_message_a_attribute_add
								(answer, i,
								 osip_strdup
								 ("fmtp"),
								 sstrdup_sprintf
								 ("%i %s",
								  payload.pt,
								  payload.
								  a_fmtp));
						}
					}
				}
				if (ncodec == 0)
				{
					/* refuse the line */
					refuse_mline(answer,mtype,proto,i);
				}
				else
					m_lines_accepted++;
			}
			else
			{
				/* refuse the line */
				refuse_mline(answer,mtype,proto,i);
			}
		}
	}
	ctx->answer = answer;
	if (m_lines_accepted > 0){
		ctx->negoc_status = 200;
		sdp_message_to_str(answer,&tmp);
		ctx->answerstr=tmp;
		return tmp;
	}else{
		ctx->negoc_status = 415;
		return NULL;
	}
}

void
sdp_context_read_answer (sdp_context_t *ctx, const char *remote_answer)
{
	sdp_message_t *remote;
	char *mtype;
	char *proto, *port, *pt, *bw;
	int i, j,err;
	sdp_payload_t payload;
	sdp_handler_t *sdph=ctx->handler;

	sdp_message_init(&remote);
	err=sdp_message_parse(remote,remote_answer);
	if (err<0) {
		eXosip_trace(OSIP_WARNING,("Could not parse remote offer."));
		return ;
	}
	ctx->remote=remote;
	/* for each m= line */
	for (i = 0; !sdp_message_endof_media (remote, i); i++)
	{
		sdp_payload_init(&payload);
		mtype = sdp_message_m_media_get (remote, i);
		proto = sdp_message_m_proto_get (remote, i);
		port = sdp_message_m_port_get (remote, i);
		payload.remoteport = osip_atoi (port);
		payload.localport = osip_atoi (sdp_message_m_port_get (ctx->offer, i));
		payload.proto = proto;
		payload.line = i;
		payload.c_addr = sdp_message_c_addr_get (remote, i, 0);
		if (payload.c_addr == NULL)
			payload.c_addr = sdp_message_c_addr_get (remote, -1, 0);
		bw=sdp_message_b_bandwidth_get(remote, i, 0);
		if (bw!=NULL) payload.b_as_bandwidth=atoi(bw);
		if (keywordcmp ("audio", mtype) == 0)
		{
			if (sdph->get_audio_codecs != NULL)
			{
				/* for each payload type */
				for (j = 0;
				     ((pt =
				       sdp_message_m_payload_get (remote, i,
							  j)) != NULL); j++)
				{
					payload.pt = osip_atoi (pt);
					/* get the rtpmap associated to this codec, if any */
					payload.a_rtpmap =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "rtpmap");
					/* get the fmtp, if any */
					payload.a_fmtp =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "fmtp");
					/* ask the application if this codec is supported */
					err = sdph->get_audio_codecs (ctx,
								      &payload);
				}
			}
		}
		else if (keywordcmp ("video", mtype) == 0)
		{
			if (sdph->accept_video_codecs != NULL)
			{
				/* for each payload type */
				for (j = 0;
				     ((pt =
				       sdp_message_m_payload_get (remote, i,
							  j)) != NULL); j++)
				{
					payload.pt = osip_atoi (pt);
					/* get the rtpmap associated to this codec, if any */
					payload.a_rtpmap =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "rtpmap");
					/* get the fmtp, if any */
					payload.a_fmtp =
						sdp_message_a_attr_value_get_with_pt
						(remote, i, payload.pt,
						 "fmtp");
					/* ask the application if this codec is supported */
					err = sdph->get_video_codecs (ctx,
									 &payload);
				}
			}
		}
	}
}
void sdp_context_free(sdp_context_t *ctx){
	osip_free(ctx->localip);
	osip_free(ctx->username);
	if (ctx->offer!=NULL) sdp_message_free(ctx->offer);
	if (ctx->remote!=NULL) sdp_message_free(ctx->remote);
	if (ctx->answer!=NULL) sdp_message_free(ctx->answer);
	if (ctx->offerstr!=NULL) osip_free(ctx->offerstr);
	if (ctx->answerstr!=NULL) osip_free(ctx->answerstr);
	osip_free(ctx);
}
