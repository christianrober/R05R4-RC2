/***************************************************************************
                          linphone.c  -  Main code for linphone's gnome 
										  interface
                             -------------------
    begin                : Mon Dec 17 2001
    copyright            : (C) 2001 by Simon Morlat
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


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include "linphone.h"
#include "support.h"
#include "interface.h"
#include "callbacks.h"
#include "gui_utils.h"

LinphoneGnomeUI *uiobj=NULL;
#define get_uiobj() (uiobj)
#define get_core() (uiobj->core)
#define get_friend_list() (&uiobj->main_window.friendlist)

void linphone_gnome_ui_init(LinphoneGnomeUI *ui,LinphoneCore *core)
{
	gdk_threads_init();
	memset(ui,0,sizeof(LinphoneGnomeUI));
	ui->core=core;
	uiobj=ui;
	ui->main_window.shown_once=FALSE;
}

void main_window_create(LinphoneGnomeUI *obj)
{
	GtkWidget *child;
	GtkWidget *window;
	window=create_app1 ();
	gnome_window_icon_set_from_default(GTK_WINDOW(window));
	obj->main_window.status_bar=lookup_widget(window,"appbar1");
	obj->main_window.addressentry=lookup_widget(window,"addressentry");
	obj->main_window.optioncontrols=lookup_widget(window,"optioncontrols");
	obj->main_window.dtmfentry=lookup_widget(window,"dtmf_entry");
	obj->main_window.callbutton=lookup_widget(window,"callbutton");
	child=lookup_widget(window,"showmore");
	/* hide the optionnal controls at startup */
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(child),0);
	presence_box_init(&obj->main_window.presencebox,window,obj->core);
	friend_list_init(&obj->main_window.friendlist,obj->core,window);
	g_object_set_data(G_OBJECT(window),"ui",(gpointer)obj);
	obj->main_window.window=window;
}

void linphone_gnome_ui_show(LinphoneGnomeUI *ui)
{
	if (ui->main_window.window==NULL){
		main_window_create(ui);
		
	}
	gtk_widget_show(ui->main_window.window);
	ui->main_window.shown_once=TRUE;
}

void linphone_gnome_ui_hide(LinphoneGnomeUI *ui)
{
	if (ui->main_window.window==NULL) return;
	gtk_widget_hide(ui->main_window.window);
}


void linphone_gnome_ui_uninit(LinphoneGnomeUI *ui)
{
	ui->main_window.window=NULL;
}

void linphone_gnome_ui_display_something(LinphoneGnomeUI *ui,GtkMessageType type,const gchar *message)
{
	GtkWidget *dialog;
	
	if (!linphone_core_is_in_main_thread(ui->core)) gdk_threads_enter();
	linphone_gnome_ui_show(ui);
	if (type==GTK_MESSAGE_QUESTION)
	{
#ifdef VINCENT_MAURY_RSVP
		/* draw a question box. link to dialog_click callback */
		dialog = gtk_message_dialog_new (
				GTK_WINDOW(ui->main_window.window),
                                GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION,
                                GTK_BUTTONS_YES_NO,
				(const gchar*)message);
		/* connect the click event to the callback */
		g_signal_connect_swapped (G_OBJECT (dialog), "response",
                           G_CALLBACK (dialog_click),
                           G_OBJECT (dialog));
		/* actually show the box */
		gtk_widget_show(dialog);
#endif
	}
	else
	{
		dialog = gtk_message_dialog_new (GTK_WINDOW(ui->main_window.window),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  type,
                                  GTK_BUTTONS_CLOSE,
                                  (const gchar*)message);
		/* Destroy the dialog when the user responds to it (e.g. clicks a button) */
		g_signal_connect_swapped (G_OBJECT (dialog), "response",
                           G_CALLBACK (gtk_widget_destroy),
                           G_OBJECT (dialog));
		gtk_widget_show(dialog);
	}
	if (!linphone_core_is_in_main_thread(ui->core)) gdk_threads_leave();
}

/* these are the LinphoneCore virtual functions */
void linphone_gnome_display_message(LinphoneCore *lc,char *message)
{
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	linphone_gnome_ui_display_something(ui,GTK_MESSAGE_INFO,message);
}

#ifdef VINCENT_MAURY_RSVP
/* Question box with yes/no answer. */
void linphone_gnome_display_yes_no(LinphoneCore *lc,char *message)
{
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	if (strcmp(message,"With QoS")==0)
		/* the caller asks for QoS, this function is called because, by default,
		 * you don't use QoS ! */
		linphone_gnome_ui_display_something(ui,GTK_MESSAGE_QUESTION,
				_("The caller asks for resource reservation. Do you agree ?"));
	else
		linphone_gnome_ui_display_something(ui,GTK_MESSAGE_QUESTION,
				_("The caller doesn't use resource reservation. \
					Do you wish to continue anyway ?"));
}
#endif

void linphone_gnome_display_warning(LinphoneCore *lc,char *warning)
{
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	linphone_gnome_ui_display_something(ui,GTK_MESSAGE_WARNING,warning);
}

