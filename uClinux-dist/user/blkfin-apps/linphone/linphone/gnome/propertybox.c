/***************************************************************************
                          propertybox.c  -  description
                             -------------------
    begin                : Wed Jan 30 2002
    copyright            : (C) 2002 by Simon Morlat
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


#include "linphone.h"

enum{
#ifdef INET6
	IFACE_INDEX,
#endif
	IFACE_NAME,
	IFACE_ADDR,
	IFACE_NCOLUMNS
};

enum {
	CODEC_NAME,
	CODEC_RATE,
	CODEC_BITRATE,
	CODEC_STATUS,
	CODEC_PRIVDATA,
	CODEC_COLOR,
	CODEC_NCOLUMNS
};

#define get_core()			(uiobj->core)
#define get_property_box()	(&uiobj->propbox)
#define property_box_changed() 	gnome_property_box_changed(GNOME_PROPERTY_BOX ( (&uiobj->propbox)->prop))
#define get_main_window() (&uiobj->main_window)


void net_section_init(NetSection *sec, GtkWidget *prop)
{
	sec->au_port=lookup_widget(prop,"audioport");
	sec->audio_jittcomp=lookup_widget(prop,"audio_jittcomp");
#ifdef LINPHONE_DEPRECATED
	sec->interfaces=lookup_widget(prop, "interfaces");	
#endif
	sec->enable_nat=lookup_widget(prop,"enable_nat");
	sec->nat_address=lookup_widget(prop,"nat_address");
	sec->nat_label=lookup_widget(prop,"nat_label");
	sec->use_sipinfo=lookup_widget(prop,"use_sipinfo");
	sec->enable_ipv6=lookup_widget(prop,"enable_ipv6");
}

void codec_list_update(GtkTreeView *listview,double bandwidth);

void on_connection_type_selected(CodecSection *sec, gint type)
{
	/* we must call linphone_core so that codec usability is checked */
	linphone_core_set_connection_type(get_core(),type);
	codec_list_update(GTK_TREE_VIEW(sec->au_codec_list),bandwidths[type]);
	sec->type_sel=type;
	property_box_changed();
}

void on_28k_selected(GtkMenuItem *menuitem, gpointer user_data)
{
	on_connection_type_selected((CodecSection*)user_data , 0);
}

void on_56k_selected(GtkMenuItem *menuitem, gpointer user_data)
{
	on_connection_type_selected((CodecSection*)user_data , 1);
}

void on_128k_selected(GtkMenuItem *menuitem, gpointer user_data)
{
	on_connection_type_selected((CodecSection*)user_data , 2);
}

void on_adsl_selected(GtkMenuItem *menuitem, gpointer user_data)
{
	on_connection_type_selected((CodecSection*)user_data , 3);
}

void on_ethernet_selected(GtkMenuItem *menuitem, gpointer user_data)
{
	on_connection_type_selected((CodecSection*)user_data , 4);
}

void net_selection_changed_cb(GtkTreeSelection *select, gpointer userdata)
{
	property_box_changed();
}

void net_section_fill(NetSection *sec,LinphoneCore *lc)
{
	gfloat value;
	gboolean use_nat;
	const gchar *nat_address;
	
	value=(gfloat)linphone_core_get_audio_jittcomp(lc);
	/* put the current value of jitt_comp*/
	gtk_adjustment_set_value (gtk_range_get_adjustment(GTK_RANGE(sec->audio_jittcomp)),value);
	/* display current rtp port */
  	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sec->au_port),
						(gfloat)linphone_core_get_audio_port(lc));
	
#ifdef LINPHONE_DEPRECATED
	store= gtk_list_store_new (IFACE_NCOLUMNS, G_TYPE_INT, G_TYPE_STRING,G_TYPE_STRING);
	/* draw the list of interface */
	elem=linphone_core_get_net_interfaces(lc);
	while(elem!=NULL){
		interface_t *iface=(interface_t *)elem->data;
		/* get an iterator */
		gtk_list_store_append(store,&iter);
		if (iface==linphone_core_get_selected_interface(lc)){
			selected=iter;
		}
#ifdef INET6
		gtk_list_store_set(store,&iter, IFACE_INDEX, iface->index, IFACE_NAME,iface->name,IFACE_ADDR,iface->ipaddr,-1);
#else
		gtk_list_store_set(store,&iter,IFACE_NAME,iface->name,IFACE_ADDR,iface->ip4addr,-1);
#endif
		elem=g_list_next(elem);
	}
	gtk_tree_view_set_model(GTK_TREE_VIEW(sec->interfaces),GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));
	renderer = gtk_cell_renderer_text_new ();
