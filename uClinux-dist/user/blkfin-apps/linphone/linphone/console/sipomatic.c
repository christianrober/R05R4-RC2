/***************************************************************************
                          linphone  - sipomatic.c
This is a test program for linphone. It acts as a sip server and answers to linphone's
call.
                             -------------------
    begin                : ven mar  30
    copyright            : (C) 2001 by Simon MORLAT
    email                : simon.morlat@linphone.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <signal.h>
#include "sipomatic.h"
#include <eXosip.h>


int run_cond=1;

Sipomatic sipomatic;

int sipomatic_accept_audio_offer(sdp_context_t *ctx,sdp_payload_t *payload);

sdp_handler_t sipomatic_sdp_handler={
	sipomatic_accept_audio_offer,   /*from remote sdp */
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

void stop_handler(int signum)
{
	run_cond=0;
}

void sipomatic_process_event(Sipomatic *obj,eXosip_event_t *ev)
{
	Call *call;
	switch(ev->type){
		case EXOSIP_CALL_NEW:
			call_new(obj,ev->did,ev->sdp_body);
			break;
		case EXOSIP_CALL_CLOSED:
		case EXOSIP_CALL_CANCELLED:
			call=sipomatic_find_call(obj,ev->did);
			if (call==NULL){
				g_warning("Could not find call with did %i !",ev->did);
			}
			call_release(call);
			call_destroy(call);
			break;
		case EXOSIP_IN_SUBSCRIPTION_NEW:
			g_message("Receving new incoming subscription.");
			eXosip_notify_accept_subscribe(ev->did,200,EXOSIP_SUBCRSTATE_ACTIVE,EXOSIP_NOTIFY_ONLINE);
			break;
		default:
			break;
	}
	eXosip_event_free(ev);
}


gint endoffile_cb(MSFilter *f,gint ev,gpointer arg,gpointer data)
{
	Call*call=(Call*)data;
	call->eof=1;
	return 0;
}

void call_accept(Call *call)
{
	sdp_context_t *ctx;
	PayloadType *payload;
	gchar *hellofile;
	
	ctx=call->sdpc;
	payload=rtp_profile_get_payload(call->profile,call->pt);
	if (strcmp(payload->mime_type,"telephone-event")==0){
		/* telephone-event is not enough to accept a call */
		g_message("Cannot accept call with only telephone-event.\n");
		eXosip_answer_call(call->did,415,NULL);
		call->state=CALL_STATE_FINISHED;
		return;
	}
	if (payload->clock_rate==16000){
		hellofile=call->root->file_path16000hz;
	}else hellofile=call->root->file_path8000hz;
	eXosip_answer_call_with_body(call->did,200,"application/sdp",call->sdpc->answerstr);
 	call->audio_stream=audio_stream_start_with_files(call->profile,call->localport,
				call->remaddr,call->remoteport,call->pt,20,hellofile,"/dev/null");
	g_timer_reset(call->timer);
	call->state=CALL_STATE_RUNNING;
	ms_filter_set_notify_func(call->audio_stream->soundread,endoffile_cb,(gpointer)call);
}


PayloadType * sipomatic_payload_is_supported(sdp_payload_t *payload,RtpProfile *local_profile,RtpProfile *dialog_profile)
{
	int localpt;
	if (payload->a_rtpmap!=NULL){
		localpt=rtp_profile_get_payload_number_from_rtpmap(local_profile,payload->a_rtpmap);
	}else{
		localpt=payload->pt;
		g_warning("payload has no rtpmap.");
	}
	
	if (localpt>=0){
		MSCodecInfo *info;
		/* this payload is supported in our local rtp profile, so add it to the dialog rtp
		profile */
		PayloadType *rtppayload;
		rtppayload=rtp_profile_get_payload(local_profile,localpt);
		if (rtppayload==NULL) return NULL;
		/*check if we have the appropriate coder/decoder for this payload */
		if (strcmp(rtppayload->mime_type,"telephone-event")==0) return rtppayload;
		info=ms_audio_codec_info_get(rtppayload->mime_type);
		if (info==NULL) return NULL;
		rtppayload=payload_type_clone(rtppayload);
		rtp_profile_set_payload(dialog_profile,payload->pt,rtppayload);
		/* add to the rtp payload type some other parameters (bandwidth) */
		if (payload->b_as_bandwidth!=0) rtppayload->normal_bitrate=payload->b_as_bandwidth*1000;
		if (payload->a_fmtp!=NULL) rtppayload->fmtp=g_strdup(payload->a_fmtp);
		else if (strcasecmp(rtppayload->mime_type,"iLBC")==0){
			/*default to 30 ms mode */
			payload->a_fmtp="ptime=30";
			rtppayload->fmtp=g_strdup(payload->a_fmtp);
		}
		return rtppayload;
	}
	return NULL;
}
int sipomatic_accept_audio_offer(sdp_context_t *ctx,sdp_payload_t *payload)
{
	static int audioport=8000;
	Call *call=(Call*)sdp_context_get_user_pointer(ctx);
	PayloadType *supported;
	
	
	/* see if this codec is supported in our local rtp profile*/
	supported=sipomatic_payload_is_supported(payload,&av_profile,call->profile);
	if (supported==NULL) {
		g_message("Refusing codec %i (%s)",payload->pt,payload->a_rtpmap);
		return -1;
	}
	if (strcmp(supported->mime_type,"telephone-event")==0) return 0;
	if (call->ncodecs==0 ){
		/* this is the first codec we may accept*/
		call->localport=payload->localport=audioport;
		call->remoteport=payload->remoteport;
		call->line=payload->line;
		call->pt=payload->pt; /* remember the first payload accepted */
		call->remaddr=payload->c_addr;
		call->ncodecs++;
		audioport+=4;
	}else{
		/* refuse all other audio lines*/
		if(call->line!=payload->line) return -1;
	}
	return 0;
}