void linphone_gnome_display_status(LinphoneCore *lc,char *status)
{
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	if (ui->main_window.window==NULL) return;
	if (!ui->main_window.shown_once) return; /* avoid a gnome bug*/
	gnome_appbar_push(GNOME_APPBAR(ui->main_window.status_bar),status);
	
}

void linphone_gnome_inv_recv(LinphoneCore *lc,char *from)
{
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	gchar *title;
	if (ui->main_window.window==NULL) return;
	gtk_entry_set_text(GTK_ENTRY(gnome_entry_gtk_entry(GNOME_ENTRY(ui->main_window.addressentry))),from);
	title=g_strdup_printf(_("linphone - receiving call from %s"),from);
	gtk_window_set_title(GTK_WINDOW(ui->main_window.window),title);
	g_free(title);
}

void linphone_gnome_show(LinphoneCore *lc)
{
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	linphone_gnome_ui_show(ui);
}

void linphone_gnome_display_url(LinphoneCore *lc, char *message, char *url)
{
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	if (!linphone_core_is_in_main_thread(ui->core)) gdk_threads_enter();
	alt_ressource_display(ui,url);
	if (!linphone_core_is_in_main_thread(ui->core)) gdk_threads_leave();
}



void linphone_gnome_notify_received(LinphoneCore *lc,LinphoneFriend *fid, const char *from, const char *status, const char *img){
	FriendList *fl=get_friend_list();
	friend_list_set_friend_status(fl,fid,from,status, img);
}

void linphone_gnome_new_unknown_subscriber(LinphoneCore *lc, LinphoneFriend *lf, const char *url){
	GtkWidget *d=create_inc_subscr_dialog();
	gchar *text=g_strdup_printf(_("You have received a subscription from %s."
		"This means that this person wishes to be notified of your presence information (online, busy, away...).\n"
		"Do you agree ?"),url);
	gtk_label_set_text(GTK_LABEL(lookup_widget(d,"subscr_label")),text);
	g_object_set_data(G_OBJECT(d),"friend_ref",(gpointer)lf);
	gtk_widget_show(d);
}

void linphone_gnome_prompt_authentication(LinphoneCore *lc, const gchar *realm, const gchar *username){
	GtkWidget *w=create_authentication_dialog();
	gchar *question=g_strdup_printf(_("Authentication required for realm %s"),realm);
	gtk_label_set_text(GTK_LABEL(lookup_widget(w,"question")),question);
	g_free(question);
	gtk_entry_set_text(GTK_ENTRY(lookup_widget(w,"realm")),realm);
	gtk_entry_set_text(GTK_ENTRY(lookup_widget(w,"username")),username);
	gtk_widget_show(w);
}

void linphone_gnome_bye_recv(LinphoneCore *lc, const char *from){
	LinphoneGnomeUI *ui=(LinphoneGnomeUI*)lc->data;
	gtk_window_set_title(GTK_WINDOW(ui->main_window.window),"linphone");
}

void stub(){
}

void linphone_gnome_call_log_updated(LinphoneCore *lc, LinphoneCallLog *newcl){
	LinphoneGnomeUI *ui=(LinphoneGnomeUI *)linphone_core_get_user_data(lc);
	linphone_gnome_update_call_logs(ui);
}

void linphone_gnome_text_received(LinphoneCore *lc,LinphoneChatRoom *cr, const char *from, const char *msg){
	GtkWidget *gcr=(GtkWidget*)linphone_chat_room_get_user_data(cr);
	if (gcr==NULL){
		gcr=chatroom_new(from,cr);
	}
	gtk_widget_show(gcr);
	chatroom_append(gcr,from,msg);
}

LinphoneCoreVTable linphone_gnome_vtable=
{
	show: linphone_gnome_show,
	inv_recv: linphone_gnome_inv_recv,
	bye_recv : linphone_gnome_bye_recv,
	notify_recv: linphone_gnome_notify_received,
	new_unknown_subscriber: linphone_gnome_new_unknown_subscriber,
	auth_info_requested: linphone_gnome_prompt_authentication,
	display_status : linphone_gnome_display_status,
	display_message : linphone_gnome_display_message,
	display_warning : linphone_gnome_display_warning,
#ifdef VINCENT_MAURY_RSVP
	display_yes_no : linphone_gnome_display_yes_no,
#endif
	display_url : linphone_gnome_display_url,
	display_question : stub,
	call_log_updated : linphone_gnome_call_log_updated,
	text_received: linphone_gnome_text_received
};

gboolean linphone_gnome_iterate(LinphoneCore *lc)
{
	linphone_core_iterate(lc);
	return TRUE;
}

void proxy_changed(GtkWidget *combo){
	LinphoneProxyConfig *pcfg=proxy_combo_box_get_selected(combo);
	linphone_core_set_default_proxy(get_core(),pcfg);
}