#ifdef INET6
	column = gtk_tree_view_column_new_with_attributes (_("Index"),
                                                   renderer,
                                                   "text", IFACE_INDEX,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (sec->interfaces), column);
#endif
	column = gtk_tree_view_column_new_with_attributes (_("Name"),
                                                   renderer,
                                                   "text", IFACE_NAME,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (sec->interfaces), column);
	column = gtk_tree_view_column_new_with_attributes (_("Address"),
                                                   renderer,
                                                   "text", IFACE_ADDR,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (sec->interfaces), column);
	/* Setup the selection handler */
	select = gtk_tree_view_get_selection (GTK_TREE_VIEW (sec->interfaces));
	gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
	gtk_tree_selection_select_iter(select,&selected);
	g_signal_connect (G_OBJECT (select), "changed",
                  G_CALLBACK (net_selection_changed_cb),
                  NULL);
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW (sec->interfaces));
#endif
	
	/* nat setup */
	nat_address=linphone_core_get_nat_address(lc,&use_nat);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sec->enable_nat),use_nat);
	if (nat_address!=NULL) {
		gtk_entry_set_text(GTK_ENTRY(sec->nat_address),nat_address);
	}
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sec->use_sipinfo),
		linphone_core_get_use_info_for_dtmf(lc));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sec->enable_ipv6),
		linphone_core_ipv6_enabled(lc));
}

void
on_enable_nat_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	LinphonePropertyBox *prop=get_property_box();
	NetSection *sec=&prop->net;
	gboolean use_nat;
	use_nat=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sec->enable_nat));
	gtk_widget_set_sensitive(sec->nat_label,use_nat);
	gtk_widget_set_sensitive(sec->nat_address,use_nat);
	property_box_changed();
	
}



void
on_nat_address_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
	property_box_changed();
}

void
on_enable_ipv6_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	property_box_changed();
}


void net_section_apply(NetSection *sec,LinphoneCore *lc)
{
	gboolean use_nat;
	gchar *name;
#ifdef LINPHONE_DEPRECATED
#ifdef INET6
 	gint index;  	
#endif
	/* get network interface*/
	select = gtk_tree_view_get_selection (GTK_TREE_VIEW (sec->interfaces));
	if (gtk_tree_selection_get_selected (select, &model, &iter))
	{
#ifdef INET6
 		gtk_tree_model_get (model, &iter, IFACE_INDEX , &index, -1);
 		linphone_core_select_net_interface(lc,index);
#else
		gtk_tree_model_get (model, &iter,IFACE_NAME , &name, -1);
		linphone_core_select_net_interface(lc,name);
		g_free(name);
#endif
	}
	linphone_core_set_connection_type(lc,sec->type_sel);
#endif
	
	use_nat=gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sec->enable_nat));
	name=gtk_editable_get_chars(GTK_EDITABLE(sec->nat_address),0,-1);
	linphone_core_set_nat_address(lc,name,use_nat);
	
	{
	/* get the value of jitt_comp*/
	GtkAdjustment *adj=  gtk_range_get_adjustment(GTK_RANGE(sec->audio_jittcomp));
	linphone_core_set_audio_jittcomp(lc,(gint)adj->value);
	/* get  rtp port */
	adj=gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(sec->au_port));
  	linphone_core_set_audio_port(lc,(gint)adj->value);
	}
	linphone_core_enable_ipv6(lc,gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sec->enable_ipv6)));
}


enum{
	PROXY_CONFIG_SERVER,
	PROXY_CONFIG_REF,
	PROXY_CONFIG_NCOL
};

void sip_section_init(SipSection *sec,GtkWidget *prop)
{
	
	GtkListStore *store;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select;
	sec->port=lookup_widget(prop,"sip_port");
	sec->username=lookup_widget(prop, "user_name");
	sec->hostname=lookup_widget(prop,"domain_name");
	sec->proxy_list=lookup_widget(prop,"proxy_list");
	sec->guess_hostname=lookup_widget(prop,"guess_hostname");
	/* create the proxy list */
	store = gtk_list_store_new (PROXY_CONFIG_NCOL, G_TYPE_STRING, G_TYPE_POINTER);
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(sec->proxy_list),GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (_("Server address"),
                                                   renderer,
                                                   "text", PROXY_CONFIG_SERVER,
                                                   NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (sec->proxy_list), column);
	
	select = gtk_tree_view_get_selection (GTK_TREE_VIEW (sec->proxy_list));
	gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
}