void sipomatic_init(Sipomatic *obj, gchar *url, gboolean ipv6)
{
	osip_uri_t *uri=NULL;
	int port=5064;
	
	eXosip_enable_ipv6(ipv6);
	
	if (url==NULL){
		url=getenv("SIPOMATIC_URL");
		if (url==NULL){
			if (ipv6) url="sip:robot@[::1]:5064";
			else url="sip:robot@127.0.0.1:5064";
		}
	}
	if (url!=NULL) {
		osip_uri_init(&uri);
		if (osip_uri_parse(uri,url)==0){
			if (uri->port!=NULL) port=atoi(uri->port);
		}else{
			g_warning("Invalid identity uri:%s",url);
		}	
	}
	g_message("Starting using url %s",url);
	obj->lock=g_mutex_new();
	obj->calls=NULL;
	obj->acceptance_time=5;
	obj->max_call_time=300;
	obj->file_path8000hz=g_strdup_printf("%s/%s",PACKAGE_SOUND_DIR,ANNOUCE_FILE8000HZ);
	obj->file_path16000hz=g_strdup_printf("%s/%s",PACKAGE_SOUND_DIR,ANNOUCE_FILE16000HZ);
	osip_trace_initialize(OSIP_INFO1,stdout);
	osip_trace_initialize(OSIP_INFO2,stdout);
	osip_trace_initialize(OSIP_WARNING,stdout);
	osip_trace_initialize(OSIP_ERROR,stdout);
	osip_trace_initialize(OSIP_BUG,stdout);
	osip_trace_initialize(OSIP_FATAL,stdout);
	osip_trace_enable_level(OSIP_INFO1);
	osip_trace_enable_level(OSIP_INFO2);
	osip_trace_enable_level(OSIP_WARNING);
	osip_trace_enable_level(OSIP_ERROR);
	osip_trace_enable_level(OSIP_BUG);
	osip_trace_enable_level(OSIP_FATAL);
	eXosip_init(NULL,stdout,port);
	eXosip_set_user_agent("sipomatic-" LINPHONE_VERSION "/eXosip");
}

void sipomatic_uninit(Sipomatic *obj)
{
	g_mutex_free(obj->lock);
	eXosip_quit();
}

void sipomatic_iterate(Sipomatic *obj)
{
	GList *elem;
	GList *to_be_destroyed=NULL;
	Call *call;
	gdouble time;
	eXosip_event_t *ev;

	while((ev=eXosip_event_wait(0,0))!=NULL){
		sipomatic_process_event(obj,ev);
	}
	elem=obj->calls;
	while(elem!=NULL){
		call=(Call*)elem->data;
		time=g_timer_elapsed(call->timer,NULL);
		switch(call->state){
			case CALL_STATE_INIT:
				if (time>obj->acceptance_time){
					call_accept(call);
				}
			break;
			case CALL_STATE_RUNNING:
				if (time>obj->max_call_time || call->eof){
					call_release(call);
					to_be_destroyed=g_list_append(to_be_destroyed,call);
				}
			break;
		}
		elem=g_list_next(elem);
	}
	for(;to_be_destroyed!=NULL; to_be_destroyed=g_list_next(to_be_destroyed)){
		call_destroy((Call*)to_be_destroyed->data);
	}
}


Call* sipomatic_find_call(Sipomatic *obj,int did)
{
	GList *it;
	Call *call=NULL;
	for (it=obj->calls;it!=NULL;it=g_list_next(it)){
		call=(Call*)it->data;
		if ( call->did==did) return call;
	}
	return call;
}