void linphone_refresh_proxy_combo_box(GtkWidget *window){
	LinphoneCore *lc=get_core();
	GtkWidget *combo;
	GList *elem=linphone_core_get_proxy_config_list(lc);
	LinphoneProxyConfig *cfg=NULL;
	GtkWidget *hbox=lookup_widget(window,"proxy_hbox");
	
	linphone_core_get_default_proxy(lc,&cfg);
	
	if (elem==NULL){
		gtk_widget_hide(hbox);
		return;
	}
	combo=(GtkWidget*)g_object_get_data(G_OBJECT(hbox),"proxy");
	if (combo!=NULL){
		gtk_widget_destroy(combo);
	}
	combo=proxy_combo_box_new(cfg);
	g_object_set_data(G_OBJECT(hbox),"proxy",(gpointer)combo);
	g_signal_connect(G_OBJECT(combo),"changed",G_CALLBACK(proxy_changed),NULL);
	gtk_box_pack_start_defaults(GTK_BOX(hbox),combo);
	gtk_widget_show(combo);
	gtk_widget_show(hbox);
	
}

void linphone_gnome_init(LinphoneGnomeUI *ui,LinphoneCore *lc)
{
	gchar *configfile_name =
			g_strdup_printf ("%s/.gnome2/linphone", getenv ("HOME"));
	linphone_gnome_ui_init(ui,lc);
	linphone_gnome_ui_show(ui);
	linphone_core_init(lc,&linphone_gnome_vtable,configfile_name,(gpointer)ui);
	g_free(configfile_name);
	set_levels(ui,linphone_core_get_rec_level(lc),linphone_core_get_play_level(lc));
	linphone_refresh_proxy_combo_box(ui->main_window.window);
	ui->timeout_id=gtk_timeout_add(500,(GtkFunction)linphone_gnome_iterate,(gpointer)lc);
}

void linphone_gnome_uninit(LinphoneGnomeUI *ui)
{
	LinphoneCore *lc=ui->core;
	linphone_gnome_ui_uninit(ui);
	linphone_core_uninit(lc);
	gtk_timeout_remove (ui->timeout_id);
}

GtkWidget *proxy_combo_box_new(LinphoneProxyConfig *selected){
	GtkWidget *combo;
	GList *elem;
	GtkListStore *store=gtk_list_store_new(2,G_TYPE_STRING,G_TYPE_POINTER);
	GtkTreeIter iter;
	GtkTreeIter prxiter;
	GtkCellRenderer *renderer;
	gboolean proxy_found=FALSE;
	/* fill the store */
	elem=linphone_core_get_proxy_config_list(get_core());
	gtk_list_store_append(store,&iter);
	gtk_list_store_set(store,&iter,0,_("None"),1,(gpointer)NULL,-1);
	for(;elem!=NULL;elem=g_list_next(elem)){
		LinphoneProxyConfig *proxy=(LinphoneProxyConfig*)elem->data;
		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,0,proxy->reg_proxy,1,(gpointer)proxy,-1);
		if (selected==proxy) {
			prxiter=iter;
			proxy_found=TRUE;
		}
	}
	combo=gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));
	renderer = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo), renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo), renderer,
                                "text", 0,
                                NULL);
	if (proxy_found){
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo),&prxiter);
	}else{
		/*else select "None" */
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo),0);
	}
	return combo;
}

LinphoneProxyConfig *proxy_combo_box_get_selected(GtkWidget *combo){
	LinphoneProxyConfig *pcfg=NULL;
	GtkTreeIter iter;
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo),&iter)){
		GtkTreeModel *model=gtk_combo_box_get_model(GTK_COMBO_BOX(combo));
		gtk_tree_model_get(model,&iter,1,(gpointer)&pcfg,-1);
	}
	return pcfg;
}

void linphone_gnome_update_call_logs(LinphoneGnomeUI *ui){
	LinphoneCore *lc=ui->core;
	GtkTextView *tv;
	GtkTextBuffer *tb;
	GtkTextIter begin,end;
	GtkTextTag *tag;
	GList *elem;
	if (ui->logs==NULL) return;
	tv=GTK_TEXT_VIEW(lookup_widget(ui->logs,"logview"));
	tb=gtk_text_view_get_buffer(tv);
	
	gtk_text_buffer_get_bounds(tb,&begin,&end);
	gtk_text_buffer_delete(tb,&begin,&end);
	gtk_text_buffer_get_end_iter(tb,&end);
	for (elem=linphone_core_get_call_logs(lc);elem!=NULL;elem=g_list_next(elem)){
		LinphoneCallLog *cl=(LinphoneCallLog*)elem->data;
		gchar *str=linphone_call_log_to_str(cl);
		tag=NULL;
		if (cl->status==LinphoneCallMissed){
			tag=gtk_text_tag_table_lookup(gtk_text_buffer_get_tag_table(tb),"redforeground");
			if (tag==NULL) tag = gtk_text_buffer_create_tag (tb, "redforeground",
	   		            "foreground", "red", NULL);
		}
		gtk_text_buffer_insert_with_tags(tb,&end,str,-1,tag,NULL);
		gtk_text_buffer_insert(tb,&end,"\n",-1);
		
		g_free(str);
	}
}

void linphone_gnome_show_call_logs_window(LinphoneGnomeUI *ui){
	if (ui->logs==NULL) {
		ui->logs=create_call_logs();
	}
	linphone_gnome_update_call_logs(ui);
	gtk_widget_show(ui->logs);
}