void proxy_list_fill(GtkTreeModel *model, LinphoneCore *lc){
	GList *elem;
	GtkTreeIter iter;
	/* fill the proxy list */
	gtk_list_store_clear(GTK_LIST_STORE(model));
	elem=linphone_core_get_proxy_config_list(lc);
	for(;elem!=NULL;elem=g_list_next(elem)){
		LinphoneProxyConfig *pcfg=(LinphoneProxyConfig*)elem->data;
		gtk_list_store_append(GTK_LIST_STORE(model),&iter);
		gtk_list_store_set(GTK_LIST_STORE(model),&iter,PROXY_CONFIG_SERVER,pcfg->reg_proxy,
			PROXY_CONFIG_REF,pcfg,-1);
	}
}

void sip_section_fill(SipSection *sec, LinphoneCore *lc)
{
	osip_from_t *contact;
	GtkTreeModel *model=gtk_tree_view_get_model(GTK_TREE_VIEW(sec->proxy_list));
	
	/* set sip port */
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sec->port),
				(gfloat)linphone_core_get_sip_port(lc));
	contact=linphone_core_get_primary_contact_parsed(lc);
	g_return_if_fail(contact!=NULL);
	/* set sip username */
	gtk_entry_set_text(GTK_ENTRY(sec->username),contact->url->username);
	/* set domain name */
	gtk_entry_set_text(GTK_ENTRY(sec->hostname),contact->url->host);
	osip_from_free(contact);
	proxy_list_fill(model,lc);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sec->guess_hostname),
		linphone_core_get_guess_hostname(lc));
}

void sip_section_apply(SipSection *sec, LinphoneCore *lc)
{
	GtkAdjustment *adj;
	gchar *tmp;
	gchar *username,*hostname;
	/* get sip port*/
	adj=gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(sec->port));
	
	username=gtk_editable_get_chars(GTK_EDITABLE(sec->username),0,-1);
	if (username!=NULL && strlen(username)!=0)
	{
		hostname=gtk_editable_get_chars(GTK_EDITABLE(sec->hostname),0,-1);
		if (hostname!=NULL && strlen(hostname)!=0)
		{
			tmp=g_strdup_printf("sip:%s@%s",username,hostname);
			linphone_core_set_primary_contact(lc,tmp);
			linphone_core_set_sip_port(lc,(int)gtk_adjustment_get_value(adj));
			g_free(hostname);
			g_free(tmp);
		}
		g_free(username);
	}
}


void
on_addproxy_button_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *w=create_proxy_config_box();
	gtk_widget_show(w);
}


void
on_editproxy_button_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *w;
	GtkTreeSelection *select;
	GtkTreeIter iter;
	GtkTreeModel *model;
	LinphoneProxyConfig * cfg;
	select = gtk_tree_view_get_selection (GTK_TREE_VIEW (get_property_box()->sip.proxy_list));
	if (gtk_tree_selection_get_selected (select, &model, &iter))
	{
		gtk_tree_model_get (model, &iter,PROXY_CONFIG_REF , &cfg, -1);
		gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
		linphone_proxy_config_edit(cfg);
	}else return;
	w=create_proxy_config_box();
	gtk_widget_show(w);
	if (cfg->reg_proxy!=NULL) gtk_entry_set_text(GTK_ENTRY(lookup_widget(w,"reg_proxy")),cfg->reg_proxy);
	if (cfg->reg_route!=NULL) gtk_entry_set_text(GTK_ENTRY(lookup_widget(w,"reg_route")),cfg->reg_route);
	if (cfg->reg_identity!=NULL) gtk_entry_set_text(GTK_ENTRY(lookup_widget(w,"reg_identity")),cfg->reg_identity);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(w,"reg_sendregister")),cfg->reg_sendregister);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lookup_widget(w,"publish")),cfg->publish);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(lookup_widget(w,"reg_expires")),cfg->expires);
	g_object_set_data(G_OBJECT(w),"edited_config",(gpointer)cfg);
}