Call * call_new(Sipomatic *root, int did, char *sdp)
{
	Call *obj;
	char *sdpans;
	int status;
	sdp_context_t *sdpc=sdp_handler_create_context(&sipomatic_sdp_handler,NULL,"sipomatic");
	obj=g_new0(Call,1);
	obj->profile=rtp_profile_new("remote");
	eXosip_answer_call(did,100,NULL);
	sdp_context_set_user_pointer(sdpc,obj);
	sdpans=sdp_context_get_answer(sdpc,sdp);
	if (sdpans!=NULL){
		eXosip_answer_call(did,180,NULL);
		
	}else{
		status=sdp_context_get_status(sdpc);
		eXosip_answer_call(did,status,NULL);
		sdp_context_free(sdpc);
		rtp_profile_destroy(obj->profile);
		g_free(obj);
	}
	obj->sdpc=sdpc;
	obj->timer=g_timer_new();
	obj->did=did;
	g_timer_start(obj->timer);
	obj->audio_stream=NULL;
	obj->state=CALL_STATE_INIT;
	obj->eof=0;
	obj->root=root;
	root->calls=g_list_append(root->calls,obj);
	return obj;
}

void call_release(Call *call)
{
	eXosip_terminate_call(0,call->did);
	if (call->audio_stream!=NULL) audio_stream_stop(call->audio_stream);
	call->state=CALL_STATE_FINISHED;
}

void call_destroy(Call *obj)
{
	obj->root->calls=g_list_remove(obj->root->calls,(gpointer)obj);
	rtp_profile_destroy(obj->profile);
	g_timer_destroy(obj->timer);
	sdp_context_free(obj->sdpc);
	g_free(obj);
}

void sipomatic_set_annouce_file(Sipomatic *obj, char *file)
{
	if (obj->file_path8000hz!=NULL){
		g_free(obj->file_path8000hz);
	}
	obj->file_path8000hz=g_strdup(file);
}


void display_help()
{
	printf("sipomatic [-u sip-url] [-f annouce-file ] [-s port]\n"
			"sipomatic -h or --help: display this help.\n"
			"sipomatic -v or --version: display version information.\n"
			"	-u sip-url : specify the sip url sipomatic listens and answers.\n"
			"	-f annouce-file : set the annouce file (16 bit raw format,8000Hz)\n"
			" -6 enable ipv6 network usage\n");
	exit(0);
}

char *getarg(int argc, char*argv[], int i)
{
	if (i<argc){
		return argv[i];
	}
	else display_help();
	return NULL;
}

int main(int argc, char *argv[])
{
	int sendport=5070;
	char *file=NULL;
	gchar *url=NULL;
	gboolean ipv6=FALSE;
	int i;
	
	for(i=1;i<argc;i++){
		if ( (strcmp(argv[i],"-h")==0) || (strcmp(argv[i],"--help")==0) ){
			display_help();
			continue;
		}
		if ( (strcmp(argv[i],"-v")==0) || (strcmp(argv[i],"--version")==0) ){
			printf("version: " LINPHONE_VERSION "\n");
			exit(0);
		}
		if (strcmp(argv[i],"-u")==0){
			i++;
			url=getarg(argc,argv,i);
			continue;
		}
		if (strcmp(argv[i],"-s")==0){
			char *port;
			i++;
			port=getarg(argc,argv,i);
			sendport=atoi(port);
			continue;
		}
		if (strcmp(argv[i],"-f")==0){
			i++;
			file=getarg(argc,argv,i);
			continue;
		}
		if (strcmp(argv[i],"-6")==0){
			ipv6=TRUE;
			continue;
		}
	}
	
	signal(SIGINT,stop_handler);
	ms_init();
	ms_speex_codec_init();
	ortp_init();
	ortp_set_debug_file("oRTP",NULL);
	rtp_profile_set_payload(&av_profile,115,&lpc1015);
	rtp_profile_set_payload(&av_profile,110,&speex_nb);
	rtp_profile_set_payload(&av_profile,111,&speex_wb);
	rtp_profile_set_payload(&av_profile,112,&payload_type_ilbc);
	rtp_profile_set_payload(&av_profile,101,&telephone_event);
    rtp_profile_set_payload(&av_profile,116,&truespeech);
	
	sipomatic_init(&sipomatic,url,ipv6);
	if (file!=NULL) sipomatic_set_annouce_file(&sipomatic,file);
	
	while (run_cond){
		sipomatic_iterate(&sipomatic);
		usleep(20000);
	}
	
	return(0);
}