void
on_removeproxy_button_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkTreeSelection *select;
	GtkTreeIter iter;
	GtkTreeModel *model;
	LinphoneProxyConfig * cfg;
	select = gtk_tree_view_get_selection (GTK_TREE_VIEW (get_property_box()->sip.proxy_list));
	if (gtk_tree_selection_get_selected (select, &model, &iter))
	{
		gtk_tree_model_get (model, &iter,PROXY_CONFIG_REF , &cfg, -1);
		gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
		linphone_core_remove_proxy_config(get_core(),cfg);
		linphone_refresh_proxy_combo_box(get_main_window()->window);
	}
}

void get_proxy_config_box_data(GtkWidget *dialog)
{
	gchar *tmp;
	gboolean editing=FALSE;
	LinphoneProxyConfig *cfg;
	tmp=gtk_editable_get_chars(GTK_EDITABLE(lookup_widget(dialog,"reg_proxy")),0,-1);
	cfg=(LinphoneProxyConfig*)g_object_get_data(G_OBJECT(dialog),"edited_config");
	if (cfg==NULL){
		cfg=linphone_proxy_config_new(tmp);
		g_free(tmp);
		if (cfg==NULL) {
			/* set an error message here */
			return;
		}
	}else{
		linphone_proxy_config_set_server_addr(cfg,tmp);
		g_free(tmp);
		editing=TRUE;
	}
	tmp=gtk_editable_get_chars(GTK_EDITABLE(lookup_widget(dialog,"reg_route")),0,-1);
	linphone_proxy_config_set_route(cfg,tmp);
	g_free(tmp);
	tmp=gtk_editable_get_chars(GTK_EDITABLE(lookup_widget(dialog,"reg_identity")),0,-1);
	linphone_proxy_config_set_identity(cfg,tmp);
	g_free(tmp);
	tmp=gtk_editable_get_chars(GTK_EDITABLE(lookup_widget(dialog,"reg_expires")),0,-1);
	{
	  int exp;
	  if (tmp!=NULL)
	    {
	      exp = atoi(tmp);
	      if (exp<=0)
		exp = 200; /* minimum */
	      if (exp>7200)
		exp = 7200; /* maximum */
	      linphone_proxy_config_expires(cfg,exp);
	    }
	}
	g_free(tmp);
	linphone_proxy_config_enableregister(cfg,gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(dialog,"reg_sendregister"))));
	linphone_proxy_config_enable_publish(cfg,gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(dialog,"publish"))));
	if (editing) linphone_proxy_config_done(cfg);
	else linphone_core_add_proxy_config(get_core(),cfg);
	/* set the last entered/changed proxy as the default one */
	linphone_core_set_default_proxy(get_core(),cfg);
	proxy_list_fill(gtk_tree_view_get_model(GTK_TREE_VIEW(get_property_box()->sip.proxy_list)),get_core());
	linphone_refresh_proxy_combo_box(get_main_window()->window);
}

void
on_proxy_config_box_response           (GtkDialog       *dialog,
                                        gint             response_id,
                                        gpointer         user_data)
{
	switch(response_id){
		case GTK_RESPONSE_OK:
			get_proxy_config_box_data(GTK_WIDGET(dialog));
			gtk_widget_destroy(GTK_WIDGET(dialog));
			
		break;
	}
}


void codec_section_init(CodecSection *sec, GtkWidget *prop)
{
	sec->au_codec_list=lookup_widget(prop,"au_codec_list");
	sec->codec_info=lookup_widget(prop,"codec_info");
	sec->connection_type=lookup_widget(prop,"connection_type");
}

void codec_selection_changed_cb(GtkTreeSelection *selection, gpointer data)
{
	GtkTreeIter iter;
    GtkTreeModel *model;
    struct _PayloadType *pt=NULL;
	LinphonePropertyBox *prop=get_property_box();
	
	if (gtk_tree_selection_get_selected (selection, &model, &iter))	
	{
		gtk_tree_model_get (model, &iter, CODEC_PRIVDATA, &pt, -1);
		g_return_if_fail(pt!=NULL);
		/* display the codec information */
		gtk_label_set_text(GTK_LABEL(prop->codec.codec_info),payload_type_get_description(pt));
	}
}

void codec_list_build(GtkTreeView *listview,GList *codeclist)
{
	GList *elem=codeclist;
	GtkListStore *store = gtk_list_store_new (CODEC_NCOLUMNS, G_TYPE_STRING,
																G_TYPE_STRING,
																G_TYPE_STRING,
																G_TYPE_STRING,
																G_TYPE_POINTER,
																G_TYPE_STRING);
	GtkTreeIter iter;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select=gtk_tree_view_get_selection (listview);
	while(elem!=NULL){
		gchar *status;
		gchar *bitrate,*rate;
		gchar *color;
		struct _PayloadType *pt=(struct _PayloadType *)elem->data;
		if (payload_type_enabled(pt)) status=_("Enabled");
		else status=_("Disabled");
		if (payload_type_usable(pt)) color="blue";
		else color="red";
		/* get an iterator */
		gtk_list_store_append(store,&iter);
		bitrate=g_strdup_printf("%g",((double)payload_type_get_bitrate(pt))/1000.0);
		rate=g_strdup_printf("%i",payload_type_get_rate(pt));
		gtk_list_store_set(store,&iter,	CODEC_NAME,payload_type_get_mime(pt),
										CODEC_RATE,rate,
										CODEC_BITRATE,bitrate,
										CODEC_STATUS,status,
										CODEC_PRIVDATA,(gpointer)pt,
										CODEC_COLOR,(gpointer)color,
										-1);
		g_free(rate);
		g_free(bitrate);
		elem=g_list_next(elem);
	}
	gtk_tree_view_set_model(listview,GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (_("Name"),
                                                   renderer,
                                                   "text", CODEC_NAME,
												   "foreground",CODEC_COLOR,
                                                   NULL);
	gtk_tree_view_append_column (listview, column);
	column = gtk_tree_view_column_new_with_attributes (_("Rate (Hz)"),
                                                   renderer,
                                                   "text", CODEC_RATE,
												   "foreground",CODEC_COLOR,
                                                   NULL);
	gtk_tree_view_append_column (listview, column);
	column = gtk_tree_view_column_new_with_attributes (_("Status"),
                                                   renderer,
                                                   "text", CODEC_STATUS,
													"foreground",CODEC_COLOR,
                                                   NULL);
	gtk_tree_view_append_column (listview, column);
	column = gtk_tree_view_column_new_with_attributes (_("Min bitrate (kbit/s)"),
                                                   renderer,
                                                   "text", CODEC_BITRATE,
													"foreground",CODEC_COLOR,
                                                   NULL);
	gtk_tree_view_append_column (listview, column);
	
	/* Setup the selection handler */
	select = gtk_tree_view_get_selection (listview);
	gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
	//gtk_tree_view_columns_autosize(GTK_TREE_VIEW (sec->interfaces));
	g_signal_connect (G_OBJECT (select), "changed",
                  G_CALLBACK (codec_selection_changed_cb),
                  NULL);
}

void codec_section_fill(CodecSection *sec, LinphoneCore *lc)
{
	GtkWidget *optionmenu=sec->connection_type;
	GtkWidget *menu;
	GtkItem *menu_item;
	GtkMenuShell *menushell;
	GList *item_list,*elem;
	/* display prefered codecs*/
	GList *audio_codecs=linphone_core_get_audio_codecs(lc);
	codec_list_build(GTK_TREE_VIEW(sec->au_codec_list),audio_codecs);
	g_list_free(audio_codecs);
	menu=gtk_option_menu_get_menu(GTK_OPTION_MENU(optionmenu));
	menushell=GTK_MENU_SHELL(menu);
	elem=item_list=menushell->children;
	
	/* 28k menu item*/
	menu_item=(GtkItem*)(elem->data);
	gtk_signal_connect(GTK_OBJECT(menu_item),"activate",(GtkSignalFunc)on_28k_selected,(gpointer)sec);
	elem=g_list_next(elem);
	/* 56k menu item */
    menu_item=(GtkItem*)(elem->data);
	gtk_signal_connect(GTK_OBJECT(menu_item),"activate",(GtkSignalFunc)on_56k_selected,(gpointer)sec);
	elem=g_list_next(elem);
	/* 128k menu item */
	menu_item=(GtkItem*)(elem->data);
	gtk_signal_connect(GTK_OBJECT(menu_item),"activate",(GtkSignalFunc)on_128k_selected,(gpointer)sec);
	elem=g_list_next(elem);
	/* ADSL menu item */
	menu_item=(GtkItem*)(elem->data);
	gtk_signal_connect(GTK_OBJECT(menu_item),"activate",(GtkSignalFunc)on_adsl_selected,(gpointer)sec);
	elem=g_list_next(elem);
	/* Ethernet menu item */
	menu_item=(GtkItem*)(elem->data);
	gtk_signal_connect(GTK_OBJECT(menu_item),"activate",(GtkSignalFunc)on_ethernet_selected,(gpointer)sec);
	elem=g_list_next(elem);
	sec->type_sel=linphone_core_get_connection_type(lc);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu),sec->type_sel);
	
	
}

void codec_section_apply(CodecSection *sec, LinphoneCore *lc)
{
	GtkTreeIter iter;
	struct _PayloadType *pt;
	GList *codeclist=NULL;
	gchar *status;
	GtkTreeModel *model=gtk_tree_view_get_model(GTK_TREE_VIEW(sec->au_codec_list));
	/* retrieve the codec list */
	g_return_if_fail(gtk_tree_model_get_iter_first(model,&iter));
	do{
		gtk_tree_model_get (model, &iter, CODEC_STATUS,&status,CODEC_PRIVDATA, &pt,-1);
		g_return_if_fail(pt!=NULL);
		if (strcmp(status,_("Enabled"))==0) payload_type_set_enable(pt,1);
		else payload_type_set_enable(pt,0);
		codeclist=g_list_append(codeclist,(gpointer)pt);
	}while (gtk_tree_model_iter_next(model,&iter));
	linphone_core_set_audio_codecs(lc,codeclist);
}

void sound_section_init(SoundSection *sec, GtkWidget *prop)
{
	sec->source_entry=lookup_widget(prop,"rec_source");
	sec->ringfileentry=lookup_widget(prop,"ringfileentry");
}


void
on_play_card_changed                   (GtkComboBox     *combobox,
                                        gpointer         user_data)
{
	int dev=gtk_combo_box_get_active(combobox);
	linphone_core_set_playback_device(get_core(),dev);
	property_box_changed();
}


void
on_capt_card_changed                   (GtkComboBox     *combobox,
                                        gpointer         user_data)
{
	int dev=gtk_combo_box_get_active(combobox);
	linphone_core_set_capture_device(get_core(),dev);
	property_box_changed();
}

void sound_section_fill(SoundSection *sec, LinphoneCore *lc)
{
	SndCardManager *cardmanager;
	SndCard *card;
	GtkComboBox *play_card=GTK_COMBO_BOX(lookup_widget(get_property_box()->prop,"play_card"));
	GtkComboBox *capt_card=GTK_COMBO_BOX(lookup_widget(get_property_box()->prop,"capt_card"));
	int i;
	/* select used sound drivers*/

	cardmanager=linphone_core_get_card_manager(lc);
	
	for (i=0;(card=snd_card_manager_get_card(cardmanager,i))!=0;i++){
		const gchar *carddesc=snd_card_get_identifier(card);
		gtk_combo_box_append_text(play_card,carddesc);
		gtk_combo_box_append_text(capt_card,carddesc);
	}
	/*select used cards */
	gtk_combo_box_set_active(play_card,linphone_core_get_playback_device(lc));
	gtk_combo_box_set_active(capt_card,linphone_core_get_capture_device(lc));
	/* select audio source*/
	switch(linphone_core_get_sound_source(lc))
	{
		case 'm':
			gtk_entry_set_text (GTK_ENTRY (sec->source_entry), _("micro"));
  			break;
  		case 'l':
  			gtk_entry_set_text (GTK_ENTRY (sec->source_entry), _("line"));
  			break;
		default:
			g_warning("Invalid source !");
  }
  {
		gchar *ringfile=linphone_core_get_ring(lc);
		gnome_file_entry_set_filename(GNOME_FILE_ENTRY(sec->ringfileentry),ringfile);
		
  }
}




void sound_section_apply(SoundSection *sec, LinphoneCore *lc)
{
	gchar *tmp;
	
	/* get audio source*/
	tmp=gtk_editable_get_chars(GTK_EDITABLE(sec->source_entry),0,-1);
	if (strcmp(tmp,_("micro"))==0) linphone_core_set_sound_source(lc,'m');
	else if (strcmp(tmp,_("line"))==0) linphone_core_set_sound_source(lc,'l');
	g_free(tmp);
	
	/* get ring path */
	tmp=gtk_editable_get_chars(GTK_EDITABLE(gnome_file_entry_gtk_entry(GNOME_FILE_ENTRY(sec->ringfileentry))),0,-1);
	linphone_core_set_ring(lc,tmp);
	g_free(tmp);
	
}

void ring_finished(LinphoneCore *lc,gpointer user_data)
{
  GtkWidget *button=(GtkWidget*)user_data;
  LinphonePropertyBox *prop=get_property_box();
  if (prop->prop==NULL) return; /* the box has been closed before the end of the preview */
  gtk_widget_set_sensitive(button,1);
}

void
on_ringpreview_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	int err;
	gchar *tmp;
	LinphonePropertyBox *prop=get_property_box();
	LinphoneCore *lc=get_core();
	/* get ring path */
	tmp=gtk_editable_get_chars(GTK_EDITABLE(gnome_file_entry_gtk_entry(GNOME_FILE_ENTRY(prop->sound.ringfileentry))),0,-1);
	err=linphone_core_preview_ring(lc,tmp,ring_finished,(gpointer)button);
	if (err==0) gtk_widget_set_sensitive(GTK_WIDGET(button),0);
	g_free(tmp);
}


void linphone_property_box_fill(LinphonePropertyBox * box, LinphoneCore *lp)
{
	net_section_fill(&box->net,lp);
	sip_section_fill(&box->sip,lp);
	codec_section_fill(&box->codec,lp);
	sound_section_fill(&box->sound,lp);
	/* set uchanged state to the prop1 box, because gtk_entry_set_text() causes signal "changed"
  	to be sent */
	gnome_property_box_set_state(GNOME_PROPERTY_BOX(box->prop),0);
}

void linphone_property_box_init(LinphonePropertyBox *box)
{
	GtkWidget *prop=create_prop1();
	box->prop=prop;
	net_section_init(&box->net,prop);
	sip_section_init(&box->sip,prop);
	codec_section_init(&box->codec,prop);
	sound_section_init(&box->sound,prop);
	gtk_widget_show(prop);
	linphone_property_box_fill(box,get_core());
}




void on_propertybox1_apply (GtkButton       *button,
                                        gint pagenum)
{	
	LinphonePropertyBox *prop=get_property_box();
	LinphoneCore *lc=get_core();
	switch(pagenum)
	{
	case 0:
		net_section_apply(&prop->net,lc);
		break;
	case 1:
		sound_section_apply(&prop->sound,lc);
		break;
	case 2:
		sip_section_apply(&prop->sip,lc);
  		break;
  	case 3:
  	 	codec_section_apply(&prop->codec,lc);
  		break;
  	}
}


void
on_audioport_changed                   (GtkEditable     *editable,
                                        gpointer         user_data)
{
	LinphonePropertyBox *prop=get_property_box();
	gnome_property_box_changed(GNOME_PROPERTY_BOX(prop->prop));
	return;
}


void
on_sipport_changed                     (GtkEditable     *editable,
                                        gpointer         user_data)
{
	LinphonePropertyBox *prop=get_property_box();
	gnome_property_box_changed(GNOME_PROPERTY_BOX(prop->prop));
	return;
}


void
on_source_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
	LinphonePropertyBox *prop=get_property_box();
	gnome_property_box_changed(GNOME_PROPERTY_BOX(prop->prop));
}

enum {
	CODEC_ACTION_UP,
	CODEC_ACTION_DOWN
};


void codec_row_move(GtkTreeView *listview, int action)
{
	GtkTreeIter iter;
	GtkTreeIter previter,nextiter,newiter;
    GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreePath *treepath;
    struct _PayloadType *codec=NULL;
	gchar *name=NULL;
	gchar *rate=NULL;
	gchar *bitrate=NULL;
	gchar *status=NULL;
	gchar *color=NULL;
	
	selection=gtk_tree_view_get_selection(listview);
	if (gtk_tree_selection_get_selected (selection, &model, &iter))	
	{
		gtk_tree_model_get (model, &iter, 	CODEC_NAME,&name,
											CODEC_RATE,&rate,
											CODEC_BITRATE,&bitrate,
											CODEC_STATUS,&status,
											CODEC_PRIVDATA, &codec,
											CODEC_COLOR,&color,
											-1);
		g_return_if_fail(codec!=NULL);
		switch(action){
			case CODEC_ACTION_UP:
				/* get an iterator on the prev codec */
				treepath=gtk_tree_model_get_path(model,&iter);
				if (!gtk_tree_path_prev(treepath)){
					/* codec is the first, no match. */
					return;
				}
				gtk_tree_model_get_iter(model,&previter,treepath);
				gtk_list_store_insert_before(GTK_LIST_STORE(model),&newiter,&previter);
			break;
			case CODEC_ACTION_DOWN:
				/* get an iterator on the next codec */
				nextiter=iter;
				if (!gtk_tree_model_iter_next(model,&nextiter)){
					/* codec is the last, no match. */
					return;
				}
				gtk_list_store_insert_after(GTK_LIST_STORE(model),&newiter,&nextiter);
				
			break;
		}
		gtk_list_store_set(GTK_LIST_STORE(model),&newiter,
											CODEC_NAME,name,
											CODEC_RATE,rate,
											CODEC_BITRATE,bitrate,
											CODEC_STATUS,status,
											CODEC_PRIVDATA, codec,
											CODEC_COLOR,color,
											-1);
		/* remove the selected line */
		gtk_list_store_remove(GTK_LIST_STORE(model),&iter);
		gtk_tree_selection_select_iter(selection,&newiter);
		g_free(name);
		g_free(bitrate);
		g_free(rate);
		g_free(status);
		g_free(color);
	}
}

void
on_aucodec_up_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	LinphonePropertyBox *prop=get_property_box();
	GtkTreeView *listview=GTK_TREE_VIEW(prop->codec.au_codec_list);
	codec_row_move(listview,CODEC_ACTION_UP);
	property_box_changed();
}


void
on_aucodec_down_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
	LinphonePropertyBox *prop=get_property_box();
	GtkTreeView *listview=GTK_TREE_VIEW(prop->codec.au_codec_list);
	codec_row_move(listview,CODEC_ACTION_DOWN);
	property_box_changed();
}

void codec_set_status(GtkTreeView *listview,gboolean status)
{
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	gchar *statusstring;
	selection=gtk_tree_view_get_selection(listview);
	if (gtk_tree_selection_get_selected (selection, &model, &iter))	
	{
		if (status) statusstring=_("Enabled");
		else statusstring=_("Disabled");
		gtk_list_store_set(GTK_LIST_STORE(model),&iter,CODEC_STATUS,statusstring,-1);
	}
}

void codec_list_update(GtkTreeView *listview,double bandwidth)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	struct _PayloadType *pt;
	gchar *color;
	model=gtk_tree_view_get_model(listview);
	
	g_return_if_fail(gtk_tree_model_get_iter_first(model,&iter));
	do{
		gtk_tree_model_get (model, &iter, 	CODEC_PRIVDATA, &pt,-1);
		if (payload_type_usable(pt)){
			color="blue";
		}else color="red";
		gtk_list_store_set(GTK_LIST_STORE(model),&iter,CODEC_COLOR,color,-1);
		
	}while (gtk_tree_model_iter_next(model,&iter));
}


void
on_aucodec_enable_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	codec_set_status(GTK_TREE_VIEW(get_property_box()->codec.au_codec_list),TRUE);
	property_box_changed();
}


void
on_aucodec_disable_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
	codec_set_status(GTK_TREE_VIEW(get_property_box()->codec.au_codec_list),FALSE);
	property_box_changed();
}


void
on_user_name_changed                   (GtkEditable     *editable,
                                        gpointer         user_data)
{
	property_box_changed();
}


void
on_domain_name_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
	property_box_changed();
}



void
on_reg_passwd_changed                  (GtkEditable     *editable,
                                        gpointer         user_data)
{
   property_box_changed();
}


void
on_obproxy_button_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
   property_box_changed();
}

void
on_address_of_record_changed           (GtkEditable     *editable,
                                        gpointer         user_data)
{
    property_box_changed();
}


void
on_audio_jittcomp_value_changed        (GtkRange        *range,
                                        gpointer         user_data)
{
	property_box_changed();
}

void
on_ringfileentry_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
	property_box_changed();
}

void linphone_property_box_uninit(LinphonePropertyBox *box)
{
	memset(box,0, sizeof(LinphonePropertyBox));
}


gboolean
on_property_box_closed                 (GnomeDialog     *gnomedialog,
                                        gpointer         user_data)
{
  linphone_property_box_uninit(get_property_box());
  return FALSE;
}

void
on_use_sipinfo_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	linphone_core_set_use_info_for_dtmf(get_core(),gtk_toggle_button_get_active(togglebutton));
}


void
on_guess_hostname_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
	GtkWidget *hostname;
	osip_from_t *from;
	gboolean val=gtk_toggle_button_get_active(togglebutton);
	linphone_core_set_guess_hostname(get_core(),val);
	hostname=get_property_box()->sip.hostname;
	from=linphone_core_get_primary_contact_parsed(get_core());
	gtk_entry_set_text(GTK_ENTRY(hostname),from->url->host);
	gtk_widget_set_sensitive(hostname,!val);
	osip_from_free(from);
}
